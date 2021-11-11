#pragma once
#include "GlobalVals.h"

// CAddCalcDialog 对话框

class CAddCalcDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddCalcDialog)

public:
	CAddCalcDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddCalcDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddCalcDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit objVA;
	CString strVA;
	CEdit objRVA;
	CString strRVA;
	CEdit objFOA;
	CString strFOA;
	CString strSection;
	CString strBytes;
	DWORD dwVA, dwRVA, dwFOA;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
};
