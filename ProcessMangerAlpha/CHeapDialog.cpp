// CHeapDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CHeapDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CHeapDialog 对话框

IMPLEMENT_DYNAMIC(CHeapDialog, CDialogEx)

CHeapDialog::CHeapDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HeapDialog, pParent)
{

}

CHeapDialog::~CHeapDialog()
{
}

void CHeapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, objHeapList);
}


BEGIN_MESSAGE_MAP(CHeapDialog, CDialogEx)
END_MESSAGE_MAP()


// CHeapDialog 消息处理程序


BOOL CHeapDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	objHeapList.InsertColumn(0, _T("HeapID"), LVCFMT_CENTER, 200);
	objHeapList.InsertColumn(1, _T("堆大小"), LVCFMT_CENTER, 200);
	getHeapInfoByProcID(g_ProcID);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 通过进程ID获取堆信息
void CHeapDialog::getHeapInfoByProcID(CString ProcID)
{
	// TODO: 在此处添加实现代码.
	// CString ==> DWORD
	DWORD dwPid = _tcstol(ProcID, NULL, 10);
	// 创建堆快照
	HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPid);
	// 堆结构体
	HEAPLIST32 hList = { sizeof(HEAPLIST32) };
	// 遍历堆快照
	int i = 0;
	if (Heap32ListFirst(hHeapSnap, &hList)) {
		do {
			// 遍历堆块数据
			HEAPENTRY32 hHeapEntry = { sizeof(HEAPENTRY32) };
			if (Heap32First(&hHeapEntry, dwPid, hList.th32HeapID)) {
				objHeapList.InsertItem(i, _T(""));
				// 堆ID
				CString HeapID;
				HeapID.Format(_T("0x%X"), hHeapEntry.th32HeapID);
				objHeapList.SetItemText(i, 0, HeapID);
				// 堆块大小
				CString HeapSize;
				HeapSize.Format(_T("%d"), hHeapEntry.dwBlockSize);
				objHeapList.SetItemText(i, 1, HeapSize);
				i++;
			}
		} while (Heap32ListNext(hHeapSnap, &hList));
	}
	// 样式
	objHeapList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	CloseHandle(hHeapSnap);
}
