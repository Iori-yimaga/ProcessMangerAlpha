// CServListDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CServListDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include <winsvc.h>

// 服务名
CString g_strServiceName;
// 服务当前状态
CString g_strServiceStatus;

// CServListDialog 对话框

IMPLEMENT_DYNAMIC(CServListDialog, CDialogEx)

CServListDialog::CServListDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ServiceListDialog, pParent)
{

}

CServListDialog::~CServListDialog()
{
}

void CServListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objServList);
}


BEGIN_MESSAGE_MAP(CServListDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CServListDialog::OnRclickList1)
	// 响应ID在某一范围的工具栏或菜单按钮消息
	ON_COMMAND_RANGE(ID_32794, ID_32795, &CServListDialog::OnCommandRangePMenu)
END_MESSAGE_MAP()


// CServListDialog 消息处理程序


BOOL CServListDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化模块表
	objServList.InsertColumn(0, _T("服务名"), LVCFMT_CENTER, 80);
	objServList.InsertColumn(1, _T("服务状态"), LVCFMT_CENTER, 100);
	objServList.InsertColumn(2, _T("服务类型"), LVCFMT_CENTER, 100);
	objServList.InsertColumn(3, _T("启动类型"), LVCFMT_CENTER, 100);
	objServList.InsertColumn(4, _T("路径信息"), LVCFMT_CENTER, 100);
	objServList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	getServiceInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取服务信息
void CServListDialog::getServiceInfo()
{
	// TODO: 在此处添加实现代码.
	// 打开服务管理器 本地服务 默认数据库 权限为所有
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	// 第一次调用 得到需要的内存的大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		hSCM,                      // 服务句柄
		SC_ENUM_PROCESS_INFO,       // 返回的属性 默认的
		SERVICE_WIN32,              // 服务类型 应用程序的服务
		SERVICE_STATE_ALL,          // 服务状态 所有 
		NULL,                      // 缓冲区
		0,                         // 缓冲区大小
		&dwSize,                   // 需要的大小
		&dwServiceNum,              // 缓冲区的服务个数
		NULL,                      // 默认0
		NULL                       // 默认0
	);
	// 申请需要的内存大小
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	// 第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(
		hSCM,                      // 服务句柄
		SC_ENUM_PROCESS_INFO,       // 返回的属性 默认的
		SERVICE_WIN32,              // 服务类型 应用程序的服务
		SERVICE_STATE_ALL,          // 服务状态 所有 
		(PBYTE)pEnumService,        // 缓冲区
		dwSize,                    // 缓冲区大小
		&dwSize,                   // 需要的大小
		&dwServiceNum,              // 缓冲区的服务个数
		NULL,                      // 默认0
		NULL                       // 默认0
	);
	// 遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++) {
		// 获取基础信息
		// 服务名
		CString strServiceName = pEnumService[i].lpServiceName;
		// 服务状态
		CString strServiceStatus;
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState) {
		case SERVICE_CONTINUE_PENDING: {
			strServiceStatus = _T("继续等待");
			break;
		}
		case SERVICE_PAUSE_PENDING: {
			strServiceStatus = _T("暂停等待");
			break;
		}
		case SERVICE_PAUSED: {
			strServiceStatus = _T("已暂停");
			break;
		}
		case SERVICE_RUNNING: {
			strServiceStatus = _T("运行中");
			break;
		}
		case SERVICE_START_PENDING: {
			strServiceStatus = _T("开始等待");
			break;
		}
		case SERVICE_STOP_PENDING: {
			strServiceStatus = _T("停止等待");
			break;
		}
		case SERVICE_STOPPED: {
			strServiceStatus = _T("已停止");
			break;
		}
		default: break;
		}
		// 服务类型
		CString strServiceType;
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType) {
		case SERVICE_FILE_SYSTEM_DRIVER: {
			strServiceType = _T("文件系统驱动服务");
			break;
		}
		case SERVICE_KERNEL_DRIVER: {
			strServiceType = _T("内核驱动服务");
			break;
		}
		case SERVICE_WIN32_OWN_PROCESS: {
			strServiceType = _T("独立进程服务");
			break;
		}
		case SERVICE_WIN32_SHARE_PROCESS: {
			strServiceType = _T("共享进程服务");
			break;
		}
		default: break;
		}
		// 根据服务名获取此服务的详细信息
		// 打开服务
		SC_HANDLE hService = OpenService(
			hSCM,                             // 服务控制管理器的句柄
			pEnumService[i].lpServiceName,      // 服务名
			SERVICE_QUERY_CONFIG               // 打开权限
		);
		// 第一次调用 获取缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		// 分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		// 第二次调用 获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		// 获取启动类型
		CString strServiceStartType;
		switch (pServiceConfig->dwStartType) {
		case SERVICE_AUTO_START: {
			strServiceStartType = _T("系统自启动");
			break;
		}
		case SERVICE_BOOT_START: {
			strServiceStartType = _T("系统加载自启动");
			break;
		}
		case SERVICE_DEMAND_START: {
			strServiceStartType = _T("需求启动");
			break;
		}
		case SERVICE_DISABLED: {
			strServiceStartType = _T("禁用");
			break;
		}
		case SERVICE_SYSTEM_START: {
			strServiceStartType = _T("系统IO初始化自启动");
			break;
		}
		default:break;
		}
		// 获取路径信息
		CString strPath = pServiceConfig->lpBinaryPathName;
		// 列表插入
		objServList.InsertItem(i, _T(""));
		objServList.SetItemText(i, 0, strServiceName);
		objServList.SetItemText(i, 1, strServiceStatus);
		objServList.SetItemText(i, 2, strServiceType);
		objServList.SetItemText(i, 3, strServiceStartType);
		objServList.SetItemText(i, 4, strPath);
	}
}

