// CSoftwareUninstallDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CSoftwareUninstallDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
// 软件卸载路径
CString uniPath;

// CSoftwareUninstallDialog 对话框

IMPLEMENT_DYNAMIC(CSoftwareUninstallDialog, CDialogEx)

CSoftwareUninstallDialog::CSoftwareUninstallDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SoftwareUninstallDialog, pParent)
{

}

CSoftwareUninstallDialog::~CSoftwareUninstallDialog()
{
}

void CSoftwareUninstallDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objSUList);
}


BEGIN_MESSAGE_MAP(CSoftwareUninstallDialog, CDialogEx)
	ON_COMMAND_RANGE(ID_32798, ID_32798, &CSoftwareUninstallDialog::OnCommandRangePMenu)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSoftwareUninstallDialog::OnRclickList1)
END_MESSAGE_MAP()


// CSoftwareUninstallDialog 消息处理程序


BOOL CSoftwareUninstallDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化卸载软件表
	objSUList.InsertColumn(0, _T("软件名称"), LVCFMT_CENTER, 80);
	objSUList.InsertColumn(1, _T("软件版本号"), LVCFMT_CENTER, 100);
	objSUList.InsertColumn(2, _T("软件安装日期"), LVCFMT_CENTER, 100);
	objSUList.InsertColumn(3, _T("软件安装路径"), LVCFMT_CENTER, 100);
	objSUList.InsertColumn(4, _T("软件卸载路径"), LVCFMT_CENTER, 100);
	objSUList.InsertColumn(5, _T("软件发布厂商"), LVCFMT_CENTER, 100);
	objSUList.InsertColumn(6, _T("软件图标路径"), LVCFMT_CENTER, 100);
	objSUList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	getUniSoftInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取软件信息
void CSoftwareUninstallDialog::getUniSoftInfo()
{
	// TODO: 在此处添加实现代码.
	// 主键
	HKEY RootKey = HKEY_LOCAL_MACHINE;
	// 子键
	LPCTSTR SubKey = L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	// 接收将要打开的键的句柄
	HKEY hkResult = 0;
	// 打开一个已经存在的注册表键
	LONG lReturn = RegOpenKeyEx(
		RootKey,                                     // 根键句柄
		SubKey,                                      // 子键路径
		0,                                           // 保留 必须为0
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,      // 指定打开键句柄的权限
		&hkResult                                    // 子键句柄
	);
	if (lReturn != ERROR_SUCCESS) {
		CString errCode;
		errCode.Format(_T("%d"), GetLastError());
		MessageBox(errCode);
	}
	else {
		// 循环遍历Uninstall目录下的子键
		DWORD Index = 0;
		BYTE softName[MAX_PATH], softVer[MAX_PATH], softUniPath[MAX_PATH], softIco[MAX_PATH], softVenRel[MAX_PATH], softDate[MAX_PATH], softInsPath[MAX_PATH];
		CString strSoftName, strSoftVer, strSoftUniPath, strSoftIco, strSoftVenRel, strSoftDate, strSoftInsPath;
		while (true) {
			DWORD keyLen = 255;
			// 注册表名称
			WCHAR newKeyName[MAX_PATH] = {};
			LONG lReturn = RegEnumKeyEx(
				hkResult,                                 // 子键句柄
				Index,                                    // 子键索引，从0开始
				newKeyName,                               // 子键名称
				&keyLen,                                  // 子键内存大小
				0,                                        // 保留 必须为0
				NULL,                                     // 用于获取子键值的类型
				NULL,                                     // 用于获取子键值的数据
				NULL                                      // 上一个参数指向的内存的大小
			);
			if (lReturn != ERROR_SUCCESS) {
				break;
			}
			// 通过得到的子键名称重新组合成新的子键路径
			WCHAR strMidReg[MAX_PATH] = {};
			swprintf_s(strMidReg, _T("%s%s%s"), SubKey, _T("\\"), newKeyName);
			// 打开新的子键，获取其句柄
			HKEY hkValueKey = 0;
			RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
			// 获取键值
			// 指向缓冲区的内存大小
			DWORD nameLen = 255;
			// 获取类型
			DWORD dwType = 0;
			// 获取软件名称
  			RegQueryValueEx(hkValueKey, L"DisplayName", 0, &dwType, softName, &nameLen);
			nameLen = 255;
			dwType = 0;
			// 卸载路径
			RegQueryValueEx(hkValueKey, L"UninstallString", 0, &dwType, softUniPath, &nameLen);
			nameLen = 255;
			dwType = 0;
			// 图标路径
			RegQueryValueEx(hkValueKey, L"DisplayIcon", 0, &dwType, softIco, &nameLen);
			nameLen = 255;
			dwType = 0;
			// 版本号
			RegQueryValueEx(hkValueKey, L"DisplayVersion", 0, &dwType, softVer, &nameLen);
			nameLen = 255;
			dwType = 0;
			// 发布商
			RegQueryValueEx(hkValueKey, L"Publisher", 0, &dwType, softVenRel, &nameLen);
			nameLen = 255;
			dwType = 0;
			// 安装日期
			RegQueryValueEx(hkValueKey, L"InstallDate", 0, &dwType, softDate, &nameLen);
			nameLen = 255;
			dwType = 0;
			// 安装路径
			RegQueryValueEx(hkValueKey, L"InstallLocation", 0, &dwType, softInsPath, &nameLen);
			nameLen = 255;
			dwType = 0;
			strSoftName.Format(_T("%s"), softName);
			strSoftVer.Format(_T("%s"), softVer);
			strSoftUniPath.Format(_T("%s"), softUniPath);
			strSoftIco.Format(_T("%s"), softIco);
			strSoftVenRel.Format(_T("%s"), softVenRel);
			strSoftDate.Format(_T("%s"), softDate);
			strSoftInsPath.Format(_T("%s"), softInsPath);
			// 插入列表
			objSUList.InsertItem(Index, _T(""));
			objSUList.SetItemText(Index, 0, strSoftName);
			objSUList.SetItemText(Index, 1, strSoftVer);
			objSUList.SetItemText(Index, 2, strSoftDate);
			objSUList.SetItemText(Index, 3, strSoftInsPath);
			objSUList.SetItemText(Index, 4, strSoftUniPath);
			objSUList.SetItemText(Index, 5, strSoftVenRel);
			objSUList.SetItemText(Index, 6, strSoftIco);
			Index++;
		}
	}
}

// 软件信息列表鼠标右键事件
void CSoftwareUninstallDialog::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nRow = pNMItemActivate->iItem;
	uniPath = objSUList.GetItemText(nRow, 4);
	// 载入菜单
	CMenu objMyMenu;
	objMyMenu.LoadMenuW(IDR_MENU5);
	CMenu* pContextMenu = objMyMenu.GetSubMenu(0);
	CPoint pt = { };
	GetCursorPos(&pt);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
	*pResult = 0;
}

// 菜单点击事件
void CSoftwareUninstallDialog::OnCommandRangePMenu(UINT nId)
{
	// TODO: 在此处添加实现代码.
	switch (nId) {
	// 卸载软件
	case ID_32798: {
		doUninstall(uniPath);
		break;
	}
	default: break;
	}
}

// 卸载软件
void CSoftwareUninstallDialog::doUninstall(CString uniPath)
{
	// TODO: 在此处添加实现代码.
	ShellExecute(NULL, L"open", uniPath, NULL, NULL, SW_SHOW);
}
