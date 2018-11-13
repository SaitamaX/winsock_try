// TCPclient.cpp : �������̨Ӧ�ó������ڵ㡣
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
		//����һ���׽��֣������б���ַ��TCP��UDP���׽���Э������TCP���׽���ʹ�õ��ض�Э��0�Զ�ָ��)
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
���ߣ�δ��
��Դ��CSDN
ԭ�ģ�https ://blog.csdn.net/shihoongbo/article/details/51913115 
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