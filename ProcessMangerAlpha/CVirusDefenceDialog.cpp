// CVirusDefenceDialog.cpp: 实现文件
//
#include "pch.h"
#include "EnDeCrypt.h"
#include "ProcessMangerAlpha.h"
#include "CVirusDefenceDialog.h"
#include "afxdialogex.h"
#include "GlobalVals.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <vector>
using std::vector;
vector<char*>PWVt;

// 跟服务端通信用的socket
SOCKET ConnectSocket;

// CVirusDefenceDialog 对话框

IMPLEMENT_DYNAMIC(CVirusDefenceDialog, CDialogEx)

CVirusDefenceDialog::CVirusDefenceDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VirusDefenceDialog, pParent)
{

}

CVirusDefenceDialog::~CVirusDefenceDialog()
{
}

void CVirusDefenceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVirusDefenceDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CVirusDefenceDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CVirusDefenceDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CVirusDefenceDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVirusDefenceDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CVirusDefenceDialog::OnBnClickedButton5)
END_MESSAGE_MAP()


// CVirusDefenceDialog 消息处理程序

BOOL CVirusDefenceDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化WinSock库
	// 使用库的版本
	WORD wVersion = 0;
	// 传出参数
	WSADATA wsd = { 0 };
	wVersion = MAKEWORD(2, 2);
	int nSuccess = WSAStartup(wVersion, &wsd);
	if (nSuccess != 0) {
		MessageBox(L"初始化WinSock库失败，请稍后再试");
	}
	else {
		// 创建一个socket 连接服务端
		ConnectSocket = socket(
			AF_INET,       // TCP/IP协议簇
			SOCK_STREAM,   // 流式协议
			IPPROTO_TCP    // 使用TCP协议
		);
		if (ConnectSocket == INVALID_SOCKET) {
			WSACleanup();
			MessageBox(L"创建socket失败，请稍后再试");
		}
		else {
			// 主动连接服务端
			sockaddr_in addrin = { 0 };
			addrin.sin_family = AF_INET;
			addrin.sin_port = htons(6666);
			addrin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			connect(
				ConnectSocket,       // 连接用的socket
				(sockaddr*)&addrin,  // 连接到哪一个IP和端口上
				sizeof(sockaddr_in)  // 结构体大小
			);
		}
	}
	// 生成一对密钥
	// generateRSAKEY();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 更新病毒库
void CVirusDefenceDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置消息结构体
	// 发送
	MYMSG myMsg = {};
	myMsg.msgType = UpdateVirus;
	strcpy_s(myMsg.msgBuf, 200, "0");
	send(ConnectSocket,(char*)&myMsg, sizeof(myMsg), 0);
	// 接收消息
	char recvBuf[200] = { 0 };
	recv(ConnectSocket, recvBuf, 200, 0);
	// char*====>byte*
	unsigned char tempBuf[200] = { 0 };
	memcpy_s(tempBuf, 200, recvBuf, 200);
	MessageBox(_T("解密中..."), _T("Tips"));

	//// 打开文件
	//FILE* pFile = NULL;
	//fopen_s(&pFile, "C:\\Users\\Yagami\\Desktop\\TheVirus.txt", "w");
	// 按位截断
	//int k = 0;
	//while (k < recvLen) {
	//	if ((k + 1) % 88 == 0) {
	//		unsigned char uchar[100] = { 0 };
	//		for (int g = k + 1 - 88; g < k + 1; g++) {	
	//			uchar[g] = tempBuf[g];
	//		}
	//		// AES 解密
	//		unsigned char deBuf[100] = { 0 };
	//		int deLength = evpDecrypt(uchar, deBuf, 88);
	//		// byte*=====>char*
	//		char deMD5[100] = { 0 };
	//		memcpy_s(deMD5, 100, deBuf, 100);
	//		// 写入文件
	//		fprintf(pFile, "%s\n", deMD5);
	//	}
	//	k++;
	//}

	// AES 解密
	unsigned char deBuf[100] = { 0 };
	int deLength = evpDecrypt(tempBuf, deBuf, 88);
	// byte*=====>char*
	char deMD5[100] = { 0 };
	memcpy_s(deMD5, 100, deBuf, 100);

	// 打开文件
	FILE* pFile = NULL;
	fopen_s(&pFile, "C:\\Users\\Yagami\\Desktop\\TheVirus.txt", "w+");
	// 写入文件
	fprintf(pFile, "%s\n", deMD5);
	
	MessageBox(_T("更新成功，本地病毒库已经是最新的了"), _T(">_"));
	fclose(pFile);
}

// 上传样本 
void CVirusDefenceDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog* pFilePicker = new CFileDialog(true, _T("文件名"), _T("都行"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File (*.*)|*.*||"), NULL);
	pFilePicker->DoModal();
	strFilePath = pFilePicker->GetPathName();
	// 打开一个文件
	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// 获取文件大小 
	DWORD fileSize = GetFileSize(hFile, NULL);
	// 申请空间保存文件内容, 如果读取成功就申请空间
	char* fileBuff = new char[fileSize];
	// 读取文件
	DWORD read_size = 0;
	errno_t err = ReadFile(hFile, fileBuff, fileSize, &read_size, NULL);
	unsigned char fileMD5[40] = { 0 };
	// MD5 加密
	MD5Encrypt(fileBuff, fileSize, fileMD5);

	// BYTE*====>CString
	int j = 0;
	CString strEn;
	while (fileMD5[j] != 0) {
		CString strtemp;
		strtemp.Format(_T("%02x"), fileMD5[j]);
		strEn += strtemp;
		j++;
	}
	// 设置消息结构体
	MYMSG myMsg = {};
	myMsg.msgType = PayloadVirus;
	char tempBuf[200] = { 0 };
	// CString====>char*
	sprintf(tempBuf, "%S", strEn.GetBuffer());

	// AES 加密
	unsigned char fileAES[200] = { 0 };
	unsigned char tempBuff[200] = { 0 };
	// char*=====>byte*
	memcpy_s(tempBuff, 200, tempBuf, 200);
	int enLength = evpEncrypt(tempBuff, fileAES, 40);
	// byte*=====>char*
	memcpy_s(myMsg.msgBuf, 200, fileAES, 200);

	send(ConnectSocket, (char*)&myMsg, 200, 0);
	MessageBox(_T("上传成功！"));
	// 关闭文件
	CloseHandle(hFile);
}

// 全盘查杀
void CVirusDefenceDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CAllKillingDialog* objAKd = new CAllKillingDialog;
	objAKd->DoModal();
}

// 自定义查杀
void CVirusDefenceDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog* pathDialog = new CFolderPickerDialog();
	pathDialog->DoModal();
	CString DirPath = pathDialog->GetFolderPath();
	CByPathKillingDialog* objBPKd = new CByPathKillingDialog;
	objBPKd->strDirPath = DirPath;
	objBPKd->DoModal();
}

// 黑名单
void CVirusDefenceDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CBlacklistDialog* objBLd = new CBlacklistDialog;
	objBLd->DoModal();
}
