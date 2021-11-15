#pragma once


// CStartUpDialog 对话框

class CStartUpDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CStartUpDialog)

public:
	CStartUpDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStartUpDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StartUpDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl objStartUpList;
	void getStartUpList();
	void OnCommandRangePMenu(UINT nId);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	void doDelete(CString BootName);
};
