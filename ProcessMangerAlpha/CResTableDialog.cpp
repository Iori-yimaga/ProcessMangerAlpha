// CResTableDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CResTableDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include <map>
using std::map;
map<HTREEITEM, PIMAGE_RESOURCE_DIRECTORY>firstMap;

// 资源表基址
DWORD resourceBase = 0;
// 第二层表entry
PIMAGE_RESOURCE_DIRECTORY second = NULL;
PIMAGE_RESOURCE_DIRECTORY_ENTRY secondTable = NULL;

// CResTableDialog 对话框

IMPLEMENT_DYNAMIC(CResTableDialog, CDialogEx)

CResTableDialog::CResTableDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ResTableDialog, pParent)
	, strClassSec(_T(""))
	, strIDSec(_T(""))
	, strIDFirst(_T(""))
	, strFOA(_T(""))
	, strRVA(_T(""))
	, strSize(_T(""))
	, strClassFirst(_T(""))
{

}

CResTableDialog::~CResTableDialog()
{
}

void CResTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, objResTree);
	//  DDX_Text(pDX, IDC_EDIT1, strClassSec);
	//  DDX_Text(pDX, IDC_EDIT2, strIDSec);
	//  DDX_Control(pDX, IDC_EDIT1, strClassFirst);
	DDX_Text(pDX, IDC_EDIT2, strIDFirst);
	DDX_Text(pDX, IDC_EDIT3, strClassSec);
	//  DDX_Control(pDX, IDC_EDIT4, strIDSec);
	//  DDX_Control(pDX, IDC_EDIT5, strRVA);
	DDX_Text(pDX, IDC_EDIT6, strFOA);
	DDX_Text(pDX, IDC_EDIT4, strIDSec);
	DDX_Text(pDX, IDC_EDIT5, strRVA);
	DDX_Text(pDX, IDC_EDIT7, strSize);
	DDX_Text(pDX, IDC_EDIT1, strClassFirst);
}


BEGIN_MESSAGE_MAP(CResTableDialog, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CResTableDialog::OnSelchangedTree1)
END_MESSAGE_MAP()


// CResTableDialog 消息处理程序


