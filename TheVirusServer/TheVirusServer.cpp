// TheVirusServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "include\\mysql.h"
#pragma comment(lib,".\\libmysql.lib")
#include <atlstr.h>
#include <tchar.h>

#define UpdateVirus   0
#define PayloadVirus  1

// 自定义消息结构体
typedef struct _MYMSG {
	// 消息类型
	int msgType;
	// 数据
	char msgBuf[200];
}MYMSG, *PMYMSG;

// 初始化Mysql对象
MYSQL mysql = { 0 };
// 来自客户端的连接socket
SOCKET clientSocket;

// 初始化数据库
int InitDataBase() {
	// 初始化数据库
	mysql_library_init(NULL, NULL, NULL);
	mysql_init(&mysql);
	// 连接到数据库
	char IPAddress[20] = { "127.0.0.1" };
	char UserName[50] = { "root" };
	char PassWord[50] = { "toor" };
	char DataBaseName[50] = { "thevirus" };
	int nPort = 3306;
	mysql_real_connect(
		&mysql,        // MYSQL对象
		IPAddress,
		UserName,
		PassWord,
		DataBaseName,
		nPort,
		NULL,
		0
	);
	if (mysql_errno(&mysql) != 0) {
        printf("%d", mysql_errno(&mysql));
		return 0;
	}
	// 建表
	const char* CreateTableQuery =
		"CREATE TABLE IF NOT EXISTS sampleTable(                  \
			sid INT NOT NULL PRIMARY KEY AUTO_INCREMENT,          \
			md5 VARCHAR(100) NOT NULL                             \
		)";
	mysql_query(&mysql, CreateTableQuery);
}

// 更新病毒库
void doUpdateVirus() {
	char sqlBuf[100] = { "SELECT md5 FROM sampletable" };
	MYSQL_RES* pRes = NULL;
	mysql_query(&mysql, sqlBuf);
	// 有查询结果的需要判断结果集
	pRes = mysql_use_result(&mysql);
	if (pRes == nullptr) {
		//没有结果
	}
	else {
		//有结果
		char result[1000] = { 0 };
		//先获取列
		//1.先获取列的数量
		int nColumnCount = mysql_num_fields(pRes);
		//2. 循环遍历
		//获取行
		MYSQL_ROW row = nullptr;
		//mysql_fetch_row 遍历到最后一行的后面，返回NULL
		while (row = mysql_fetch_row(pRes)) {
			for (int i = 0; i < nColumnCount; i++) {
				if (row[i] == nullptr) {
				}
				else {
					strcat_s(result, 1000, row[i]);
				}
			}
		}
		int reslen = strlen(result);
		send(clientSocket, result, reslen, 0);
	}
}

// 上传样本
void doPayloadVirus(char buf[1000]) {
	char sqlBuf[1000] = { 0 };
	sprintf_s(sqlBuf, "INSERT INTO sampletable (md5) VALUES ('%s')", buf);
	mysql_query(&mysql, sqlBuf);
}

int main() {
    // 初始化数据库
    InitDataBase();
    // 初始化TCP连接
    // 库的版本号
    WORD wVersion = 0;
    // 传出参数，告知具体加载哪一个库
    WSADATA wsd = { 0 };
    wVersion = MAKEWORD(2, 2);
    int nSuccess = WSAStartup(wVersion, &wsd);
    if (nSuccess != 0) {
        printf("初始化失败");
        return 0;
    }
    // 2. 创建一个socket 用于接收客户端的连接
    SOCKET ListenSocket =
        socket(
            AF_INET,                // TCP/IP协议族
            SOCK_STREAM,            // 流式协议
            IPPROTO_TCP             // 使用TCP协议
        );
    if (ListenSocket == INVALID_SOCKET) {
        WSACleanup();
        printf("Socket 创建失败");
        return 0;
    }
    // 3. 绑定到IP和PORT 
    sockaddr_in addrin = { 0 };
    addrin.sin_family = AF_INET;      // 协议簇
    // host to net short 类型
    // 主机字节序转网络字节序：小尾转大尾
    addrin.sin_port = htons(6666);     // 端口号
    addrin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    nSuccess =
        bind(
            ListenSocket,            // 要绑定的socket
            (sockaddr*)&addrin,      // 绑定的IP和PORT
            sizeof(addrin)           // 结构的大小
        );
    if (SOCKET_ERROR == nSuccess) {
        printf("绑定失败");
        closesocket(ListenSocket);
        WSACleanup();
        return 0;
    }
    // 4. 监听、设置最大连接请求数            
    listen(
        ListenSocket,                  // 监听socket
        1                             // 最大请求数10
    );
    // 5. 接受客户端的连接
    // 需要知道IP和PORT
    // 客户端的socket信息
    sockaddr_in addrinClient = { 0 };
    int nLen = sizeof(sockaddr_in);
	// accept 函数是阻塞的
	// 当没有连接请求的时候它是卡住的
	clientSocket =
		accept(
			ListenSocket,            // 通过绑定好的socket接受连接
			(sockaddr*)&addrinClient, // 建立收到连接请求之后获取目标IP和PORT
			&nLen                    // 结构体大小
		);
    while (true) {
        // 接受
        char recvBuf[1000] = { 0 };
        recv(clientSocket, recvBuf, 1000, 0);
		if (PMYMSG(recvBuf)->msgType == UpdateVirus) {
			doUpdateVirus();
		}
		if (PMYMSG(recvBuf)->msgType == PayloadVirus) {
			doPayloadVirus(PMYMSG(recvBuf)->msgBuf);
		}
    }
	// 7. 关闭socket
	closesocket(ListenSocket);
	// 清理WinSock库
	WSACleanup();
	return 0;
}

