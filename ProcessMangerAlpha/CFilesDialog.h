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

//// 时间转换函数
//CString fileTime2Time() {
//	WIN32_FIND_DATA wfda = {};
//	FILETIME ft;
//	SYSTEMTIME st;
//	CString fileTime;
//	FileTimeToLocalFileTime(&wfda.ftCreationTime, &ft);
//	FileTimeToSystemTime(&ft, &st);
//	fileTime.Format(_T("%dnian %d...."), ...);
//	return fileTime;
//}