BOOL CResTableDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	getResTableUnfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 获取资源表信息
void CResTableDialog::getResTableUnfo()
{
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取扩展头
	optHead = PIMAGE_OPTIONAL_HEADER(&ntHead->OptionalHeader);
	// 找到数据目录表的第三项，也就是下标为2
	DWORD resourceTableRVA = optHead->DataDirectory[2].VirtualAddress;
	DWORD resourceTableFOA = rvaTofoa(resourceTableRVA);
	// 资源表的基址
	resourceBase = resourceTableFOA + (DWORD)fileBuff;
	PIMAGE_RESOURCE_DIRECTORY resourceTable = PIMAGE_RESOURCE_DIRECTORY(resourceBase);
	// 名称条目
	DWORD nameCount = resourceTable->NumberOfNamedEntries;
	strClassFirst.Format(_T("%.4x"), nameCount);
	// ID条目
	DWORD IDCount = resourceTable->NumberOfIdEntries;
	strIDFirst.Format(_T("%.4X"), IDCount);
	DWORD count = nameCount + IDCount;
	// 得到第一种资源
	PIMAGE_RESOURCE_DIRECTORY_ENTRY firstTable = PIMAGE_RESOURCE_DIRECTORY_ENTRY(resourceTable + 1);
	// 遍历第一种资源
	CString strClassName, strOrdinal, strName;
	for (unsigned int i = 0; i < count; i++) {
		// 记录资源名称是一个ID还是一个字符串
		// 如果不是1，就是一个ID
		printf("资源类型：");
		if (firstTable->NameIsString != 1) {
			switch (firstTable->Id) {
			case 0x1:  strClassName = _T("鼠标指针"); break;
			case 0x2:  strClassName = _T("位图"); break;
			case 0x3:  strClassName = _T("图标"); break;
			case 0x4:  strClassName = _T("菜单"); break;
			case 0x5:  strClassName = _T("对话框"); break;
			case 0x6:  strClassName = _T("字符串列表"); break;
			case 0x7:  strClassName = _T("字体目录"); break;
			case 0x8:  strClassName = _T("资源类型"); break;
			case 0x9:  strClassName = _T("字体"); break;
			case 0xA:  strClassName = _T("快捷键"); break;
			case 0xB:  strClassName = _T("非格式化资源"); break;
			case 0xC:  strClassName = _T("消息列表"); break;
			case 0xD:  strClassName = _T("鼠标指针组"); break;
			case 0xE:  strClassName = _T("图标组"); break;
			case 0x10: strClassName = _T("版本信息"); break;
			default:   strClassName = _T("UnDefine"); break;
			}
		}
		else {
			// 获取名称字符串
			PIMAGE_RESOURCE_DIR_STRING_U nameString = PIMAGE_RESOURCE_DIR_STRING_U(resourceBase + firstTable->NameOffset);
			strClassName = nameString->NameString;
		}
		// 根目录（资源种类）
		HTREEITEM hRoot = objResTree.InsertItem(strClassName);
		// 判断下一层是否为目录
		if (firstTable->DataIsDirectory == 1) {
			// 获取第二层的entry
			second = PIMAGE_RESOURCE_DIRECTORY(firstTable->OffsetToDirectory + resourceBase);
			DWORD resourceCount = second->NumberOfNamedEntries + second->NumberOfIdEntries;
			secondTable = PIMAGE_RESOURCE_DIRECTORY_ENTRY(second + 1);
			// 保存根节点与表的关系
			firstMap[hRoot] = second;
			for (unsigned int j = 0; j < resourceCount; j++) {
				if (secondTable->NameIsString != 1) {
					strOrdinal.Format(_T("%d"), secondTable->Id);
					HTREEITEM hLeaf = objResTree.InsertItem(strOrdinal, hRoot);
					if (secondTable->DataIsDirectory == 1) {
						firstMap[hLeaf] = PIMAGE_RESOURCE_DIRECTORY(secondTable->OffsetToDirectory + resourceBase);
					}
				}
				else {
					// 获取名称字符串
					PIMAGE_RESOURCE_DIR_STRING_U nameString = PIMAGE_RESOURCE_DIR_STRING_U(resourceBase + secondTable->NameOffset);
					//wprintf(L"%s\n", nameString->NameString);
					strName = nameString->NameString;
					HTREEITEM hLeaf = objResTree.InsertItem(strName, hRoot);
					if (secondTable->DataIsDirectory == 1) {
						firstMap[hLeaf] = PIMAGE_RESOURCE_DIRECTORY(secondTable->OffsetToDirectory + resourceBase);
					}
				}
				secondTable++;
			}
		}
		firstTable++;
	}
	UpdateData(FALSE);
}

// 树控件的点击事件
void CResTableDialog::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hCurNode = objResTree.GetSelectedItem();
	if (objResTree.GetParentItem(hCurNode) == NULL) {
		PIMAGE_RESOURCE_DIRECTORY firstTable = firstMap[hCurNode];
		strClassSec.Format(_T(" %.4X"), firstTable->NumberOfNamedEntries);
		strIDSec.Format(_T("%.4X"), firstTable->NumberOfIdEntries);
		// 清空下面的框
		strRVA = "";
		strFOA = "";
		strSize = "";
		UpdateData(FALSE);
	}
	else {
		// 获取父节点的信息
		HTREEITEM hparent = objResTree.GetParentItem(hCurNode);
		PIMAGE_RESOURCE_DIRECTORY secondTable = firstMap[hparent];
		strClassSec.Format(_T(" %.4X"), secondTable->NumberOfNamedEntries);
		strIDSec.Format(_T("%.4X"), secondTable->NumberOfIdEntries);
		// 获取自己的信息
		getDataEntryInfo(hCurNode);
	}
	*pResult = 0;
}

// 通过第二层找到第三层
void CResTableDialog::getDataEntryInfo(HTREEITEM hTree)
{
	// TODO: 在此处添加实现代码.

	PIMAGE_RESOURCE_DIRECTORY third = firstMap[hTree];
	PIMAGE_RESOURCE_DIRECTORY_ENTRY thirdTable = PIMAGE_RESOURCE_DIRECTORY_ENTRY(third + 1);
	PIMAGE_RESOURCE_DATA_ENTRY data = PIMAGE_RESOURCE_DATA_ENTRY(thirdTable->OffsetToData + resourceBase);
	//printf("资源数据的RVA：0x%X\t资源数据的大小：0x%X\n", data->OffsetToData, data->Size);
	strRVA.Format(_T("%.8X"), data->OffsetToData);
	strFOA.Format(_T("%.8X"), rvaTofoa(data->OffsetToData));
	strSize.Format(_T("%.8X"), data->Size);
	UpdateData(FALSE);
}
