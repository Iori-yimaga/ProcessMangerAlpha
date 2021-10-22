#pragma once
#include "GlobalVals.h"

// CHeapDialog 对话框

class CHeapDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHeapDialog)

public:
	CHeapDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHeapDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HeapDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl objHeapList;
	virtual BOOL OnInitDialog();
	void getHeapInfoByProcID(CString ProcID);
};
