// CByPathKillingDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CByPathKillingDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include "EnDeCrypt.h"
#include <vector>
using std::vector;
vector<CString>localbyVirusVt;

// 扫描的线程
HANDLE g_hByThread;
// CByPathKillingDialog 对话框

IMPLEMENT_DYNAMIC(CByPathKillingDialog, CDialogEx)

CByPathKillingDialog::CByPathKillingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ByPathKillingDialog, pParent)
{

}

CByPathKillingDialog::~CByPathKillingDialog()
{
}

void CByPathKillingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objByVirusList);
	//DDX_Control(pDX, IDC_STATIC1, objStaticText);
}


BEGIN_MESSAGE_MAP(CByPathKillingDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CByPathKillingDialog::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CByPathKillingDialog 消息处理程序


BOOL CByPathKillingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 将本地病毒库文件读取到内存中
	FILE* pFile;
	WCHAR localMD5[100] = { 0 };
	fopen_s(&pFile, "C:\\Users\\Yagami\\Desktop\\TheVirus.txt", "r");
	while (!feof(pFile)) {
		fscanf_s(pFile, "%S\n", localMD5, 100);
		localbyVirusVt.push_back(localMD5);
	}
	// 初始化表
	objByVirusList.InsertColumn(0, _T("文件路径"), LVCFMT_CENTER, 200);
	objByVirusList.InsertColumn(1, _T("MD5"), LVCFMT_CENTER, 200);
	// 样式
	objByVirusList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	g_hByThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)byKillingFilesProc, this, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 找文件线程处理函数
DWORD WINAPI byKillingFilesProc(LPARAM lParam)
{
	// TODO: 在此处添加实现代码.
	CByPathKillingDialog* pthis = (CByPathKillingDialog*)lParam;
	pthis->enumFilesByDir(pthis->strDirPath);
	return 0;
}

// 自定义路径查杀
void CByPathKillingDialog::enumFilesByDir(CString filePath)
{
	// TODO: 在此处添加实现代码.
	// 遍历文件夹时使用的路径
	CString szFilePath = filePath + _T("\\*.*");
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
				szFullPath += (filePath + _T("\\") + fd.cFileName);
				enumFilesByDir(szFullPath);
			}
			// 否则就是文件
			else {
				CString szFullPath;
				szFullPath += (filePath + _T("\\") + fd.cFileName);
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
					strtemp.Format(_T("%02x"), fileMD5[i]);
					strMD5.Append(strtemp);
				}
				// 和本地病毒库循环比较，匹配到了就是病毒，插入列表
				for (unsigned int Index = 0; Index < localbyVirusVt.size(); Index++) {
					if (strMD5 == localbyVirusVt[Index]) {
						objByVirusList.InsertItem(Index, _T(""));
						objByVirusList.SetItemText(Index, 0, szFullPath);
						objByVirusList.SetItemText(Index, 1, strMD5);
					}
				}
				delete fileBuff;
				CloseHandle(hFile);
			}
		} while (FindNextFile(hListFile, &fd));
	}
	FindClose(hListFile);
}

// 退出线程
void CByPathKillingDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TerminateThread(g_hByThread, -1);
	CDialogEx::OnClose();
}

// 删除文件
void CByPathKillingDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	for (int i = objByVirusList.GetItemCount(); i > 0; i--) {
		// 获得是否被选择（整行） 是否被checkbox选择
		if (objByVirusList.GetItemState(i - 1, LVIS_SELECTED) == LVIS_SELECTED || objByVirusList.GetCheck(i - 1)) {
			CString path = objByVirusList.GetItemText(i - 1, 0);
			//MessageBox(path, _T("Tips"));
			DeleteFile(path);
			objByVirusList.DeleteItem(i - 1);
		}
	}
}

// 显示文件路径
//void CByPathKillingDialog::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (nIDEvent == REFERSHPATH_TIMER){
//		CString nTemp;
//		nTemp.Format(_T("文件数量：%d 正在扫描："), m_ScaningNum);
//		objStaticText.SetWindowText(nTemp + szFullPath);
//	}
//	CDialogEx::OnTimer(nIDEvent);
//}
