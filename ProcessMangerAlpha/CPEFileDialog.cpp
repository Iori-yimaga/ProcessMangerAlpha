// CPEFileDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CPEFileDialog.h"
#include "afxdialogex.h"

// 声明一下全局变量
char* fileBuff = NULL;
// 获取DOS头
extern PIMAGE_DOS_HEADER dosHead = NULL;
// 获取NT头
extern PIMAGE_NT_HEADERS ntHead = NULL;
// 获取文件头
extern PIMAGE_FILE_HEADER fileHead = NULL;
// 获取扩展头
extern PIMAGE_OPTIONAL_HEADER32 optHead = NULL;

// CPEFileDialog 对话框

IMPLEMENT_DYNAMIC(CPEFileDialog, CDialogEx)

CPEFileDialog::CPEFileDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PEFileDialog, pParent)
	, wMachine(_T(""))
	, dwBaseOfCode(_T(""))
	, dwBaseOfData(_T(""))
	, dwBaseImage(_T(""))
	, dwSectionAlign(_T(""))
	, dwFileAlign(_T(""))
	, dwSizeOfImage(_T(""))
	, dwSizeOfHeads(_T(""))
	, wDllCharacts(_T(""))
	, dwNumberOfRvaAndSizes(_T(""))
	, dwTime(_T(""))
	, wSizeOfOptHead(_T(""))
	, wCharacteristic(_T(""))
	, wMagic(_T(""))
	, dwSizeOfCode(_T(""))
	, dwSizeOfInitCode(_T(""))
	, dwSizeOfUnInitCode(_T(""))
	, dwOEP(_T(""))
{

}

CPEFileDialog::~CPEFileDialog()
{
}

void CPEFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objSectionList);
	DDX_Text(pDX, IDC_EDIT1, wMachine);
	DDX_Text(pDX, IDC_EDIT10, dwBaseOfCode);
	DDX_Text(pDX, IDC_EDIT11, dwBaseOfData);
	DDX_Text(pDX, IDC_EDIT12, dwBaseImage);
	DDX_Text(pDX, IDC_EDIT13, dwSectionAlign);
	DDX_Text(pDX, IDC_EDIT14, dwFileAlign);
	DDX_Text(pDX, IDC_EDIT15, dwSizeOfImage);
	DDX_Text(pDX, IDC_EDIT16, dwSizeOfHeads);
	DDX_Text(pDX, IDC_EDIT17, wDllCharacts);
	DDX_Text(pDX, IDC_EDIT18, dwNumberOfRvaAndSizes);
	DDX_Text(pDX, IDC_EDIT2, dwTime);
	DDX_Text(pDX, IDC_EDIT3, wSizeOfOptHead);
	DDX_Text(pDX, IDC_EDIT4, wCharacteristic);
	DDX_Text(pDX, IDC_EDIT5, wMagic);
	DDX_Text(pDX, IDC_EDIT6, dwSizeOfCode);
	DDX_Text(pDX, IDC_EDIT7, dwSizeOfInitCode);
	DDX_Text(pDX, IDC_EDIT8, dwSizeOfUnInitCode);
	DDX_Text(pDX, IDC_EDIT9, dwOEP);
}


BEGIN_MESSAGE_MAP(CPEFileDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPEFileDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPEFileDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPEFileDialog::OnBnClickedButton3)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPEFileDialog 消息处理程序


BOOL CPEFileDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	objSectionList.InsertColumn(0, _T("区段名"), LVCFMT_CENTER, 80);
	objSectionList.InsertColumn(1, _T("内存中起始位置"), LVCFMT_CENTER, 100);
	objSectionList.InsertColumn(2, _T("内存中大小"), LVCFMT_CENTER, 100);
	objSectionList.InsertColumn(3, _T("文件中起始位置"), LVCFMT_CENTER, 100);
	objSectionList.InsertColumn(4, _T("文件中大小"), LVCFMT_CENTER, 100);
	objSectionList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 选择文件
void CPEFileDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog* pFilePicker = new CFileDialog(true, _T("文件名"), _T("都行"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File (*.*)|*.*||"), NULL);
	pFilePicker->DoModal();
	strFilePath = pFilePicker->GetPathName();
	// 打开一个文件
	HANDLE hFile = CreateFile(
		strFilePath,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	// 获取文件大小 
	DWORD fileSize = GetFileSize(hFile, NULL);
	// 申请空间保存文件内容, 如果读取成功就申请空间
	fileBuff = new char[fileSize];
	// 读取文件
	DWORD read_size = 0;
	errno_t err = ReadFile(hFile, fileBuff, fileSize, &read_size, NULL);
	// 关闭文件
	CloseHandle(hFile);
	// 判断是否是 PE 文件
	if (isPEFile(fileBuff)) {
		getFileHeadInfo(fileBuff);
		getOptionalHeadInfo(fileBuff);
		getSectionHeadInfo(fileBuff);
		UpdateData(FALSE);
	}
	else {
		MessageBox(_T("这不是一个PE文件"), _T("Error"), 0);
	}
}

// 判断是否为PE文件
bool CPEFileDialog::isPEFile(PVOID fileBuff)
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 判断DOS头前两个字节是否为5A4D
	if (dosHead->e_magic == IMAGE_DOS_SIGNATURE) {
		// 如果是，则获取其NT头
		ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
		// 判断NT头前四个字节是否为0000 4550
		if (ntHead->Signature == IMAGE_NT_SIGNATURE) {
			// 如果是，则返回true
			return true;
		}
		// 否则返回false
		else {
			return false;
		}
	}
	// 否则返回false
	else {
		return false;
	}
}

// 获取文件头信息
void CPEFileDialog::getFileHeadInfo(PVOID fileBuff)
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取文件头
	fileHead = PIMAGE_FILE_HEADER(&ntHead->FileHeader);
	// 平台信息
	if (fileHead->Machine == IMAGE_FILE_MACHINE_I386) {
		wMachine = _T("Windows");
	}
	else {
		wMachine = _T("UnKnown");
	}
	// 时间
	struct tm fileTime = {};
	errno_t err = gmtime_s(&fileTime, (time_t*)&fileHead->TimeDateStamp);
	dwTime.Format(_T("%d-%d-%d %d:%d:%d"), fileTime.tm_year + 1900, fileTime.tm_mon + 1, fileTime.tm_mday, fileTime.tm_hour + 8, fileTime.tm_min, fileTime.tm_sec);
	// 可选头部大小
	wSizeOfOptHead.Format(_T("%X"), fileHead->SizeOfOptionalHeader);
	// 文件属性值
	wCharacteristic.Format(_T("%X"), fileHead->Characteristics);
}

// 获取扩展头信息
void CPEFileDialog::getOptionalHeadInfo(PVOID fileBuff)
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER32(&ntHead->OptionalHeader);
	// 标志字
	wMagic.Format(_T("%X"), optHead->Magic);
	// 所有代码区段的总大小
	dwSizeOfCode.Format(_T("%X"), optHead->SizeOfCode);
	// 已初始化数据的总大小
	dwSizeOfInitCode.Format(_T("%X"), optHead->SizeOfInitializedData);
	// 未初始化数据的总大小
	dwSizeOfUnInitCode.Format(_T("%X"), optHead->SizeOfUninitializedData);
	// 程序开始执行的相对虚拟地址
	dwOEP.Format(_T("%X"), optHead->AddressOfEntryPoint);
	// 起始代码的相对虚拟地址
	dwBaseOfCode.Format(_T("%X"), optHead->BaseOfCode);
	// 起始数据的相对虚拟地址
	dwBaseOfData.Format(_T("%X"), optHead->BaseOfData);
	// 默认加载基址
	dwBaseImage.Format(_T("%X"), optHead->ImageBase);
	// 块对齐数
	dwSectionAlign.Format(_T("%X"), optHead->SectionAlignment);
	// 文件对齐数
	dwFileAlign.Format(_T("%X"), optHead->FileAlignment);
	// 映像大小（文件被加载进内存后大小，需要注意对齐）
	dwSizeOfImage.Format(_T("%X"), optHead->SizeOfImage);
	// 所有头部小（文件被加载进内存后大小，需要注意对齐）
	dwSizeOfHeads.Format(_T("%X"), optHead->SizeOfHeaders);
	// Dll 特征标志
	wDllCharacts.Format(_T("%X"), optHead->DllCharacteristics);
	// 数据目录的个数
	dwNumberOfRvaAndSizes.Format(_T("%X"), optHead->NumberOfRvaAndSizes);
}

