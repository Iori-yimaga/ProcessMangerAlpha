// CBlacklistDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CBlacklistDialog.h"
#include "afxdialogex.h"
#include "EnDeCrypt.h"
#include <TlHelp32.h>

int i = 0;
// CBlacklistDialog 对话框

IMPLEMENT_DYNAMIC(CBlacklistDialog, CDialogEx)

CBlacklistDialog::CBlacklistDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BlacklistDialog, pParent)
{

}

CBlacklistDialog::~CBlacklistDialog()
{
}

void CBlacklistDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objBlacklist);
	DDX_Control(pDX, IDC_LIST2, objProcList);
}


BEGIN_MESSAGE_MAP(CBlacklistDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBlacklistDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBlacklistDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBlacklistDialog::OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CBlacklistDialog::OnDblclkList2)
END_MESSAGE_MAP()


// CBlacklistDialog 消息处理程序


BOOL CBlacklistDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	// 初始化表
	objProcList.InsertColumn(0, _T("PID"), LVCFMT_CENTER, 100);
	objProcList.InsertColumn(1, _T("进程名"), LVCFMT_CENTER, 100);
	// 样式
	objProcList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	BLProcess();
	// 初始化表
	objBlacklist.InsertColumn(0, _T("PID"), LVCFMT_CENTER, 100);
	objBlacklist.InsertColumn(1, _T("进程名"), LVCFMT_CENTER, 100);
	// 样式
	objBlacklist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 添加黑名单
void CBlacklistDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码


}

// 移除黑名单
void CBlacklistDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	for (int i = objBlacklist.GetItemCount(); i > 0; i--) {
		// 获得是否被选择（整行） 是否被checkbox选择
		if (objBlacklist.GetItemState(i - 1, LVIS_SELECTED) == LVIS_SELECTED || objBlacklist.GetCheck(i - 1)) {
			objBlacklist.DeleteItem(i - 1);
			blackListCount--;
		}
	}
}

// 查杀
void CBlacklistDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 创建快照
	PROCESSENTRY32 stcPe32 = { 0 };
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	// 1.创建快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,      // 创建进程快照，还可以创建线程、模块等快照
		0);
	// 2.获取第一个进程的信息
	if (Process32First(hProcessSnap, &stcPe32)) {
		// 3. 循环获取所有的进程的信息
		do {
			for (int j = 0; j < blackListCount; j++) {
				DWORD dwPID = _tcstol(objBlacklist.GetItemText(j, 0), NULL, 10);
				if (stcPe32.th32ProcessID == dwPID) {
					MessageBox(objBlacklist.GetItemText(j, 0));
					HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
					TerminateProcess(hProc, 0);
				}
			}
		} while (Process32Next(hProcessSnap, &stcPe32));
	}
	// 关闭快照
	CloseHandle(hProcessSnap);
}

// 遍历进程
void CBlacklistDialog::BLProcess()
{
	// TODO: 在此处添加实现代码.
	// 创建快照
	PROCESSENTRY32 stcPe32 = { 0 };
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	// 1.创建快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,      // 创建进程快照，还可以创建线程、模块等快照
		0);
	// 2.获取第一个进程的信息
	int i = 0;
	if (Process32First(hProcessSnap, &stcPe32)) {
		// 3. 循环获取所有的进程的信息
		// 插入行
		do {
			CString procAttr;
			objProcList.InsertItem(i, _T(""));
			procAttr.Format(_T("%d"), stcPe32.th32ProcessID);
			objProcList.SetItemText(i, 0, procAttr);
			objProcList.SetItemText(i, 1, stcPe32.szExeFile);
			i++;
		} while (Process32Next(hProcessSnap, &stcPe32));
	}
	// 关闭快照
	CloseHandle(hProcessSnap);
}

// 双击添加黑名单
void CBlacklistDialog::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nRow = pNMItemActivate->iItem;
	CString strPID = objProcList.GetItemText(nRow, 0);
	CString strProcName = objProcList.GetItemText(nRow, 1);
	objBlacklist.InsertItem(blackListCount, _T(""));
	objBlacklist.SetItemText(blackListCount, 0, strPID);
	objBlacklist.SetItemText(blackListCount, 1, strProcName);
	blackListCount++;
	*pResult = 0;
}
