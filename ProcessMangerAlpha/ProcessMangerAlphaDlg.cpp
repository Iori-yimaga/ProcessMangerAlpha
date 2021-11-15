
// ProcessMangerAlphaDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ProcessMangerAlpha.h"
#include "ProcessMangerAlphaDlg.h"
#include "afxdialogex.h"
#include <powrprof.h>
#pragma comment(lib,"Powrprof.lib")
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 声明一下全局变量
CString g_ProcID;
int nRow;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClose();
//	void getTLSTableInfo();
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

// 转换时间
double FILETIME2Double(const _FILETIME& fileTime) {
	return double(fileTime.dwHighDateTime * 4.294967296e9) +
		double(fileTime.dwLowDateTime);
}

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CProcessMangerAlphaDlg 对话框



CProcessMangerAlphaDlg::CProcessMangerAlphaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESSMANGERALPHA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessMangerAlphaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objListCtrl);
}

BEGIN_MESSAGE_MAP(CProcessMangerAlphaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcessMangerAlphaDlg::OnRclickList1)
	// 响应ID在某一范围的工具栏或菜单按钮消息
	ON_COMMAND_RANGE(ID_32771, ID_32775, &CProcessMangerAlphaDlg::OnCommandRangePMenu)
	ON_COMMAND_RANGE(ID_32779, ID_32793, &CProcessMangerAlphaDlg::OnCommandRangeMMenu)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProcessMangerAlphaDlg 消息处理程序

BOOL CProcessMangerAlphaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// 绑定老板键
	// 获取当前窗口的句柄
	HWND hMainWindow = AfxGetMainWnd()->m_hWnd;
	// 注册热键
	::RegisterHotKey(
		hMainWindow,                // 窗口句柄
		0x1234,                     // ID
		MOD_CONTROL | MOD_SHIFT,    // 同时按下Ctrl+Shift
		'H'                         // H
	);
	// 添加列
	objListCtrl.InsertColumn(0, _T("PID"),    LVCFMT_CENTER, 100);
	objListCtrl.InsertColumn(1, _T("进程名"),  LVCFMT_CENTER, 150);
	objListCtrl.InsertColumn(2, _T("线程数量"), LVCFMT_CENTER, 100);
	objListCtrl.InsertColumn(3, _T("进程路径"), LVCFMT_CENTER, 200);
	// 样式
	objListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	traverseProcess();
	// 时间使用1号计时器
	SetTimer(1, 1000, NULL);
	// CPU和内存使用2号计时器
	SetTimer(2, 3000, NULL);
	// 初始化时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	// 初始化状态栏
	UINT buf[3] = { 1,2,3 };
	objStatusBar.Create(this);
	// 状态栏分为3部分
	objStatusBar.SetIndicators(buf, 3);
	// 设置每一个部分的宽度
	objStatusBar.SetPaneInfo(0, 1, 0, 180);
	objStatusBar.SetPaneInfo(1, 2, 0, 180);
	objStatusBar.SetPaneInfo(2, 3, 0, 180);
	// 设置状态栏每一部分的内容
	objStatusBar.SetPaneText(0, _T("获取系统时间中"));
	objStatusBar.SetPaneText(1, _T("获取内存信息中"));
	objStatusBar.SetPaneText(2, _T("获取CPU信息中"));
	// 重定位
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProcessMangerAlphaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessMangerAlphaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProcessMangerAlphaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 遍历进程
void CProcessMangerAlphaDlg::traverseProcess()
{
	// TODO: 在此处添加实现代码.
	// 创建快照
	PROCESSENTRY32 stcPe32 = { 0 };
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	// 1.创建快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,      // 创建进程快照，还可以创建线程、模块等快照
		0);
	// 2.获取第一个进程的信息
	int i = 0;
	if (Process32First(hProcessSnap, &stcPe32)) {
		// 3. 循环获取所有的进程的信息
		// 插入行
		do {
			CString procAttr;
			objListCtrl.InsertItem(i, _T(""));
			procAttr.Format(_T("%d"), stcPe32.th32ProcessID);
			objListCtrl.SetItemText(i, 0, procAttr);
			objListCtrl.SetItemText(i, 1, stcPe32.szExeFile);
			objListCtrl.SetItemText(i, 2, getThreadsCountByProcID(stcPe32.th32ProcessID));
			if (_tcslen(getProcFullPath(stcPe32.th32ProcessID)) != 0) {
				objListCtrl.SetItemText(i, 3, getProcFullPath(stcPe32.th32ProcessID));
			}
			else {
				objListCtrl.SetItemText(i, 3, _T("权限不够哦亲，查不到~"));
			}
			i++;
		} while (Process32Next(hProcessSnap, &stcPe32));
	}
	// 关闭快照
	CloseHandle(hProcessSnap);
}

