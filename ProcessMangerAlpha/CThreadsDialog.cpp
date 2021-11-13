// CThreadsDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CThreadsDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// 声明一下全局变量
CString g_ThreadID;

// CThreadsDialog 对话框

IMPLEMENT_DYNAMIC(CThreadsDialog, CDialogEx)

CThreadsDialog::CThreadsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ThreadsDialog, pParent)
{

}

CThreadsDialog::~CThreadsDialog()
{
}

void CThreadsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, objThreadsList);
}


BEGIN_MESSAGE_MAP(CThreadsDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CThreadsDialog::OnRclickList2)
	// 响应ID在某一范围的工具栏或菜单按钮消息
	ON_COMMAND_RANGE(ID_32776, ID_32778, &CThreadsDialog::OnCommandRangeTMenu)
END_MESSAGE_MAP()


// CThreadsDialog 消息处理程序


BOOL CThreadsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 添加列
	objThreadsList.InsertColumn(0, _T("线程ID"), LVCFMT_CENTER, 200);
	objThreadsList.InsertColumn(1, _T("线程优先级"), LVCFMT_CENTER, 200);
	getThreadsInfoByProcID();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 根据进程ID查看线程信息
void CThreadsDialog::getThreadsInfoByProcID()
{
	// TODO: 在此处添加实现代码.
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 stcTe32 = { sizeof(stcTe32) };
	// 1.创建模块快照
	hThreadSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPTHREAD,      // 创建线程快照
		0                       // 进程ID
	);
	// 2.获取第一个线程的信息
	int i = 0;
	// CString ==> DWORD
	DWORD dwPid = _tcstol(g_ProcID, NULL, 10);
	if (Thread32First(hThreadSnap, &stcTe32)) {
		// 3. 循环获取所有的线程的信息
		do {
			if (stcTe32.th32OwnerProcessID == dwPid) {
				objThreadsList.InsertItem(i, _T(""));
				CString threadID;
				threadID.Format(_T("%d"), stcTe32.th32ThreadID);
				objThreadsList.SetItemText(i, 0, threadID);
				CString threadPri;
				threadPri.Format(_T("%d"), stcTe32.tpBasePri);
				objThreadsList.SetItemText(i, 1, threadPri);
				i++;
			}
		} while (Thread32Next(hThreadSnap, &stcTe32));
	}
	// 样式
	objThreadsList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 关闭快照
	CloseHandle(hThreadSnap);
}

// 线程列表点击事件
void CThreadsDialog::OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nRow = pNMItemActivate->iItem;
	// int nColume = pNMItemActivate->iSubItem;
	g_ThreadID = objThreadsList.GetItemText(nRow, 0);
	// 载入菜单
	CMenu objMyMenu;
	objMyMenu.LoadMenuW(IDR_MENU2);
	CMenu* pContextMenu = objMyMenu.GetSubMenu(0);
	CPoint pt = { };
	GetCursorPos(&pt);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
	*pResult = 0;
}

// 线程操作菜单
void CThreadsDialog::OnCommandRangeTMenu(UINT nId) {
	// CString ==> DWORD
	DWORD dwPid = _tcstol(g_ThreadID, NULL, 10);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, NULL, dwPid);
	switch (nId) {
		// 挂起线程
		case ID_32776:{
			SuspendThread(hThread);
			break;
		}
		// 恢复线程
		case ID_32777:{
			ResumeThread(hThread);
			break;
		}
		// 结束线程
		case ID_32778:{
			TerminateThread(hThread, 0);
			break;
		}
		default:
			break;
	}
}