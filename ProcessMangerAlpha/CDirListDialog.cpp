// CDirListDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CDirListDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include <vector>
using std::vector;


// CDirListDialog 对话框

IMPLEMENT_DYNAMIC(CDirListDialog, CDialogEx)

CDirListDialog::CDirListDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DirListDialog, pParent)
	, strExportRVA(_T(""))
	, strTLSRVA(_T(""))
	, strLoadConfigRVA(_T(""))
	, strBoundImportRVA(_T(""))
	, strIATRVA(_T(""))
	, strDelayImportRVA(_T(""))
	, strCOMRVA(_T(""))
	, strBLRVA(_T(""))
	, strExportSize(_T(""))
	, strImportSize(_T(""))
	, strResourceSize(_T(""))
	, strImportRVA(_T(""))
	, strExceptionSize(_T(""))
	, strSecuritySize(_T(""))
	, strRelocSize(_T(""))
	, strDebugSize(_T(""))
	, strCopyrightSize(_T(""))
	, strGlobalptrSize(_T(""))
	, strTLSSize(_T(""))
	, strLoadConfigSize(_T(""))
	, strBoundImportSize(_T(""))
	, strIATSize(_T(""))
	, strResouceRVA(_T(""))
	, strDelayImportSize(_T(""))
	, strCOMSize(_T(""))
	, strBLSize(_T(""))
	, strExceptionRVA(_T(""))
	, strSecurityRVA(_T(""))
	, strRelocRVA(_T(""))
	, strDebugRVA(_T(""))
	, strCopyrightRVA(_T(""))
	, strGlobalptrRVA(_T(""))
{

}

CDirListDialog::~CDirListDialog()
{
}

void CDirListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, strExportRVA);
	DDX_Text(pDX, IDC_EDIT10, strTLSRVA);
	DDX_Text(pDX, IDC_EDIT11, strLoadConfigRVA);
	DDX_Text(pDX, IDC_EDIT12, strBoundImportRVA);
	DDX_Text(pDX, IDC_EDIT13, strIATRVA);
	DDX_Text(pDX, IDC_EDIT14, strDelayImportRVA);
	DDX_Text(pDX, IDC_EDIT15, strCOMRVA);
	DDX_Text(pDX, IDC_EDIT16, strBLRVA);
	DDX_Text(pDX, IDC_EDIT17, strExportSize);
	DDX_Text(pDX, IDC_EDIT18, strImportSize);
	DDX_Text(pDX, IDC_EDIT19, strResourceSize);
	DDX_Text(pDX, IDC_EDIT2, strImportRVA);
	DDX_Text(pDX, IDC_EDIT20, strExceptionSize);
	DDX_Text(pDX, IDC_EDIT21, strSecuritySize);
	DDX_Text(pDX, IDC_EDIT22, strRelocSize);
	DDX_Text(pDX, IDC_EDIT23, strDebugSize);
	DDX_Text(pDX, IDC_EDIT24, strCopyrightSize);
	DDX_Text(pDX, IDC_EDIT25, strGlobalptrSize);
	DDX_Text(pDX, IDC_EDIT26, strTLSSize);
	DDX_Text(pDX, IDC_EDIT27, strLoadConfigSize);
	DDX_Text(pDX, IDC_EDIT28, strBoundImportSize);
	DDX_Text(pDX, IDC_EDIT29, strIATSize);
	DDX_Text(pDX, IDC_EDIT3, strResouceRVA);
	DDX_Text(pDX, IDC_EDIT30, strDelayImportSize);
	DDX_Text(pDX, IDC_EDIT31, strCOMSize);
	DDX_Text(pDX, IDC_EDIT32, strBLSize);
	DDX_Text(pDX, IDC_EDIT4, strExceptionRVA);
	DDX_Text(pDX, IDC_EDIT5, strSecurityRVA);
	DDX_Text(pDX, IDC_EDIT6, strRelocRVA);
	DDX_Text(pDX, IDC_EDIT7, strDebugRVA);
	DDX_Text(pDX, IDC_EDIT8, strCopyrightRVA);
	DDX_Text(pDX, IDC_EDIT9, strGlobalptrRVA);
}


BEGIN_MESSAGE_MAP(CDirListDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDirListDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirListDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDirListDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDirListDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CDirListDialog::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDirListDialog::OnBnClickedButton8)
END_MESSAGE_MAP()


// CDirListDialog 消息处理程序


BOOL CDirListDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER32(&ntHead->OptionalHeader);
	// 初始化向量和窗口句柄指针
	vector<IMAGE_DATA_DIRECTORY*>IDDVt;
	CWnd* pWnd = NULL;
	// 将目录表push进vector
	IMAGE_DATA_DIRECTORY* dirList = (IMAGE_DATA_DIRECTORY*)((PDWORD)&optHead->NumberOfRvaAndSizes + 1);
	for (unsigned int i = 0; i < optHead->NumberOfRvaAndSizes; i++) {
		IDDVt.push_back(dirList++);
	}
	// 循环给窗口赋值
	for (unsigned int i = 0; i < IDDVt.size(); i++) {
		CString strRVA, strSize;
		strRVA.Format(_T("%.8X"), IDDVt[i]->VirtualAddress);
		strSize.Format(_T("%.8X"), IDDVt[i]->Size);
		// RVA
		pWnd = GetDlgItem(IDC_EDIT1 + i);
		pWnd->SetWindowText(strRVA);
		// Size
		pWnd = GetDlgItem(IDC_EDIT1 + i + 16);
		pWnd->SetWindowText(strSize);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 导出表信息
void CDirListDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (strExportSize == _T("0")) {
		MessageBox(_T("莫得导出表"), _T(">_<"), 0);
	}
	else {
		CExportTableDialog* objEd = new CExportTableDialog;
		objEd->DoModal();
	}
}

// 导入表信息
void CDirListDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CImportTableDialog* objId = new CImportTableDialog;
	objId->DoModal();
}

// 资源表信息
void CDirListDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CResTableDialog* objRd = new CResTableDialog;
	objRd->DoModal();
}

// 重定位表信息
void CDirListDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CRelocDialog* objRld = new CRelocDialog;
	objRld->DoModal();
}

// TLS表信息
void CDirListDialog::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CTLSTableDialog* objTd = new CTLSTableDialog;
	objTd->DoModal();
}

// 延迟载入表
void CDirListDialog::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CDelayLoadTableDialog* objDd = new CDelayLoadTableDialog;
	objDd->DoModal();
}
