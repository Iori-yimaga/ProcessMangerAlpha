#pragma once


// CFilesDialog 对话框

class CFilesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFilesDialog)

public:
	CFilesDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFilesDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FilesDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objFilesList;
	virtual BOOL OnInitDialog();
	// 文件夹路径
	CString dirPath;
	void enumFilesByDir(CString szPath);
};

DWORD WINAPI findFilesProc(LPARAM lParam);
