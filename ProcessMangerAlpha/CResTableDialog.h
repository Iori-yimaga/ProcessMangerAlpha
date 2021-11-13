#pragma once


// CResTableDialog 对话框

class CResTableDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResTableDialog)

public:
	CResTableDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CResTableDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ResTableDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl objResTree;
//	CString strClassSec;
//	CString strIDSec;
//	CEdit strClassFirst;
	CString strIDFirst;
	CString strClassSec;
//	CEdit strIDSec;
//	CEdit strRVA;
	CString strFOA;
	CString strIDSec;
	CString strRVA;
	CString strSize;
	void getResTableUnfo();
	CString strClassFirst;
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	void getDataEntryInfo(HTREEITEM hTree);
};
