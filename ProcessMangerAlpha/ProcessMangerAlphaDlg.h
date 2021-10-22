
// ProcessMangerAlphaDlg.h: 头文件
//

#pragma once
#include <TlHelp32.h>
#include "CThreadsDialog.h"
#include "CModulesDialog.h"
#include "CHeapDialog.h"
#include "CWindowsDialog.h"
#include "GlobalVals.h"

// CProcessMangerAlphaDlg 对话框
class CProcessMangerAlphaDlg : public CDialogEx
{
// 构造
public:
	CProcessMangerAlphaDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSMANGERALPHA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 遍历进程
	void traverseProcess();
	// 列表对象
	CListCtrl objListCtrl;
	// 根据进程ID查找线程数
	CString getThreadsCountByProcID(DWORD procID);
	// 右键点击列表事件
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	// 进程菜单回调函数
	void OnCommandRangePMenu(UINT nId);
	// 主窗口菜单
	void OnCommandRangeMMenu(UINT nId);
	// 提权操作
	void upperPrivileges();
	// 响应热键消息函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatusBar objStatusBar;
	// 始终回调函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 时间相关
	_FILETIME idleTime, kernelTime, userTime;
};

// 时间转换
double FILETIME2Double(const _FILETIME& fileTime);