// 根据进程ID遍历线程数量
CString CProcessMangerAlphaDlg::getThreadsCountByProcID(DWORD procID)
{
	// TODO: 在此处添加实现代码.
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 stcTe32 = { sizeof(stcTe32) };
	// 1.创建模块快照
	hThreadSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPTHREAD,      // 创建线程快照
		0                       // 进程ID
	);
	// 2.获取第一个线程的信息
	int count = 0;
	if (Thread32First(hThreadSnap, &stcTe32)) {
		// 3. 循环获取所有的线程的信息
		do {
			if (stcTe32.th32OwnerProcessID == procID) {
				count++;
			}
		} while (Thread32Next(hThreadSnap, &stcTe32));
	}
	CloseHandle(hThreadSnap);
	CString countOfThreads;
	countOfThreads.Format(_T("%d"), count);
	return countOfThreads;
}

// 右键点击事件1
void CProcessMangerAlphaDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nRow = pNMItemActivate->iItem;
	// 获取进程ID
	g_ProcID = objListCtrl.GetItemText(nRow, 0);
	// 载入菜单
	CMenu objMyMenu;
	objMyMenu.LoadMenuW(IDR_MENU1);
	CMenu* pContextMenu = objMyMenu.GetSubMenu(0);
	CPoint pt = { };
	GetCursorPos(&pt);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
	*pResult = 0;
}

// 进程菜单选择
void CProcessMangerAlphaDlg::OnCommandRangePMenu(UINT nId) {
	switch (nId) {
		// 线程
		case ID_32771: {
			CThreadsDialog* objTd = new CThreadsDialog;
			objTd->DoModal();
			break;
		}
		// 模块
		case ID_32772: {
			CModulesDialog* objMd = new CModulesDialog;
			objMd->DoModal();
			break;
		}
		// 结束进程
		case ID_32773: {
			// CString ==> DWORD
			DWORD dwPid = _tcstol(g_ProcID, NULL, 10);
			HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
			TerminateProcess(hProc, 0);
			objListCtrl.DeleteItem(nRow);
			break;
		}
		// 查看堆信息
		case ID_32774: {
			CHeapDialog* objHd = new CHeapDialog;
			objHd->DoModal();
			break;
		}
		// 注入DLL
		case ID_32775: {
			MessageBox(_T("期待后续更新~~~"), _T("Oops!"));
			break;
		}
		default:
			break;
	}
}

// 主窗口主菜单
void CProcessMangerAlphaDlg::OnCommandRangeMMenu(UINT nId) {
	switch (nId) {
		// 查看服务
		case ID_32779: {
			CServListDialog* objSd = new CServListDialog;
			objSd->DoModal();
			break;
		}
		// PE解析
		case ID_32780: {
			CPEFileDialog* objPd = new CPEFileDialog;
			objPd->DoModal();
			break;
		}
		// 查看窗口
		case ID_32781: {
			CWindowsDialog* objWd = new CWindowsDialog;
			objWd->DoModal();
			break;
		}
		// 查看文件
		case ID_32782: {
			CFolderPickerDialog* pathDialog = new CFolderPickerDialog();
			pathDialog->DoModal();
			CString DirPath = pathDialog->GetFolderPath();
			CFilesDialog* objFd = new CFilesDialog;
			objFd->dirPath = DirPath;
			objFd->DoModal();
			break;
		}
		// 清理内存
		case ID_32783: {
			doCleanMemory();
			MessageBox(_T("内存清理完毕！"), _T("O.O"));
			break;
		}
		// 清理回收站
		case ID_32784: {
			GetWindowLong(m_hWnd, 0);
			SHEmptyRecycleBin(m_hWnd, NULL, SHERB_NOCONFIRMATION || SHERB_NOPROGRESSUI || SHERB_NOSOUND);
			MessageBox(_T("回收站已清空！"), _T("O.O"));
			break;
		}
		// 清理VS项目
		case ID_32785: {
			CVSCleanerDialog* objCd = new CVSCleanerDialog;
			objCd->DoModal();
			break;
		}
		// 杀毒
		case ID_32786: {
			MessageBox(_T("期待后续更新~~~"), _T("Oops!"));
			break;
		}
		// 关机
		case ID_32787: {
			upperPrivileges();
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION);
			break;
		}
		// 重启
		case ID_32788: {
			upperPrivileges();
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION);
			break;
		}
		// 注销
		case ID_32789: {
			upperPrivileges();
			ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION);
			break;
		}
		// 休眠
		case ID_32790: {
			upperPrivileges();
			SetSuspendState(TRUE, FALSE, FALSE);
			break;
		}
		// 老板键
		case ID_32791: {
			MessageBox(_T("Ctrl + Shift + H"),_T("提示 >_<"));
			break;
		}
		// 卸载软件
		case ID_32792: {
			CSoftwareUninstallDialog* objSUd = new CSoftwareUninstallDialog;
			objSUd->DoModal();
			break;
		}
		// 管理启动项
		case ID_32793: {
			
			break;
		}
		default:
			break;
	}
}

