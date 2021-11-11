#pragma once


// CExportTableDialog 对话框

class CExportTableDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CExportTableDialog)

public:
	CExportTableDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExportTableDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ExportTableDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objExportTab;
	CString strFOA;
	CString strFuncNameOrdinalAdd;
	CString strCharact;
	CString strBase;
	CString strNameOffset;
	CString strName;
	CString strNumberOfFunc;
	CString strNumberOfFuncName;
	CString strFuncAdd;
	CString strFuncNameAdd;
	virtual BOOL OnInitDialog();
	void getExportTableInfo();
};
