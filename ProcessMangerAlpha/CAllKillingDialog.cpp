// CAllKillingDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CAllKillingDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include "EnDeCrypt.h"
#include <vector>
using std::vector;
vector<CString>localVirusVt;

int nIndex = 0;

// 扫描的线程
HANDLE g_hThread1;
HANDLE g_hThread2;
HANDLE g_hThread3;

// CAllKillingDialog 对话框

IMPLEMENT_DYNAMIC(CAllKillingDialog, CDialogEx)

CAllKillingDialog::CAllKillingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AllKillingDialog, pParent)
{

}

CAllKillingDialog::~CAllKillingDialog()
{
}

void CAllKillingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objVirusFileList);
}


BEGIN_MESSAGE_MAP(CAllKillingDialog, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAllKillingDialog 消息处理程序


BOOL CAllKillingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 将本地病毒库文件读取到内存中
	FILE* pFile;
	WCHAR localMD5[100] = { 0 };
	fopen_s(&pFile, "C:\\Users\\Yagami\\Desktop\\TheVirus.txt", "r");
	while (!feof(pFile)) {
		fscanf_s(pFile, "%S\n", localMD5, 100);
		localVirusVt.push_back(localMD5);
	}
	// 初始化启动项表
	objVirusFileList.InsertColumn(0, _T("文件路径"), LVCFMT_CENTER, 200);
	objVirusFileList.InsertColumn(1, _T("MD5"), LVCFMT_CENTER, 200);
	// 样式
	objVirusFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	g_hThread1 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)allKillingFilesProc1, this, 0, 0);
	g_hThread2 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)allKillingFilesProc2, this, 0, 0);
	g_hThread3 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)allKillingFilesProc3, this, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 找文件线程处理函数
DWORD WINAPI allKillingFilesProc1(LPARAM lParam)
{
	// TODO: 在此处添加实现代码.
	CAllKillingDialog* pthis = (CAllKillingDialog*)lParam;
	CString RootPath;
	RootPath = _T("C:");
	pthis->enumFilesByDir(RootPath);
	return 0;
}

// 找文件线程处理函数
DWORD WINAPI allKillingFilesProc2(LPARAM lParam)
{
	// TODO: 在此处添加实现代码.
	CAllKillingDialog* pthis = (CAllKillingDialog*)lParam;
	CString RootPath;
	RootPath = _T("D:");
	pthis->enumFilesByDir(RootPath);
	return 0;
}

// 找文件线程处理函数
DWORD WINAPI allKillingFilesProc3(LPARAM lParam)
{
	// TODO: 在此处添加实现代码.
	CAllKillingDialog* pthis = (CAllKillingDialog*)lParam;
	CString RootPath;
	RootPath = _T("F:");
	pthis->enumFilesByDir(RootPath);
	return 0;
}

// 全盘查杀
void CAllKillingDialog::enumFilesByDir(CString szPath)
{
	// TODO: 在此处添加实现代码.
	// 遍历文件夹时使用的路径
	CString szFilePath = szPath + _T("\\*.*");
	// 文件信息结构体
	WIN32_FIND_DATA fd = { 0 };
	// 首文件的句柄
	HANDLE hListFile = FindFirstFile(szFilePath, &fd);
	// 如果是一个无效句柄就弹窗警告错误码
	if (hListFile == INVALID_HANDLE_VALUE) {
		CString Err;
		Err.Format(_T("错误: %d"), GetLastError());
		MessageBox(Err, _T("警告"));
	}
	else {
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
				CString szFullPath;
				szFullPath += (szPath + _T("\\") + fd.cFileName);
				// MD5 值
				// 打开一个文件
				HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				// 获取文件大小 
				DWORD fileSize = GetFileSize(hFile, NULL);
				if (fileSize >= 1024 * 1024 * 1024 * 2) {
					continue;
				}
				// 申请空间保存文件内容, 如果读取成功就申请空间
				char* fileBuff = new char[fileSize];
				// 读取文件
				DWORD read_size = 0;
				errno_t err = ReadFile(hFile, fileBuff, fileSize, &read_size, NULL);
				unsigned char fileMD5[100] = { 0 };
				MD5Encrypt(fileBuff, fileSize, fileMD5);
				CString strMD5;
				for (int i = 0; i < 16; i++) {
					CString strtemp;
					strtemp.Format(_T("%02X"), fileMD5[i]);
					strMD5.Append(strtemp);
				}
				// 和本地病毒库循环比较，匹配到了就是病毒，插入列表
				for (unsigned int Index = 0; Index < localVirusVt.size(); Index++) {
					if (strMD5 == localVirusVt[Index]) {
						WaitForSingleObject(g_hEvent, -1);
						objVirusFileList.InsertItem(nIndex, _T(""));
						objVirusFileList.SetItemText(nIndex, 0, szFullPath);
						objVirusFileList.SetItemText(nIndex, 1, strMD5);
						nIndex++;
						SetEvent(g_hEvent);
					}
				}
				delete fileBuff;
			}
		} while (FindNextFile(hListFile, &fd));
	}
	FindClose(hListFile);
}

// 关闭窗口停止查杀
void CAllKillingDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TerminateThread(g_hThread1, -1);
	TerminateThread(g_hThread2, -1);
	TerminateThread(g_hThread3, -1);
	CDialogEx::OnClose();
}