// 提权操作
void CProcessMangerAlphaDlg::upperPrivileges() {
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}

// 热键响应函数
BOOL CProcessMangerAlphaDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0x1234)) {
		if (::IsWindowVisible(m_hWnd)) {
			ShowWindow(SW_HIDE);
		}
		else {
			ShowWindow(SW_SHOW);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//计时器处理函数
void CProcessMangerAlphaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 1号计时器（系统时间）
	if (nIDEvent == 1) {
		// 时间
		SYSTEMTIME st;
		CString strDTime, strDate, strTime;
		GetLocalTime(&st);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		strDate.Format(_T("%4d-%2d-%2d"), st.wYear, st.wMonth, st.wDay);
		strTime.Format(_T("%2d:%2d:%2d"), st.wHour, st.wMinute, st.wSecond);
		strDTime = strDate + _T("  ") + strTime;
		objStatusBar.SetPaneText(0, strDTime);
	}
	// 2号计时器（内存和CPU占用）
	if (nIDEvent == 2) {
		// 获取新的时间
		_FILETIME newIdleTIme, newKernelTime, newUserTime;
		GetSystemTimes(&newIdleTIme, &newKernelTime, &newUserTime);
		// 转换时间
		double dOldIdleTime = FILETIME2Double(idleTime);
		double dNewIdleTime = FILETIME2Double(newIdleTIme);
		double dOldkernelTime = FILETIME2Double(kernelTime);
		double dNewKernelTime = FILETIME2Double(newKernelTime);
		double dOldUserTime = FILETIME2Double(userTime);
		double dNewUserTime = FILETIME2Double(newUserTime);

		// 给成员赋值
		idleTime = newIdleTIme;
		kernelTime = newKernelTime;
		userTime = newUserTime;

		// 计算使用率
		double cpu = (100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldkernelTime + dNewUserTime - dOldUserTime) * 100.0);
		// 获取内存占用率
		MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
		GlobalMemoryStatusEx(&memStatus);
		// 设置CPU和内存的内容
		CString MEM;
		MEM.Format(_T("内存占用率 %d%%"), memStatus.dwMemoryLoad);
		objStatusBar.SetPaneText(1, MEM);
		CString CPU;
		CPU.Format(_T("CPU使用率 %.2lf%%"), cpu);
		objStatusBar.SetPaneText(2, CPU);
	}
	CDialogEx::OnTimer(nIDEvent);
}

// 获取进程路径
CString CProcessMangerAlphaDlg::getProcFullPath(DWORD procID)
{
	// TODO: 在此处添加实现代码.
	TCHAR        szImagePath[MAX_PATH];
	TCHAR        pszFullPath[MAX_PATH];
	HANDLE        hProcess;
	if (!pszFullPath)
		return FALSE;
	pszFullPath[0] = '\0';
	//由进程ID获得线程信息
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, procID);    
	if (!hProcess)
		return FALSE;
	//得到进程完整DOS路径
	if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH)) {
		CloseHandle(hProcess);
		return FALSE;
	}
	//DOS路径转NT路径
	if (!DosPath2NTPath(szImagePath, pszFullPath)) {
		CloseHandle(hProcess);
		return FALSE;
	}
	CloseHandle(hProcess);
	return pszFullPath;
}

// DOS路径转NT路径
BOOL CProcessMangerAlphaDlg::DosPath2NTPath(LPTSTR DosPath, LPTSTR NTPath)
{
	// TODO: 在此处添加实现代码.
	TCHAR  szDriveStr[500];
	TCHAR  szDrive[3];
	TCHAR  szDevName[100];
	INT    iDevName;
	INT    i;
	//检查参数
	if (!DosPath || !NTPath)
		return FALSE;
	//获取本地磁盘所有盘符,以'\0'分隔,所以下面+4
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr)){
		for (i = 0; szDriveStr[i]; i += 4){
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;    //从C盘开始
			//盘符
			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			//查询 Dos 设备名(盘符由NT查询DOS)
			if (!QueryDosDevice(szDrive, szDevName, 100))
				return FALSE;
			iDevName = lstrlen(szDevName);
			//是否为此盘
			if (_tcsnicmp(DosPath, szDevName, iDevName) == 0){
				lstrcpy(NTPath, szDrive);//复制驱动器
				lstrcat(NTPath, DosPath + iDevName);//复制路径
				return TRUE;
			}
		}
	}
	lstrcpy(NTPath, DosPath);
	return FALSE;
}

// 清理内存
void CProcessMangerAlphaDlg::doCleanMemory()
{
	// TODO: 在此处添加实现代码.
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
}

