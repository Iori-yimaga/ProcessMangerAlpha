// CModulesDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CModulesDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <Psapi.h>

// CModulesDialog 对话框

IMPLEMENT_DYNAMIC(CModulesDialog, CDialogEx)

CModulesDialog::CModulesDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ModuleDialog, pParent)
{

}

CModulesDialog::~CModulesDialog()
{
}

void CModulesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, objModulesList);
}


BEGIN_MESSAGE_MAP(CModulesDialog, CDialogEx)
END_MESSAGE_MAP()


// CModulesDialog 消息处理程序


BOOL CModulesDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 添加列
	objModulesList.InsertColumn(0, _T("模块路径"), LVCFMT_CENTER, 200);
	objModulesList.InsertColumn(1, _T("DLL入口点"), LVCFMT_CENTER, 200);
	objModulesList.InsertColumn(2, _T("DLL基址"), LVCFMT_CENTER, 200);
	objModulesList.InsertColumn(3, _T("DLL大小"), LVCFMT_CENTER, 200);
	getModulesInfoByProcID(g_ProcID);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 根据进程ID获取模块信息
void CModulesDialog::getModulesInfoByProcID(CString ProcID)
{
	// CString ==> DWORD
	DWORD dwPid = _tcstol(ProcID, NULL, 10);
	// TODO: 在此处添加实现代码.
	MODULEENTRY32 stcMe32 = { 0 };
	stcMe32.dwSize = sizeof(MODULEENTRY32);
	// 1.创建模块快照
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,      // 创建模块快照
		dwPid                   // 进程ID
	);
	// 2.获取第一个模块的信息
	int i = 0;
	if (Module32First(hModuleSnap, &stcMe32)) {
		// 3. 循环获取所有的模块的信息
		do {
			objModulesList.InsertItem(i, _T(""));
			objModulesList.SetItemText(i, 0, stcMe32.szExePath);
			CString moduleBaseAddr;
			moduleBaseAddr.Format(_T("0x%X"), stcMe32.modBaseAddr);
			objModulesList.SetItemText(i, 2, moduleBaseAddr);
			CString moduleSize;
			moduleSize.Format(_T("%d"), stcMe32.modBaseSize);
			objModulesList.SetItemText(i, 3, moduleSize);
		} while (Module32Next(hModuleSnap, &stcMe32));
	}

	//HANDLE hProcess = NULL;
	//hProcess = OpenThread(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	//HMODULE hModules[0x2000] = {};
	//DWORD dwNeed = 0;
	//EnumProcessModulesEx(hProcess, hModules, sizeof(hModules), &dwNeed, LIST_MODULES_ALL);
	//DWORD dwModeleCount = dwNeed / sizeof(HMODULE);
	//MODULEINFO mofi = { 0 };
	//WCHAR modulesPath[100] = { 0 };
	//int mpSize = 100;
	//for (unsigned int i = 0; i < dwModeleCount; i++) {
	//	GetModuleFileNameEx(hProcess, hModules[i], modulesPath, mpSize);
	//	GetModuleInformation(hProcess, hModules[i], &mofi, sizeof(MODULEINFO));
	//	objModulesList.InsertItem(i, _T(""));
	//	objModulesList.SetItemText(i, 0, modulesPath);
	//	CString entryPoint;
	//	entryPoint.Format(_T("0x%P"), mofi.EntryPoint);
	//	objModulesList.SetItemText(i, 1, entryPoint);
	//	CString baseOfDll;
	//	baseOfDll.Format(_T("0x%P"), mofi.lpBaseOfDll);
	//	objModulesList.SetItemText(i, 2, baseOfDll);
	//	CString sizeImage;
	//	sizeImage.Format(_T("%d"), mofi.SizeOfImage);
	//	objModulesList.SetItemText(i, 3, sizeImage);
	//}
	// 样式
	objModulesList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CloseHandle(hModuleSnap);
}
