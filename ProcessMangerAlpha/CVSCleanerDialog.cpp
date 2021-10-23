// CVSCleanerDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CVSCleanerDialog.h"
#include "afxdialogex.h"


// CVSCleanerDialog 对话框

IMPLEMENT_DYNAMIC(CVSCleanerDialog, CDialogEx)

CVSCleanerDialog::CVSCleanerDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VSCLEANERDIALOG, pParent)
	, strPostfix(_T(""))
	, strProjPath(_T(""))
{

}

CVSCleanerDialog::~CVSCleanerDialog()
{
}

void CVSCleanerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, strPostfix);
	DDX_Text(pDX, IDC_EDIT1, strProjPath);
	DDX_Control(pDX, IDC_LIST1, objDelOptList);
}


BEGIN_MESSAGE_MAP(CVSCleanerDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVSCleanerDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVSCleanerDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CVSCleanerDialog::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &CVSCleanerDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CVSCleanerDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CVSCleanerDialog::OnBnClickedButton4)
END_MESSAGE_MAP()


// CVSCleanerDialog 消息处理程序

// 选择文件夹
void CVSCleanerDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog* pathDialog = new CFolderPickerDialog();
	pathDialog->DoModal();
	strProjPath = pathDialog->GetFolderPath();
	UpdateData(FALSE);
}


BOOL CVSCleanerDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 默认后缀
	strDefaultPostfix = _T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\");
	strPostfix = strDefaultPostfix;
	UpdateData(FALSE);
	// 添加列
	// 样式
	objDelOptList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	objDelOptList.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, 150);
	objDelOptList.InsertColumn(1, _T("文件路径"), LVCFMT_CENTER, 150);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 恢复默认按钮
void CVSCleanerDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	strPostfix = strDefaultPostfix;
	UpdateData(FALSE);
}

// 读取后缀
void CVSCleanerDialog::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	postfixVt.clear();
	int index = strPostfix.Find(_T("\\"));
	do {
		if (index == -1) break;
		CString tmpStr = strPostfix.Left(index);
		postfixVt.push_back(tmpStr);
		strPostfix.Delete(0, index + 1);
		index = strPostfix.Find(_T("\\"));
	} while (index != -1);
}

// 扫描指定后缀的文件
void CVSCleanerDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	enumFilesPostfix(strProjPath);
}

// 遍历路径下指定后缀的所有文件
void CVSCleanerDialog::enumFilesPostfix(CString szPath) {
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
				enumFilesPostfix(szFullPath);
			}
			// 否则就是文件
			else {
				// 文件名
				CString fileName;
				fileName.Format(_T("%s"), fd.cFileName);
				// 遍历后缀向量，查找该文件是否符合
				for (unsigned int j = 0; j < postfixVt.size(); j++) {
					if (fileName.Find(postfixVt[j]) != -1) {
						objDelOptList.InsertItem(i, _T(""));
						objDelOptList.SetItemText(i, 0, fileName);
						CString tmpFilePath = szPath;
						tmpFilePath += (_T("\\") + fileName);
						objDelOptList.SetItemText(i, 1, tmpFilePath);
						i++;
						break;
					}
				}
			}
		} while (FindNextFile(hListFile, &fd));
	}
	FindClose(hListFile);
}

// 撤了撤了
void CVSCleanerDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}

// 确认删除
void CVSCleanerDialog::OnBnClickedButton4() {
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	for (int i = objDelOptList.GetItemCount(); i > 0; i--) {
		// 获得是否被选择（整行） 是否被checkbox选择
		if (objDelOptList.GetItemState(i - 1, LVIS_SELECTED) == LVIS_SELECTED || objDelOptList.GetCheck(i - 1)) {
			//str.Format(_T("第%d行被选中选中"), i);
			//获得要删除的文件名和路径
			//MessageBox(str);
			CString path = objDelOptList.GetItemText(i - 1, 1);
			DeleteFile(path);
			objDelOptList.DeleteItem(i - 1);
		}
	}
}
