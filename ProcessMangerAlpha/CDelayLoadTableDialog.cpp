// CDelayLoadTableDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CDelayLoadTableDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"

// CDelayLoadTableDialog 对话框

IMPLEMENT_DYNAMIC(CDelayLoadTableDialog, CDialogEx)

CDelayLoadTableDialog::CDelayLoadTableDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DelayLoadTableDialog, pParent)
	, strDllNameRVA(_T(""))
	, strModuleHandleRVA(_T(""))
	, strIATRVA(_T(""))
	, strINTRVA(_T(""))
	, strBIATRVA(_T(""))
	, strUITRVA(_T(""))
{

}

CDelayLoadTableDialog::~CDelayLoadTableDialog()
{
}

void CDelayLoadTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, strDllNameRVA);
	DDX_Text(pDX, IDC_EDIT2, strModuleHandleRVA);
	DDX_Text(pDX, IDC_EDIT3, strIATRVA);
	DDX_Text(pDX, IDC_EDIT4, strINTRVA);
	DDX_Text(pDX, IDC_EDIT5, strBIATRVA);
	DDX_Text(pDX, IDC_EDIT6, strUITRVA);
}


BEGIN_MESSAGE_MAP(CDelayLoadTableDialog, CDialogEx)
END_MESSAGE_MAP()


// CDelayLoadTableDialog 消息处理程序


BOOL CDelayLoadTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	getDelayLoadTableInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取延迟载入表信息
void CDelayLoadTableDialog::getDelayLoadTableInfo()
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER(&ntHead->OptionalHeader);
	// 找到数据目录表的第三项，也就是下标为2
	DWORD delayTableRVA = optHead->DataDirectory[13].VirtualAddress;
	DWORD delayTableFOA = rvaTofoa(delayTableRVA);
	// 资源表的基址
	PIMAGE_DELAYLOAD_DESCRIPTOR delayTable = PIMAGE_DELAYLOAD_DESCRIPTOR(delayTableFOA + (DWORD)fileBuff);
	strDllNameRVA.Format(_T("%.8X"), delayTable->DllNameRVA);
	strModuleHandleRVA.Format(_T("%.8X"), delayTable->ModuleHandleRVA);
	strIATRVA.Format(_T("%.8X"), delayTable->ImportAddressTableRVA);
	strINTRVA.Format(_T("%.8X"), delayTable->ImportNameTableRVA);
	strBIATRVA.Format(_T("%.8X"), delayTable->BoundImportAddressTableRVA);
	strUITRVA.Format(_T("%.8X"), delayTable->UnloadInformationTableRVA);
	UpdateData(FALSE);
}