// 服务列表右键点击事件
void CServListDialog::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nRow = pNMItemActivate->iItem;
	// 获取服务名
	g_strServiceName = objServList.GetItemText(nRow, 0);
	// 获取服务状态
	g_strServiceStatus = objServList.GetItemText(nRow, 1);
	// 载入菜单
	CMenu objMyMenu;
	objMyMenu.LoadMenuW(IDR_MENU4);
	CMenu* pContextMenu = objMyMenu.GetSubMenu(0);
	CPoint pt = { };
	GetCursorPos(&pt);
	if (g_strServiceStatus == _T("继续等待") || g_strServiceStatus == _T("暂停等待") || g_strServiceStatus == _T("已暂停") ||
		g_strServiceStatus == _T("开始等待") || g_strServiceStatus == _T("停止等待") || g_strServiceStatus == _T("已停止")) {
		pContextMenu->EnableMenuItem(ID_32795, MF_BYCOMMAND | MF_GRAYED);
	}
	else {
		pContextMenu->EnableMenuItem(ID_32794, MF_BYCOMMAND | MF_GRAYED);
	}
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
	*pResult = 0;
}

// 菜单点击事件
void CServListDialog::OnCommandRangePMenu(UINT nId)
{
	// TODO: 在此处添加实现代码.
	switch (nId) {
		// 开启服务
		case ID_32794: {
			doStartService(g_strServiceName);
			objServList.SetItemText(nRow, 1, _T("运行中"));
			break;
		}
		// 暂停服务
		case ID_32795: {
			doPauseService(g_strServiceName);
			objServList.SetItemText(nRow, 1, _T("已暂停"));
			break;
		}
		default: break;
	}
}

// 开启服务
bool CServListDialog::doStartService(CString serviceName)
{
	// TODO: 在此处添加实现代码.
	SC_HANDLE hSCManger = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManger != NULL) {
		SC_HANDLE hService = OpenService(hSCManger, serviceName, SERVICE_START);
		if (hService != NULL) {
			if (StartService(hService, NULL, NULL)) {
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCManger);
				return TRUE;
			}
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManger);
			return FALSE;
		}
		CloseServiceHandle(hSCManger);
		return FALSE;
	}
	else return false;
}

// 暂停服务
bool CServListDialog::doPauseService(CString serviceName)
{
	// TODO: 在此处添加实现代码.
	SC_HANDLE hSCManger = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManger != NULL) {
		SC_HANDLE hService = OpenService(hSCManger, serviceName, SERVICE_STOP);
		if (hService != NULL) {
			SERVICE_STATUS status;
			if (ControlService(hService, SERVICE_CONTROL_STOP, &status)) {
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCManger);
				return TRUE;
			}
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManger);
			return FALSE;
		}
		CloseServiceHandle(hSCManger);
		return FALSE;
	}else return false;
}
