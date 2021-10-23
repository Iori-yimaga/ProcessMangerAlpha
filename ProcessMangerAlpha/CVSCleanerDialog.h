#pragma once
#include <vector>
using std::vector;

// CVSCleanerDialog 对话框

class CVSCleanerDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVSCleanerDialog)

public:
	CVSCleanerDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVSCleanerDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VSCLEANERDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	vector<CString> postfixVt;
	CString strDefaultPostfix;
	CString strPostfix;
	CString strProjPath;
	CListCtrl objDelOptList;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton3();
	void enumFilesPostfix(CString szPath);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
};
