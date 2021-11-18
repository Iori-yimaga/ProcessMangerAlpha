#pragma once


// CBlacklistDialog 对话框

class CBlacklistDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBlacklistDialog)

public:
	CBlacklistDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBlacklistDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BlacklistDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 黑名单行数
	int blackListCount = 0;
	// 黑名单列表对象
	CListCtrl objBlacklist;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	// 进程列表对象
	CListCtrl objProcList;
	void BLProcess();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
};
