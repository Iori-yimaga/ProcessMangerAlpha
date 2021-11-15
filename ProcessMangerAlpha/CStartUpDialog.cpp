// CStartUpDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CStartUpDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"


HKEY RootKey = NULL;       // 主键
HKEY hkResult = NULL;      // 打开键的句柄
LONG lReturn = 0;          // 记录打开的句柄
DWORD Index = 0;           // 索引
DWORD KeyLen = 255;        // 键的长度
DWORD nameLen = 255;       // 名字长度
DWORD dwType = 0;          // 类型
CString buff;              //缓冲区大小
LPCTSTR SubKey[2];         // 子键

CString BootName;          // 启动项的名字
// CStartUpDialog 对话框

IMPLEMENT_DYNAMIC(CStartUpDialog, CDialogEx)

CStartUpDialog::CStartUpDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_StartUpDialog, pParent)
{

}

CStartUpDialog::~CStartUpDialog()
{
}

void CStartUpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objStartUpList);
}


BEGIN_MESSAGE_MAP(CStartUpDialog, CDialogEx)
	ON_COMMAND_RANGE(ID_32799, ID_32799, &CStartUpDialog::OnCommandRangePMenu)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CStartUpDialog::OnRclickList1)
END_MESSAGE_MAP()


// CStartUpDialog 消息处理程序


BOOL CStartUpDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化启动项表
	objStartUpList.InsertColumn(0, _T("启动项名"), LVCFMT_CENTER, 80);
	objStartUpList.InsertColumn(1, _T("启动项序号"), LVCFMT_CENTER, 100);
	objStartUpList.InsertColumn(2, _T("删除时的路径"), LVCFMT_CENTER, 100);
	objStartUpList.InsertColumn(3, _T("主键信息"), LVCFMT_CENTER, 100);
	objStartUpList.InsertColumn(4, _T("启动位置"), LVCFMT_CENTER, 100);
	objStartUpList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	getStartUpList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取启动项列表信息
void CStartUpDialog::getStartUpList()
{
	// TODO: 在此处添加实现代码.
	WCHAR BootName[MAX_PATH];      // 启动项名
	WCHAR BootOrder[MAX_PATH];     // 启动项序号
	WCHAR BootOffset[MAX_PATH];    // 删除时的路径 
	WCHAR BootHKEY[50];            // 主键信息
	WCHAR BootAllOffset[MAX_PATH];  // 启动位置

	SubKey[0] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	SubKey[1] = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";

	for (DWORD i = 0; i < 2; i++) {
		if (i == 0) {
			RootKey = HKEY_CURRENT_USER;
		}
		if (i == 1) {
			RootKey = HKEY_LOCAL_MACHINE;
		}
		// 打开子键
		lReturn = RegOpenKeyEx(RootKey, SubKey[i], 0, KEY_ALL_ACCESS, &hkResult);
		if (lReturn == ERROR_SUCCESS) {
			// 打开成功
			while (ERROR_NO_MORE_ITEMS != RegEnumValue(
				hkResult, Index, BootName, &KeyLen, 0, NULL, NULL, NULL
			)) {
				// 获取键值中的名称
				RegQueryValueEx(hkResult, BootName, 0, &dwType, (LPBYTE)BootOrder, &nameLen);
				// 保存位置
				if (i == 0) {
					_tcscpy_s(BootHKEY, _T("HKEY_CURRENT_USER"));
				}
				else {
					_tcscpy_s(BootHKEY, _T("HKEY_LOCAL_MACHINE"));
				}
				// 保存子键路径
				_tcscpy_s(BootOffset, SubKey[i]);
				// 保存启动项的全路径
				buff.Empty();
				buff = buff + BootHKEY + _T("\\") + BootOffset;
				_tcscpy_s(BootAllOffset, buff);

				objStartUpList.InsertItem(i, L"");
				objStartUpList.SetItemText(i, 0, BootName);
				objStartUpList.SetItemText(i, 1, BootOrder);
				objStartUpList.SetItemText(i, 2, BootOffset);
				objStartUpList.SetItemText(i, 3, BootHKEY);
				objStartUpList.SetItemText(i, 4, BootAllOffset);
				Index++;
				nameLen = 255;
				KeyLen = 255;
			}
			Index = 0;
			RegCloseKey(hkResult);
		}
	}
}

// 启动项右键点击事件
void CStartUpDialog::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nRow = pNMItemActivate->iItem;
	BootName = objStartUpList.GetItemText(nRow, 0);
	// 载入菜单
	CMenu objMyMenu;
	objMyMenu.LoadMenuW(IDR_MENU6);
	CMenu* pContextMenu = objMyMenu.GetSubMenu(0);
	CPoint pt = { };
	GetCursorPos(&pt);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
	*pResult = 0;
}

// 菜单点击事件
void CStartUpDialog::OnCommandRangePMenu(UINT nId)
{
	// TODO: 在此处添加实现代码.
	switch (nId) {
		// 卸载软件
	case ID_32799: {
		doDelete(BootName);
		break;
	}
	default: break;
	}
}

// 删除启动项
void CStartUpDialog::doDelete(CString BootName)
{
	// TODO: 在此处添加实现代码.
	for (DWORD i = 0; i < 2; i++) {
		if (i == 0) {
			RootKey = HKEY_CURRENT_USER;
		}
		if (i == 1) {
			RootKey = HKEY_LOCAL_MACHINE;
		}
		// 打开要删除键值所属的子键
		HKEY hSubKey = 0;
		RegOpenKeyEx(RootKey, SubKey[i], 0, KEY_SET_VALUE, &hSubKey);
		// 删除指定键值
		RegDeleteValue(hSubKey, BootName);
		MessageBox(L"已经移除~", L"Oops");
		// 刷新一下列表（重新获取）
		objStartUpList.DeleteAllItems();
		getStartUpList();
	}
}
