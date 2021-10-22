#pragma once


// CWindowsDialog 对话框

class CWindowsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowsDialog)

public:
	CWindowsDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWindowsDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WindowsDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objWindowsList;
	virtual BOOL OnInitDialog();
};

// EnumWIndows的窗口回调函数
BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam);
