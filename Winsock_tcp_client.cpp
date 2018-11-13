// TCPclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define SERVERIP "192.168.3.23"
#define SERVERPORT 5050

int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA initData;
	int err = WSAStartup(sockVersion, &initData);
	if (err != 0)
		cout << "init Failed!" << endl;
	while (true)
	{
		//创建一个套接字，参数列表（地址族TCP、UDP；套接字协议类型TCP；套接字使用的特定协议0自动指定)
		SOCKET mysocket = socket(AF_INET, SOCK_STREAM, 0);
		if (mysocket == INVALID_SOCKET)
		{
			cout << "create socket failed!" << endl;
			return 0;
		}

		sockaddr_in hostAddr;
		hostAddr.sin_family = AF_INET;
		hostAddr.sin_port = htons(SERVERPORT);
		in_addr addr;
		inet_pton(AF_INET, SERVERIP, (void*)&addr);
		hostAddr.sin_addr = addr;
		cout << "ip:" << addr.S_un.S_addr << endl;

		SOCKET conSock = socket(AF_INET, SOCK_STREAM, 0);
		if (conSock == INVALID_SOCKET)
		{
			cout << "conSock failed" << endl;
			system("pause");
			return 0;
		}

		err = connect(conSock, (sockaddr*)&hostAddr, sizeof(sockaddr));
		if (err == INVALID_SOCKET)
		{
			cout << "connect failed!" << endl;
			system("pause");
			return 0;
		}


		char buf[1024] = "\0";
		cout << "input data: ";
		cin >> buf;
		err = send(conSock, buf, strlen(buf), 0);
		if (err == SOCKET_ERROR)
		{
			cout << "send failed!" << endl;
			system("pause");
			return 0;
		}
		if (closesocket(conSock) != 0)
		{
			cout << "closesocket failed!" << endl;
			system("pause");
			return 0;
		}
	}


	WSACleanup();

	system("pause");
	return 0;
}

-------------------- -
作者：未狂
来源：CSDN
原文：https ://blog.csdn.net/shihoongbo/article/details/51913115 
版权声明：本文为博主原创文章，转载请附上博文链接！