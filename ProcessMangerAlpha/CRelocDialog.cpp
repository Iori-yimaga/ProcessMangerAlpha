// CRelocDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CRelocDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include <map>
using std::map;
map<DWORD, PIMAGE_BASE_RELOCATION>DPMap;

DWORD baseRVA = 0;
DWORD baseVA = 0;
DWORD realVA = 0;

typedef struct _TYPEOFFSET {
	WORD offset : 12;
	WORD Type : 4;
}TYPEOFFSET, * PTYPEOFFSET;

// CRelocDialog 对话框

IMPLEMENT_DYNAMIC(CRelocDialog, CDialogEx)

CRelocDialog::CRelocDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RelocDialog, pParent)
{

}

CRelocDialog::~CRelocDialog()
{
}

void CRelocDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, objSectionList);
	DDX_Control(pDX, IDC_LIST2, objBlockList);
}


BEGIN_MESSAGE_MAP(CRelocDialog, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CRelocDialog::OnClickList1)
END_MESSAGE_MAP()


// CRelocDialog 消息处理程序


BOOL CRelocDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 区段列表
	objSectionList.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 80);
	objSectionList.InsertColumn(1, _T("Section"), LVCFMT_CENTER, 100);
	objSectionList.InsertColumn(2, _T("RVA"), LVCFMT_CENTER, 100);
	objSectionList.InsertColumn(3, _T("Items"), LVCFMT_CENTER, 100);
	objSectionList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	getRelocTableInfo();
	// 块项目列表
	objBlockList.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 80);
	objBlockList.InsertColumn(1, _T("RVA"), LVCFMT_CENTER, 100);
	objBlockList.InsertColumn(2, _T("Offset"), LVCFMT_CENTER, 100);
	objBlockList.InsertColumn(3, _T("Type"), LVCFMT_CENTER, 100);
	objBlockList.InsertColumn(4, _T("Far Address"), LVCFMT_CENTER, 100);
	objBlockList.InsertColumn(5, _T("Data Interpretation"), LVCFMT_CENTER, 100);
	objBlockList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取重定位表（区段信息）
void CRelocDialog::getRelocTableInfo()
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER(&ntHead->OptionalHeader);
	// 找到数据目录表的第六项，也就是下标为5
	DWORD relocTableRVA = optHead->DataDirectory[5].VirtualAddress;
	DWORD relocTableFOA = rvaTofoa(relocTableRVA);
	// 获取重定位表
	PIMAGE_BASE_RELOCATION reloc = PIMAGE_BASE_RELOCATION(relocTableFOA + (DWORD)fileBuff);
	int relocIndex = 0;
	while (reloc->SizeOfBlock != 0) {
		CString strIndex, strSection, strPTRDRVA, strCount;
		// 索引
		strIndex.Format(_T("%d"), relocIndex + 1);
		baseRVA = reloc->VirtualAddress;                  // 0X012000
		baseVA = baseRVA + (DWORD)fileBuff;               // 0x412000
		realVA = rvaTofoa(baseRVA);                       // 0x1400
		// 区域起始RVA
		strPTRDRVA.Format(_T("%.8X"), realVA);
		// 保存一下此RVA对应的reloc表
		DPMap[realVA] = reloc;
		// 区段名
		strSection = sectionName;
		// 个数
		DWORD count = (reloc->SizeOfBlock - 8) / 2;
		strCount.Format(_T("%Xh / %dd"), count, count);
		// 插入
		objSectionList.InsertItem(relocIndex, _T(""));
		objSectionList.SetItemText(relocIndex, 0, strIndex);
		objSectionList.SetItemText(relocIndex, 1, strSection);
		objSectionList.SetItemText(relocIndex, 2, strPTRDRVA);
		objSectionList.SetItemText(relocIndex, 3, strCount);
		// 当前位置加上当前的大小，就找到了下一页
		reloc = PIMAGE_BASE_RELOCATION((DWORD)reloc + reloc->SizeOfBlock);
		relocIndex++;
	}
}

// 区段表点击事件
void CRelocDialog::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nRow = pNMItemActivate->iItem;
	// 获取鼠标选择的RVA
	CString strRVA = objSectionList.GetItemText(nRow, 2);
	DWORD dwRVA = _tcstol(strRVA, NULL, 16);
	PIMAGE_BASE_RELOCATION curReloc = DPMap[dwRVA];
	// 清空一下块信息表
	objBlockList.DeleteAllItems();
	getBlocksInfo(curReloc);
	*pResult = 0;
}

// 获取块信息
void CRelocDialog::getBlocksInfo(PIMAGE_BASE_RELOCATION reloc)
{
	// TODO: 在此处添加实现代码.
	PTYPEOFFSET firstTable = PTYPEOFFSET(reloc + 1);
	DWORD count = (reloc->SizeOfBlock - 8) / 2;
	CString strIndex, strRVA, strFOA, strType, strVA, strData;
	for (unsigned int i = 0; i < count; i++) {
		if (firstTable[i].Type == 3) {
			strIndex.Format(_T("%d"), i + 1);
			// 计算出真正需要重定位的RVA
			DWORD realRVA = baseRVA + firstTable[i].offset;      // 0x129b8
			strRVA.Format(_T("%.8X"), realRVA);
			DWORD realFOA = realVA + firstTable[i].offset;       // 0x1db8
			strFOA.Format(_T("%.8X"), realFOA);
			strType = _T("HIGHLOW(3)");
			strVA.Format(_T("%.8X"), realRVA + (DWORD)fileBuff);
			PDWORD data = PDWORD(realFOA + (DWORD)fileBuff);
			strData.Format(_T("%.8X"), *data);

			objBlockList.InsertItem(i, _T(""));
			objBlockList.SetItemText(i, 0, strIndex);
			objBlockList.SetItemText(i, 1, strRVA);
			objBlockList.SetItemText(i, 2, strFOA);
			objBlockList.SetItemText(i, 3, strType);
			objBlockList.SetItemText(i, 4, strVA);
			objBlockList.SetItemText(i, 5, strData);
		}
	}
}
