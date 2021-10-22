// CWindowsDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CWindowsDialog.h"
#include "afxdialogex.h"


// CWindowsDialog 对话框

IMPLEMENT_DYNAMIC(CWindowsDialog, CDialogEx)

CWindowsDialog::CWindowsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WindowsDialog, pParent)
{

}

CWindowsDialog::~CWindowsDialog()
{
}

void CWindowsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, objWindowsList);
}


BEGIN_MESSAGE_MAP(CWindowsDialog, CDialogEx)

END_MESSAGE_MAP()

// CWindowsDialog 消息处理程序

BOOL CWindowsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 添加列
	objWindowsList.InsertColumn(0, _T("窗口名"), LVCFMT_CENTER, 400);
	// 枚举所有窗口
	EnumWindows(EnumWinProc, (LPARAM)&objWindowsList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 枚举窗口
BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam) {
	CListCtrl* objWinList = (CListCtrl*)lParam;
	// 根据窗口句柄获取窗口名
	TCHAR buff[200];
	::GetWindowText(hWnd, buff, 200);
	// 判断窗口是否被隐藏,是否有窗口标题
	int i = 0;
	if (::IsWindowVisible(hWnd) && _tcslen(buff) != 0) {
		objWinList->InsertItem(i, _T(""));
		objWinList->SetItemText(i, 0, buff);
		i++;
	}
	// 样式
	objWinList->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	return TRUE;
}