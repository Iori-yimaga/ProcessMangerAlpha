#pragma once


// CDelayLoadTableDialog 对话框

class CDelayLoadTableDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDelayLoadTableDialog)

public:
	CDelayLoadTableDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDelayLoadTableDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DelayLoadTableDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void getDelayLoadTableInfo();
	CString strDllNameRVA;
	CString strModuleHandleRVA;
	CString strIATRVA;
	CString strINTRVA;
	CString strBIATRVA;
	CString strUITRVA;
};
