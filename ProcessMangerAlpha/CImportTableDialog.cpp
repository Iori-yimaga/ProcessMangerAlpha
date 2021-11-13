// CImportTableDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CImportTableDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"

// 全局变量
PIMAGE_IMPORT_DESCRIPTOR importTable = NULL;
PIMAGE_THUNK_DATA IAT = NULL;

// CImportTableDialog 对话框

IMPLEMENT_DYNAMIC(CImportTableDialog, CDialogEx)

CImportTableDialog::CImportTableDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImportTableDialog, pParent)
{

}

CImportTableDialog::~CImportTableDialog()
{
}

void CImportTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objModuleList);
	DDX_Control(pDX, IDC_LIST2, objFuncList);
}


BEGIN_MESSAGE_MAP(CImportTableDialog, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CImportTableDialog::OnClickList1)
END_MESSAGE_MAP()


// CImportTableDialog 消息处理程序


BOOL CImportTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化模块表
	objModuleList.InsertColumn(0, _T("DllName"), LVCFMT_CENTER, 80);
	objModuleList.InsertColumn(1, _T("OriginalFirstThunk"), LVCFMT_CENTER, 100);
	objModuleList.InsertColumn(2, _T("TimeDateStamp"), LVCFMT_CENTER, 100);
	objModuleList.InsertColumn(3, _T("ForwarderChain"), LVCFMT_CENTER, 100);
	objModuleList.InsertColumn(4, _T("名称"), LVCFMT_CENTER, 100);
	objModuleList.InsertColumn(5, _T("FirstThunk"), LVCFMT_CENTER, 100);
	objModuleList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	getImportTableInfo();
	// 初始化函数表
	objFuncList.InsertColumn(0, _T("ThunkRVA"), LVCFMT_CENTER, 80);
	objFuncList.InsertColumn(1, _T("ThunkOffset"), LVCFMT_CENTER, 100);
	objFuncList.InsertColumn(2, _T("ThunkValue"), LVCFMT_CENTER, 100);
	objFuncList.InsertColumn(3, _T("Hint"), LVCFMT_CENTER, 100);
	objFuncList.InsertColumn(4, _T("ApiName"), LVCFMT_CENTER, 100);
	objFuncList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取导入表信息（获取所有模块）
void CImportTableDialog::getImportTableInfo()
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER(&ntHead->OptionalHeader);
	// 找到数据目录表的第二项，也就是下表为1
	DWORD importTableRVA = optHead->DataDirectory[1].VirtualAddress;
	DWORD importTableFOA = rvaTofoa(importTableRVA);
	// 获取到导入表（第一个）
	importTable = PIMAGE_IMPORT_DESCRIPTOR(importTableFOA + (DWORD)fileBuff);

	// 按名字遍历导入表
	int i = 0;
	while (importTable->Name != 0) {
		objModuleList.InsertItem(i, _T(""));
		// 获取模块名
		DWORD nameFOA = rvaTofoa(importTable->Name);
		PCHAR name = PCHAR(nameFOA + (DWORD)fileBuff);
		CString strDllName;
		strDllName.Format(_T("%S"), name);
		objModuleList.SetItemText(i, 0, strDllName);
		// 获取名称导入表的FOA (INT)
		DWORD INTRVA = importTable->OriginalFirstThunk;
		CString strINTRVA;
		strINTRVA.Format(_T("%.8X"), INTRVA);
		objModuleList.SetItemText(i, 1, strINTRVA);
		// 获取时间戳
		DWORD TimeDateStamp = importTable->TimeDateStamp;
		CString strTDS;
		strTDS.Format(_T("%.8X"), TimeDateStamp);
		objModuleList.SetItemText(i, 2, strTDS);
		// 获取转发信息
		DWORD ForwarderChain = importTable->ForwarderChain;
		CString strFC;
		strFC.Format(_T("%.8X"), ForwarderChain);
		objModuleList.SetItemText(i, 3, strFC);
		// 获取名称地址
		DWORD DllNameRVA = importTable->Name;
		CString strDllNameRVA;
		strDllNameRVA.Format(_T("%.8X"), DllNameRVA);
		objModuleList.SetItemText(i, 4, strDllNameRVA);

		// 获取导入的地址表 (IAT)
		DWORD IATRVA = importTable->FirstThunk;
		CString strIATRVA;
		strIATRVA.Format(_T("%.8X"), IATRVA);
		objModuleList.SetItemText(i, 5, strIATRVA);
		importTable++;
		i++;
	}
}


// 模块列表左键点击事件
void CImportTableDialog::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nRow = pNMItemActivate->iItem;
	// 获取IAT的RVA
	CString strIATRVA = objModuleList.GetItemText(nRow, 5);
	DWORD IATRVA = _tcstol(strIATRVA, NULL, 16);
	objFuncList.DeleteAllItems();
	getFuncInfo(IATRVA);
	*pResult = 0;
}

// 获取函数信息
void CImportTableDialog::getFuncInfo(DWORD IATRVA)
{
	// TODO: 在此处添加实现代码.
	DWORD IATFOA = rvaTofoa(IATRVA);
	IAT = PIMAGE_THUNK_DATA(IATFOA + (DWORD)fileBuff);
	// 遍历导入地址表的地址
	int i = 0;
   	while (IAT->u1.AddressOfData != 0) {
		objFuncList.InsertItem(i, _T(""));
		CString strIATRVA, strIATFOA, strThunkValue;
		strIATRVA.Format(_T("%.8X"), IATRVA);
		strIATFOA.Format(_T("%.8X"), IATFOA);
		strThunkValue.Format(_T("%.8X"), *(PDWORD)(IATFOA + (DWORD)fileBuff));
		objFuncList.SetItemText(i, 0, strIATRVA);
		objFuncList.SetItemText(i, 1, strIATFOA);
		objFuncList.SetItemText(i, 2, strThunkValue);
		IATRVA += 4;
		IATFOA += 4;
		// 按序号导入
		BOOL isOnlyOrdinal = IMAGE_SNAP_BY_ORDINAL32(IAT->u1.Ordinal);
		// 如果不是只按序号导入
		if (!isOnlyOrdinal) {
			// 输出序号和名字
			// 获取名字的地址
			DWORD nameTableFOA = rvaTofoa(IAT->u1.AddressOfData);
			PIMAGE_IMPORT_BY_NAME nameTable = PIMAGE_IMPORT_BY_NAME(nameTableFOA + (DWORD)fileBuff);
			//printf("序号：0x%2X\t 函数名：%s\n", nameTable->Hint, nameTable->Name);
			CString strHint, strApiName;
			strHint.Format(_T("%.4X"), nameTable->Hint);
			strApiName.Format(_T("%S"), nameTable->Name);
			objFuncList.SetItemText(i, 3, strHint);
			objFuncList.SetItemText(i, 4, strApiName);
		}
		else {
			// 如果最高位是1，就是序号导入方式
			// 去掉最高位，低位保存的就是序号
			//printf("序号：0x%2X\n", IAT->u1.Ordinal & 0x7FFFFFFF);
			CString strHint, strApiName;
			strHint.Format(_T("-.-"));
			strApiName.Format(_T("Ordinal: %.8X"), IAT->u1.Ordinal & 0x7FFFFFFF);
			objFuncList.SetItemText(i, 3, strHint);
			objFuncList.SetItemText(i, 4, strApiName);
		}
		IAT++;
		i++;
	}
}