// 获取区段头信息
void CPEFileDialog::getSectionHeadInfo(PVOID fileBuff)
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取文件头
	fileHead = PIMAGE_FILE_HEADER(&ntHead->FileHeader);
	// 获取区段数量
	DWORD secCount = fileHead->NumberOfSections;
	// 获取第一个区段的头
	PIMAGE_SECTION_HEADER secHead = IMAGE_FIRST_SECTION(ntHead);
	objSectionList.DeleteAllItems();
	// 遍历所有的区段
	for (unsigned int i = 0; i < secCount; i++) {
		// 区段名
		objSectionList.InsertItem(i, _T(""));
		CString name;
		name.Format(_T("%S"), secHead->Name);
		objSectionList.SetItemText(i, 0, name);
		// 内存位置和大小
		CString secVA, secVS;
		secVA.Format(_T("%.8X"), secHead->VirtualAddress);
		secVS.Format(_T("%.8X"), secHead->Misc.VirtualSize);
		objSectionList.SetItemText(i, 1, secVA);
		objSectionList.SetItemText(i, 2, secVS);
		// 文件位置和大小
		CString secPTRD, secSORD;
		secPTRD.Format(_T("%.8X"), secHead->PointerToRawData);
		secSORD.Format(_T("%.8X"), secHead->SizeOfRawData);
		objSectionList.SetItemText(i, 3, secPTRD);
		objSectionList.SetItemText(i, 4, secSORD);
		secHead++;
	}
}

// 地址计算器
void CPEFileDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (fileBuff == NULL) {
		MessageBox(_T("先选一个文件再来吧"), _T("0_o"), 0);
	}
	else {
		CAddCalcDialog* objAd = new CAddCalcDialog;
		objAd->DoModal();
	}
}

// 目录表
void CPEFileDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (fileBuff == NULL) {
		MessageBox(_T("先选一个文件再来吧"), _T("0_o"), 0);
	}
	else {
		CDirListDialog* objLd = new CDirListDialog;
		objLd->DoModal();
	}
}

// 实现文件拖拽
//void CPEFileDialog::OnDropFiles(HDROP hDropInfo)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	// 解析文件名
//	//DragQueryFile(hDropInfo, 0, (LPWSTR)(LPCWSTR)strFilePath, 100);
//	//// 打开一个文件
//	//HANDLE hFile = CreateFile(
//	//	strFilePath,
//	//	GENERIC_READ,
//	//	NULL,
//	//	NULL,
//	//	OPEN_EXISTING,
//	//	FILE_ATTRIBUTE_NORMAL,
//	//	NULL
//	//);
//	//// 获取文件大小 
//	//DWORD fileSize = GetFileSize(hFile, NULL);
//	//// 申请空间保存文件内容, 如果读取成功就申请空间
//	//fileBuff = new char[fileSize];
//	//// 读取文件
//	//DWORD read_size = 0;
//	//errno_t err = ReadFile(hFile, fileBuff, fileSize, &read_size, NULL);
//	//// 关闭文件
//	//CloseHandle(hFile);
//	//// 判断是否是 PE 文件
//	//if (isPEFile(fileBuff)) {
//	//	getFileHeadInfo(fileBuff);
//	//	getOptionalHeadInfo(fileBuff);
//	//	getSectionHeadInfo(fileBuff);
//	//	UpdateData(FALSE);
//	//}
//	//else {
//	//	MessageBox(_T("这不是一个PE文件"), _T("Error"), 0);
//	//}
//	CDialogEx::OnDropFiles(hDropInfo);
//}
