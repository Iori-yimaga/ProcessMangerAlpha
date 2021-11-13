#pragma once


// CTLSTableDialog 对话框

class CTLSTableDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTLSTableDialog)

public:
	CTLSTableDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTLSTableDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLSTableDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void getTLSTableInfo();
	CString strBeginVA;
	CString strEndVA;
	CString strIndexVA;
	CString strCallBackVA;
	CString strZero;
	CString strCharact;
};
