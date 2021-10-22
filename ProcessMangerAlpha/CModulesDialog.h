#pragma once 
#include "GlobalVals.h"
// CModulesDialog 对话框

class CModulesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CModulesDialog)

public:
	CModulesDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CModulesDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ModuleDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objModulesList;
	virtual BOOL OnInitDialog();
	void getModulesInfoByProcID(CString ProcID);
};
