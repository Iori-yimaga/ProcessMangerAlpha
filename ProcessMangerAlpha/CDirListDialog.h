#pragma once
#include "CExportTableDialog.h"
#include "CImportTableDialog.h"
#include "CResTableDialog.h"
#include "CRelocDialog.h"
#include "CTLSTableDialog.h"
#include "CDelayLoadTableDialog.h"

// CDirListDialog 对话框

class CDirListDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDirListDialog)

public:
	CDirListDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDirListDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DirListDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString strExportRVA;
	CString strTLSRVA;
	CString strLoadConfigRVA;
	CString strBoundImportRVA;
	CString strIATRVA;
	CString strDelayImportRVA;
	CString strCOMRVA;
	CString strBLRVA;
	CString strExportSize;
	CString strImportSize;
	CString strResourceSize;
	CString strImportRVA;
	CString strExceptionSize;
	CString strSecuritySize;
	CString strRelocSize;
	CString strDebugSize;
	CString strCopyrightSize;
	CString strGlobalptrSize;
	CString strTLSSize;
	CString strLoadConfigSize;
	CString strBoundImportSize;
	CString strIATSize;
	CString strResouceRVA;
	CString strDelayImportSize;
	CString strCOMSize;
	CString strBLSize;
	CString strExceptionRVA;
	CString strSecurityRVA;
	CString strRelocRVA;
	CString strDebugRVA;
	CString strCopyrightRVA;
	CString strGlobalptrRVA;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};
