#pragma once


// CServListDialog 对话框

class CServListDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CServListDialog)

public:
	CServListDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CServListDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ServiceListDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objServList;
	virtual BOOL OnInitDialog();
	void getServiceInfo();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);

	void OnCommandRangePMenu(UINT nId);
	bool doStartService(CString serviceName);
	bool doPauseService(CString serviceName);
};
