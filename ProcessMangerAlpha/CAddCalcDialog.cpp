// CAddCalcDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CAddCalcDialog.h"
#include "afxdialogex.h"

// 全局变量
int isActive = 2;

// CAddCalcDialog 对话框

IMPLEMENT_DYNAMIC(CAddCalcDialog, CDialogEx)

CAddCalcDialog::CAddCalcDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddCalcDialog, pParent)
	, strVA(_T(""))
	, strRVA(_T(""))
	, strFOA(_T(""))
	, strSection(_T(""))
	, strBytes(_T(""))
{

}

CAddCalcDialog::~CAddCalcDialog()
{
}

void CAddCalcDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, objVA);
	DDX_Text(pDX, IDC_EDIT1, strVA);
	DDX_Control(pDX, IDC_EDIT2, objRVA);
	DDX_Text(pDX, IDC_EDIT2, strRVA);
	DDX_Control(pDX, IDC_EDIT3, objFOA);
	DDX_Text(pDX, IDC_EDIT3, strFOA);
	DDX_Text(pDX, IDC_EDIT4, strSection);
	DDX_Text(pDX, IDC_EDIT5, strBytes);
}


BEGIN_MESSAGE_MAP(CAddCalcDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CAddCalcDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddCalcDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CAddCalcDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddCalcDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAddCalcDialog 消息处理程序


BOOL CAddCalcDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	objVA.SetReadOnly(TRUE);
	objRVA.SetReadOnly(FALSE);
	objFOA.SetReadOnly(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// RVA按钮
void CAddCalcDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	objVA.SetReadOnly(TRUE);
	objRVA.SetReadOnly(FALSE);
	objFOA.SetReadOnly(TRUE);
	isActive = 2;
}

// VA按钮
void CAddCalcDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	objVA.SetReadOnly(FALSE);
	objRVA.SetReadOnly(TRUE);
	objFOA.SetReadOnly(TRUE);
	isActive = 1;
	strVA.Format(_T(""));
	strRVA.Format(_T(""));
	strFOA.Format(_T(""));
	strSection.Format(_T(""));
	strBytes.Format(_T(""));
	UpdateData(FALSE);
}

// 偏移量按钮
void CAddCalcDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	objVA.SetReadOnly(TRUE);
	objRVA.SetReadOnly(TRUE);
	objFOA.SetReadOnly(FALSE);
	isActive = 3;
	strVA.Format(_T(""));
	strRVA.Format(_T(""));
	strFOA.Format(_T(""));
	strSection.Format(_T(""));
	strBytes.Format(_T(""));
	UpdateData(FALSE);
}

// 计算
void CAddCalcDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	dwVA = _tcstol(strVA, NULL, 16);
	dwRVA = _tcstol(strRVA, NULL, 16);
	dwFOA = _tcstol(strFOA, NULL, 16);
	CString strZero;
	strZero = _T("00");
	switch (isActive) {
		// VA
	case 1: {
		//	VA	=====>	RVA
		dwRVA = dwVA - optHead->ImageBase;
		strRVA.Format(_T("%X"), dwRVA);
		//	RVA	=====>	FOA
		dwFOA = rvaTofoa(dwRVA);
		strFOA.Format(_T("%X"), dwFOA);
		// 区段名
		strSection = sectionName;
		// 输出8个字节
		PBYTE pByte = (PBYTE)(dwFOA + (DWORD)fileBuff);
		CString aByte;
		for (int i = 0; i < 8; i++) {
			if (*pByte == 0) {
				strBytes += (strZero + _T(" "));
			}
			else {
				aByte.Format(_T("%X"), *pByte);
				strBytes += (aByte + _T(" "));
			}
			pByte++;
		}
		break;
	}
		  // RVA
	case 2: {
		// RVA	=====>	VA
		dwVA = dwRVA + optHead->ImageBase;
		strVA.Format(_T("%X"), dwVA);
		//	RVA	=====>	FOA
		dwFOA = rvaTofoa(dwRVA);
		strFOA.Format(_T("%X"), dwFOA);
		// 区段名
		strSection = sectionName;
		// 输出8个字节
		PBYTE pByte = (PBYTE)(dwFOA + (DWORD)fileBuff);
		CString aByte;
		for (int i = 0; i < 8; i++) {
			if (*pByte == 0) {
				strBytes += (strZero + _T(" "));
			}
			else {
				aByte.Format(_T("%X"), *pByte);
				strBytes += (aByte + _T(" "));
			}
			pByte++;
		}
		break;
	}
		  // FOA
	case 3: {
		//	FOA	=====>	RVA
		dwRVA = foaTorva(dwFOA);
		strRVA.Format(_T("%X"), dwRVA);
		//	RVA	=====>	VA
		dwVA = dwRVA + optHead->ImageBase;
		strVA.Format(_T("%X"), dwVA);
		// 区段名
		strSection = sectionName;
		// 输出8个字节
		PBYTE pByte = (PBYTE)(dwFOA + (DWORD)fileBuff);
		CString aByte;
		for (int i = 0; i < 8; i++) {
			if (*pByte == 0) {
				strBytes += (strZero + _T(" "));
			}
			else {
				aByte.Format(_T("%X"), *pByte);
				strBytes += (aByte + _T(" "));
			}
			pByte++;
		}
		break;
	}
	}
	UpdateData(FALSE);
}
