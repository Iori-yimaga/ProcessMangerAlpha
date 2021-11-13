#pragma once


// CImportTableDialog 对话框

class CImportTableDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTableDialog)

public:
	CImportTableDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImportTableDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImportTableDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl objModuleList;
	CListCtrl objFuncList;
	void getImportTableInfo();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	void getFuncInfo(DWORD IATRVA);
};
