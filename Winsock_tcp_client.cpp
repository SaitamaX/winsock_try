// 网络聊天_客户端.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
//#include <Ws2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

//接收服务器发来的数据的进程
void recvProc(SOCKET socketSer)
{
	char msgRcv[100] = { 0 };
	while (true)
	{
		/*
		调用recv函数的目的：接受服务进程发来的数据

		该函数的函数原型：int recv(SOCKET s,char FAR *buf,int len,int flags);

		s:表示一个要接收数据的套接字的描述字（客户端进程自身创建的套接字）

		buf:即将接收到数据的字符缓冲区

		len:准备接收的字符数或buf缓冲的长度

		flags: 0：表示无特殊行为
		MSG_PEEk：表示使有用的数据复制到所提供的接收缓冲区内，但没有从系统缓冲区中将它删除
		MSG_OOB：接收带外数据（紧急数据）

		返回值：若函数成功调用则返回0,否则返回错误信息.
		*/
		if (SOCKET_ERROR == recv(socketSer, msgRcv, sizeof(msgRcv), 0))
		{
			cout << "服务器已关闭" << endl;
			return;
		}
		else {
			cout << msgRcv << endl;
			ZeroMemory(msgRcv, sizeof(msgRcv));
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	/*
	调用WSAStartup函数的目的：对Winsock DLL进行初始化（Winsock的服务是以动态链接库Winsock DLL的形式实现的），
	获取Winsock的版本支持，并分配必要的资源。

	该函数的函数原型：int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData);

	wVersionRequested:用于指定准备加载的Winsock库的版本，可以用宏MAKEWORD( X, Y) (X为高位字节，Y为地位字节)来获得
	wVersionRequested的正确值，其中高位字节指定所需的Winsock库的副版本,低位字节指定主版本。

	lpWSAData:指向LPWSAData结构的指针，该结构包含了与加载的库版本有关的信息：
	typeDef struct WSAData{
	WORD      wVersion;//希望使用的Winsock库的版本
	WORD      wHighVersion;//现有的Winsock库的最高版本
	. . . . . .
	}WSADATA,* LPWSADATA;

	返回值：若函数成功调用则返回0,否则返回错误信息.
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		/*
		调用WSACleanup函数的目的：在Winsock DLL启动失败或想终止对Winsock DLL的使用时调用，释放资源。

		该函数的函数原型：int WSACleanup(void);

		返回值：若函数成功调用则返回0,否则返回错误信息.
		*/
		WSACleanup();
		return -1;
	}
	if (HIBYTE(wsaData.wVersion != 2) || LOBYTE(wsaData.wVersion) != 2)//检查启动的Winsock版本与之前设置的Winsock版本是否一致
	{
		WSACleanup();
		return -1;
	}

	/*
	调用socket函数的目的：创建一个套接字

	该函数的函数原型：SOCKET socket(int af,int type,int protocol);

	af：用于指定网络地址类型,一般取AF_INET,表示该套接字在Internet域中进行通信

	type:用于指定套接字类型，若取值为SOCK_STREAM表示要创建的套接字为流套接字，而取值为SOCK_DGRAM则表示要创建数据报套接字

	protocol:用于指定网络协议类型，一般取值为0，表示为TCP/IP协议

	返回值：若创建成功则返回所创建的套接字句柄SOCKET,否则将产生INVALID_SOCKET错误
	*/
	SOCKET socketCli = socket(AF_INET, SOCK_STREAM, 0);//创建客户端流套接字
	if (INVALID_SOCKET == socketCli)//创建不成功
	{
		/*
		调用closesocket函数的目的：关闭套接字以释放套接字所占用的资源，但该函数调用可能会导致数据的丢失
		（可以在调用该函数之前调用int shutdown(SOCKET s,int how)函数来中断连接）
		该函数的函数原型：int closesocket(SOCKET s);

		s:要关闭的套接字

		返回值：若函数成功调用则返回0,否则返回错误信息.
		*/
		closesocket(socketCli);
		WSACleanup();
		return -1;
	}

	/*
	sockaddr与sockaddr_in结构体的比较：sockaddr地址结构随所选择的网络协议的不同而变化，而sockaddr_in结构标志TCP/IP
	协议下的地址，可以通过强制类型转换把sockaddr_in结构转换成为sockaddr结构

	struct sockaddr{
	u_short  sa_family;
	char sa_data[14];
	};

	struct sockaddr_in{
	short      sin_family;//必须设为AF_Inet,表示该socket处于Internet域
	u_short      sin_port;//用于指定端口号（注意要将主机字节顺序转换为网络字节顺序，通过u_short htons(int port)函数）
	struct in_addr    sin_addr;//用于把一个ip地址保存为一个4字节的无符号长整数类型（使用struct in_addr inet_addr(char[] ip)函数将点式ip转换为32位的无符号长整数类型）
	char    sin_zero[8];//充当填充项的角色，以使得sockaddr_in结构与sockaddr结构的长度保持一致（为了强制类型转换的顺利进行）
	};
	*/
	sockaddr_in addrSer;//服务器地址（协议、端口号、ip地址）

	char ip[64];
	int port;
	cout << "请输入ip地址\n";
	cin >> ip;//输入的是点式ip
	cout << "请输入端口号\n";
	cin >> port;//主机字节顺序的端口号
	addrSer.sin_addr.S_un.S_addr =inet_addr(ip);
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(port);

	char sendBuf[100];//发送缓冲区
	char recvBuf[100];//接受缓冲区
	ZeroMemory(recvBuf, 100);//ZeroMemory是美国微软公司的软件开发包SDK中的一个宏。 其作用是用0来填充一块内存区域。
	ZeroMemory(sendBuf, 100);

	/*
	调用connect函数的目的：客户端向服务器发出连接请求，建立一个端到端的连接。

	该函数的函数原型：int connect(SOCKET s,const struct sockaddr FAR *name,int nameLen);

	s:表示一个未连接的数据报或流套接字的描述字（客户端进程自身创建的套接字）

	name:针对TCP/IP的套接字地址结构，标识服务进程的ip地址和端口号信息(FAR表示是远指针)

	nameLen:标识name参数的长度

	返回值：若函数成功调用则返回0,
	若将要连接的计算机没有监听指定端口的这一进程则返回错误信息WSAECONREFUSEED,
	若连接超时则返回WSAETIMEOUT信息.
	*/
	if (connect(socketCli, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) != 0)//客户端向服务器发出连接请求
	{
		cout << "未连接服务器" << endl;
		closesocket(socketCli);

		WSACleanup();
		system("pause");
		return 0;
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvProc, (void*)socketCli, 0, NULL);//创建接收进程
	while (true)
	{
		ZeroMemory(sendBuf, 100);//清空发送缓冲区
		cin >> sendBuf;//从控制台输入要发送的数据

					   /*
					   调用send函数的目的：发送数据

					   该函数的函数原型：int send(SOCKET s,const char FAR *buf,int len,int flags);

					   s:表示一个要发送数据的套接字的描述字（客户端进程自身创建的套接字）

					   buf:包含要发送数据的字符缓冲区

					   len:即将发送的缓冲区内的字符数

					   flags: 0：表示无特殊行为
					   MSG_DONTROUTE：表示要求传输层不要将它发出去的数据包路由出去
					   MSG_OOB：数据应该被带外发送（紧急数据）

					   返回值：成功发送则返回发送数据的字节数，若发生错误就返回错误信息SOCKET_ERROR
					   */
		send(socketCli, sendBuf, strlen(sendBuf), 0);
	}

	closesocket(socketCli);

	WSACleanup();
	system("pause");
	return 0;
}
