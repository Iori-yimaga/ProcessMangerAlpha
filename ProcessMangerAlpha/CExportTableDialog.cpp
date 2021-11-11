// CExportTableDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CExportTableDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"

// CExportTableDialog 对话框

IMPLEMENT_DYNAMIC(CExportTableDialog, CDialogEx)

CExportTableDialog::CExportTableDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ExportTableDialog, pParent)
	, strFOA(_T(""))
	, strFuncNameOrdinalAdd(_T(""))
	, strCharact(_T(""))
	, strBase(_T(""))
	, strNameOffset(_T(""))
	, strName(_T(""))
	, strNumberOfFunc(_T(""))
	, strNumberOfFuncName(_T(""))
	, strFuncAdd(_T(""))
	, strFuncNameAdd(_T(""))
{

}

CExportTableDialog::~CExportTableDialog()
{
}

void CExportTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objExportTab);
	DDX_Text(pDX, IDC_EDIT1, strFOA);
	DDX_Text(pDX, IDC_EDIT10, strFuncNameOrdinalAdd);
	DDX_Text(pDX, IDC_EDIT2, strCharact);
	DDX_Text(pDX, IDC_EDIT3, strBase);
	DDX_Text(pDX, IDC_EDIT4, strNameOffset);
	DDX_Text(pDX, IDC_EDIT5, strName);
	DDX_Text(pDX, IDC_EDIT6, strNumberOfFunc);
	DDX_Text(pDX, IDC_EDIT7, strNumberOfFuncName);
	DDX_Text(pDX, IDC_EDIT8, strFuncAdd);
	DDX_Text(pDX, IDC_EDIT9, strFuncNameAdd);
}


BEGIN_MESSAGE_MAP(CExportTableDialog, CDialogEx)
END_MESSAGE_MAP()


// CExportTableDialog 消息处理程序


BOOL CExportTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	// 初始化列表区
	objExportTab.InsertColumn(0, _T("Ordinal"), LVCFMT_CENTER, 80);
	objExportTab.InsertColumn(1, _T("RVA"), LVCFMT_CENTER, 100);
	objExportTab.InsertColumn(2, _T("Offset"), LVCFMT_CENTER, 100);
	objExportTab.InsertColumn(3, _T("Function Name"), LVCFMT_CENTER, 100);
	objExportTab.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 获取输出表的信息
	getExportTableInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取输出表的信息
void CExportTableDialog::getExportTableInfo()
{
	// TODO: 在此处添加实现代码.
	// 获取头部信息
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER(&ntHead->OptionalHeader);
	// 先找到导出表，在数据目录表的 第 1 项，下标为 0
	DWORD exportRVA = optHead->DataDirectory[0].VirtualAddress;
	DWORD exportFOA = rvaTofoa(exportRVA);
	// 输出表偏移
	strFOA.Format(_T("%8X"), exportFOA);
	PIMAGE_EXPORT_DIRECTORY  exportTable = PIMAGE_EXPORT_DIRECTORY(exportFOA + (DWORD)fileBuff);
	// 特征值
	strCharact.Format(_T("%8X"), exportTable->Characteristics);
	// 先根据名称的 rva 得到名称  foa
	// 再根据 foa 找到名称字符串在文件中的具体位置
	// 强制转换成字符串指针，进行输出
	// 
	// 名称
	strNameOffset.Format(_T("%8X"), exportTable->Name);

	// 名称字符串
	PCHAR name = PCHAR(rvaTofoa(exportTable->Name) + (DWORD)fileBuff);
	strName.Format(_T("%S"), name);

	// 基址
	DWORD base = exportTable->Base;

	// 序号基数（基址）
	strBase.Format(_T("%8X"), base);

	// 先获取到两张表的数量
	DWORD funcCount = exportTable->NumberOfFunctions;
	strNumberOfFunc.Format(_T("%8X"), funcCount);
	DWORD nameCount = exportTable->NumberOfNames;
	strNumberOfFuncName.Format(_T("%8X"), nameCount);

	// 得到 函数地址表
	DWORD funcTableRVA = exportTable->AddressOfFunctions;
	strFuncAdd.Format(_T("%8X"), funcTableRVA);
	DWORD funcTableFOA = rvaTofoa(funcTableRVA);
	PDWORD funcTable = PDWORD(funcTableFOA + (DWORD)fileBuff);

	// 得到 函数名称表
	DWORD nameTableRVA = exportTable->AddressOfNames;
	strFuncNameAdd.Format(_T("%8X"), nameTableRVA);
	DWORD nameTableFOA = rvaTofoa(nameTableRVA);
	PDWORD nameTable = PDWORD(nameTableFOA + (DWORD)fileBuff);

	// 得到 名称序号表
	DWORD ordinalTAbleRVA = exportTable->AddressOfNameOrdinals;
	strFuncNameOrdinalAdd.Format(_T("%8X"), ordinalTAbleRVA);
	DWORD ordinalTAbleFOA = rvaTofoa(ordinalTAbleRVA);
	PWORD ordinalTable = PWORD(ordinalTAbleFOA + (DWORD)fileBuff);

	// 因为地址表的数量多，所以使用地址表进行遍历
	int iCol = 0;
	for (unsigned int index = 0; index < funcCount; index++) {
		if (funcTable[index] == 0) {
			continue;
		}
		// 判断下标对应的函数是否在名称表里面，如果在，就是名称导出
		BOOL is_find = false;
		for (unsigned int i = 0; i < nameCount; i++) {
			// 得到序号表中的内容
			if (index == ordinalTable[i]) {
				PCHAR funcName = PCHAR(rvaTofoa(nameTable[i]) + (DWORD)fileBuff);
				CString strOrdinal, strRVA, strFOA, strName;
				strOrdinal.Format(_T("%4X"), ordinalTable[i] + base);
				strRVA.Format(_T("%8X"), funcTable[index]);
				strFOA.Format(_T("%8X"), rvaTofoa(funcTable[index]));
				strName.Format(_T("%S"), funcName);

				objExportTab.InsertItem(iCol, _T(""));
				objExportTab.SetItemText(iCol, 0, strOrdinal);
				objExportTab.SetItemText(iCol, 1, strRVA);
				objExportTab.SetItemText(iCol, 2, strFOA);
				objExportTab.SetItemText(iCol, 3, strName);
				iCol++;
				is_find = true;
				break;
			}
		}
		// 否则就是序号导出
		if (is_find == false) {
			CString strOrdinal, strRVA, strFOA, strName;

			strOrdinal.Format(_T("%4X"), index + base);
			strRVA.Format(_T("%8X"), funcTable[index]);
			strFOA.Format(_T("%8X"), rvaTofoa(funcTable[index]));
			strName.Format(_T("--"));

			objExportTab.InsertItem(iCol, _T(""));
			objExportTab.SetItemText(iCol, 0, strOrdinal);
			objExportTab.SetItemText(iCol, 1, strRVA);
			objExportTab.SetItemText(iCol, 2, strFOA);
			objExportTab.SetItemText(iCol, 3, strName);
			iCol++;
		}
	}
	UpdateData(FALSE);
}
