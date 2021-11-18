#pragma once
#include "CAllKillingDialog.h"
#include "CByPathKillingDialog.h"
#include "CBlacklistDialog.h"

#define UpdateVirus   0
#define PayloadVirus  1

// 自定义消息结构体
typedef struct _MYMSG {
	// 消息类型
	int msgType;
	// 数据
	char msgBuf[200];
}MYMSG, * PMYMSG;

// CVirusDefenceDialog 对话框

class CVirusDefenceDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVirusDefenceDialog)

public:
	CVirusDefenceDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVirusDefenceDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VirusDefenceDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	CString strFilePath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
};
