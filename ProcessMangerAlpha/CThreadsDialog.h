#pragma once
#include "GlobalVals.h"
// CThreadsDialog 对话框

class CThreadsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadsDialog)

public:
	CThreadsDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CThreadsDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ThreadsDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objThreadsList;
	virtual BOOL OnInitDialog();
	void getThreadsInfoByProcID();
	afx_msg void OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCommandRangeTMenu(UINT nId);
};
