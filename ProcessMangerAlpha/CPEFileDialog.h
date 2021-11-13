#pragma once
#include "GlobalVals.h"
#include "CAddCalcDialog.h"
#include "CDirListDialog.h"

// CPEFileDialog 对话框

class CPEFileDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPEFileDialog)

public:
	CPEFileDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPEFileDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEFileDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 区段信息列表
	CListCtrl objSectionList;
	// 文件路径
	CString strFilePath;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CString wMachine;
	CString dwBaseOfCode;
	CString dwBaseOfData;
	CString dwBaseImage;
	CString dwSectionAlign;
	CString dwFileAlign;
	CString dwSizeOfImage;
	CString dwSizeOfHeads;
	CString wDllCharacts;
	CString dwNumberOfRvaAndSizes;
	CString dwTime;
	CString wSizeOfOptHead;
	CString wCharacteristic;
	CString wMagic;
	CString dwSizeOfCode;
	CString dwSizeOfInitCode;
	CString dwSizeOfUnInitCode;
	CString dwOEP;
	bool isPEFile(PVOID fileBuff);
	void getFileHeadInfo(PVOID fileBuff);
	void getOptionalHeadInfo(PVOID fileBuff);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	void getSectionHeadInfo(PVOID fileBuff);
//	afx_msg void OnDropFiles(HDROP hDropInfo);
};
