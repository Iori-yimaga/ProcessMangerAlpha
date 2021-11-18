#pragma once

// CAllKillingDialog 对话框

class CAllKillingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAllKillingDialog)

public:
	CAllKillingDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAllKillingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AllKillingDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HANDLE g_hEvent = NULL;
	virtual BOOL OnInitDialog();
	CListCtrl objVirusFileList;
	void enumFilesByDir(CString szPath);
	afx_msg void OnClose();
};

DWORD WINAPI allKillingFilesProc1(LPARAM lParam);
DWORD WINAPI allKillingFilesProc2(LPARAM lParam);
DWORD WINAPI allKillingFilesProc3(LPARAM lParam);