#pragma once


// CByPathKillingDialog 对话框

class CByPathKillingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CByPathKillingDialog)

public:
	CByPathKillingDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CByPathKillingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ByPathKillingDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件夹路径
	CString strDirPath;
	virtual BOOL OnInitDialog();
	// 自定义路径杀毒列表对象
	CListCtrl objByVirusList;
	void enumFilesByDir(CString filePath);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	CStatic objStaticText;
};

DWORD WINAPI byKillingFilesProc(LPARAM lParam);