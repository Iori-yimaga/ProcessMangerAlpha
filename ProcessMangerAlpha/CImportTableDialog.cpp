// CImportTableDialog.cpp: 实现文件
//

#include "pch.h"
#include "ProcessMangerAlpha.h"
#include "CImportTableDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"

// CImportTableDialog 对话框

IMPLEMENT_DYNAMIC(CImportTableDialog, CDialogEx)

CImportTableDialog::CImportTableDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImportTableDialog, pParent)
{

}

CImportTableDialog::~CImportTableDialog()
{
}

void CImportTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImportTableDialog, CDialogEx)
END_MESSAGE_MAP()


// CImportTableDialog 消息处理程序
