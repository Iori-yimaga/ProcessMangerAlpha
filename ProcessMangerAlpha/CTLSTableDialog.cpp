// CTLSTableDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CTLSTableDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"


// CTLSTableDialog 对话框

IMPLEMENT_DYNAMIC(CTLSTableDialog, CDialogEx)

CTLSTableDialog::CTLSTableDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TLSTableDialog, pParent)
	, strBeginVA(_T(""))
	, strEndVA(_T(""))
	, strIndexVA(_T(""))
	, strCallBackVA(_T(""))
	, strZero(_T(""))
	, strCharact(_T(""))
{

}

CTLSTableDialog::~CTLSTableDialog()
{
}

void CTLSTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, strBeginVA);
	DDX_Text(pDX, IDC_EDIT2, strEndVA);
	DDX_Text(pDX, IDC_EDIT3, strIndexVA);
	DDX_Text(pDX, IDC_EDIT4, strCallBackVA);
	DDX_Text(pDX, IDC_EDIT5, strZero);
	DDX_Text(pDX, IDC_EDIT6, strCharact);
}


BEGIN_MESSAGE_MAP(CTLSTableDialog, CDialogEx)
END_MESSAGE_MAP()


// CTLSTableDialog 消息处理程序


BOOL CTLSTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	getTLSTableInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取TLS表信息
void CTLSTableDialog::getTLSTableInfo()
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER32(&ntHead->OptionalHeader);
	// 找到数据目录表的第三项，也就是下标为2
	DWORD TLSTableRVA = optHead->DataDirectory[9].VirtualAddress;
	DWORD TLSTableFOA = rvaTofoa(TLSTableRVA);
	// 资源表的基址
	PIMAGE_TLS_DIRECTORY TLSTable = PIMAGE_TLS_DIRECTORY(TLSTableFOA + (DWORD)fileBuff);
	strBeginVA.Format(_T("%.8X"), TLSTable->StartAddressOfRawData);
	strEndVA.Format(_T("%.8X"), TLSTable->EndAddressOfRawData);
	strIndexVA.Format(_T("%.8X"), TLSTable->AddressOfIndex);
	strCallBackVA.Format(_T("%.8X"), TLSTable->AddressOfCallBacks);
	strZero.Format(_T("%.8X"), TLSTable->SizeOfZeroFill);
	strCharact.Format(_T("%.8X"), TLSTable->Characteristics);
	UpdateData(FALSE);
}
