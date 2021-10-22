// CFilesDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CFilesDialog.h"
#include "afxdialogex.h"


// CFilesDialog 对话框

IMPLEMENT_DYNAMIC(CFilesDialog, CDialogEx)

CFilesDialog::CFilesDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FilesDialog, pParent)
{

}

CFilesDialog::~CFilesDialog()
{
}

void CFilesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, objFilesList);
}


BEGIN_MESSAGE_MAP(CFilesDialog, CDialogEx)
END_MESSAGE_MAP()


// CFilesDialog 消息处理程序


BOOL CFilesDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	objFilesList.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, 200);
	objFilesList.InsertColumn(1, _T("创建时间"), LVCFMT_CENTER, 200);
	objFilesList.InsertColumn(2, _T("修改时间"), LVCFMT_CENTER, 200);
	objFilesList.InsertColumn(3, _T("文件大小"), LVCFMT_CENTER, 200);
	// 样式
	objFilesList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	enumFilesByDir(dirPath);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 遍历路径下的所有文件
void CFilesDialog::enumFilesByDir(CString szPath)
{
	// TODO: 在此处添加实现代码.
	// 遍历文件夹时使用的路径
	CString szFilePath = szPath + _T("\\*.*");
	// 文件信息结构体
	WIN32_FIND_DATA fd = { 0 };
	// 获取文件时间
	FILETIME ft;
	SYSTEMTIME st;
	CString fileTime;
	CString fileSize;
	// 首文件的句柄
	HANDLE hListFile = FindFirstFile(szFilePath, &fd);
	// 如果是一个无效句柄就弹窗警告错误码
	if (hListFile == INVALID_HANDLE_VALUE) {
		CString Err;
		Err.Format(_T("错误: %d"), GetLastError());
		MessageBox(Err, _T("警告"));
	}
	else {
		int i = 0;
		do {
			// 如果是一个本级或上级，就跳过
			if (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0) {
				continue;
			}
			// 如果是隐藏文件就跳过
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
				continue;
			}
			// 如果是文件夹，就拼上路径去递归遍历
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				CString szFullPath;
				szFullPath += (szPath + _T("\\") + fd.cFileName);
				enumFilesByDir(szFullPath);
			}
			// 否则就是文件
			else {
				objFilesList.InsertItem(i, _T(""));
				// 文件名
				CString fileName;
				fileName.Format(_T("%s"), fd.cFileName);
				objFilesList.SetItemText(i, 0, fileName);
				// 创建时间
				FileTimeToLocalFileTime(&fd.ftCreationTime, &ft);
				FileTimeToSystemTime(&ft, &st);
				fileTime.Format(_T("%d-%d-%d  %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				objFilesList.SetItemText(i, 1, fileTime);
				// 修改时间
				FileTimeToLocalFileTime(&fd.ftLastWriteTime, &ft);
				FileTimeToSystemTime(&ft, &st);
				fileTime.Format(_T("%d-%d-%d  %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				objFilesList.SetItemText(i, 2, fileTime);
				// 文件大小
				ULONGLONG FileSize;
				FileSize = fd.nFileSizeHigh;
				FileSize <<= 32;
				FileSize += fd.nFileSizeLow;
				fileSize.Format(_T("%d 字节"), FileSize);
				objFilesList.SetItemText(i, 3, fileSize);
				i++;
			}
		} while (FindNextFile(hListFile, &fd));
	}
	FindClose(hListFile);
}

