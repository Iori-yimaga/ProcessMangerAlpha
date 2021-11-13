#pragma once


// CRelocDialog 对话框

class CRelocDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocDialog)

public:
	CRelocDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRelocDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RelocDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objSectionList;
	CListCtrl objBlockList;
	virtual BOOL OnInitDialog();
	void getRelocTableInfo();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	void getBlocksInfo(PIMAGE_BASE_RELOCATION reloc);
};
