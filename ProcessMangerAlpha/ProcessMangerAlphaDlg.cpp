
// ProcessMangerAlphaDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ProcessMangerAlpha.h"
#include "ProcessMangerAlphaDlg.h"
#include "afxdialogex.h"

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
//	void getThreadsInfoByProcID(CString ProcID);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
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
	
	// 添加列
	objListCtrl.InsertColumn(0, _T("PID"),    LVCFMT_CENTER, 100);
	objListCtrl.InsertColumn(1, _T("进程名"),  LVCFMT_CENTER, 200);
	objListCtrl.InsertColumn(2, _T("线程数量"), LVCFMT_CENTER, 100);
	traverseProcess();


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
			i++;
		} while (Process32Next(hProcessSnap, &stcPe32));
	}
	// 样式
	objListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
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
			
			break;
		}
		default:
			break;
	}
}

//void CAboutDlg::getThreadsInfoByProcID(CString ProcID)
//{
//	// TODO: 在此处添加实现代码.
//}
