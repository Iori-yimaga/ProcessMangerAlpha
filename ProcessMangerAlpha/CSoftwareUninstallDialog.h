#pragma once

// CSoftwareUninstallDialog 对话框

class CSoftwareUninstallDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftwareUninstallDialog)

public:
	CSoftwareUninstallDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSoftwareUninstallDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SoftwareUninstallDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objSUList;
	virtual BOOL OnInitDialog();
	void getUniSoftInfo();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCommandRangePMenu(UINT nId);
	void doUninstall(CString uniPath);
};
