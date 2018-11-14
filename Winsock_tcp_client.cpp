// ��������_�ͻ���.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
//#include <Ws2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

//���շ��������������ݵĽ���
void recvProc(SOCKET socketSer)
{
	char msgRcv[100] = { 0 };
	while (true)
	{
		/*
		����recv������Ŀ�ģ����ܷ�����̷���������

		�ú����ĺ���ԭ�ͣ�int recv(SOCKET s,char FAR *buf,int len,int flags);

		s:��ʾһ��Ҫ�������ݵ��׽��ֵ������֣��ͻ��˽������������׽��֣�

		buf:�������յ����ݵ��ַ�������

		len:׼�����յ��ַ�����buf����ĳ���

		flags: 0����ʾ��������Ϊ
		MSG_PEEk����ʾʹ���õ����ݸ��Ƶ����ṩ�Ľ��ջ������ڣ���û�д�ϵͳ�������н���ɾ��
		MSG_OOB�����մ������ݣ��������ݣ�

		����ֵ���������ɹ������򷵻�0,���򷵻ش�����Ϣ.
		*/
		if (SOCKET_ERROR == recv(socketSer, msgRcv, sizeof(msgRcv), 0))
		{
			cout << "�������ѹر�" << endl;
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
	����WSAStartup������Ŀ�ģ���Winsock DLL���г�ʼ����Winsock�ķ������Զ�̬���ӿ�Winsock DLL����ʽʵ�ֵģ���
	��ȡWinsock�İ汾֧�֣��������Ҫ����Դ��

	�ú����ĺ���ԭ�ͣ�int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData);

	wVersionRequested:����ָ��׼�����ص�Winsock��İ汾�������ú�MAKEWORD( X, Y) (XΪ��λ�ֽڣ�YΪ��λ�ֽ�)�����
	wVersionRequested����ȷֵ�����и�λ�ֽ�ָ�������Winsock��ĸ��汾,��λ�ֽ�ָ�����汾��

	lpWSAData:ָ��LPWSAData�ṹ��ָ�룬�ýṹ����������صĿ�汾�йص���Ϣ��
	typeDef struct WSAData{
	WORD      wVersion;//ϣ��ʹ�õ�Winsock��İ汾
	WORD      wHighVersion;//���е�Winsock�����߰汾
	. . . . . .
	}WSADATA,* LPWSADATA;

	����ֵ���������ɹ������򷵻�0,���򷵻ش�����Ϣ.
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		/*
		����WSACleanup������Ŀ�ģ���Winsock DLL����ʧ�ܻ�����ֹ��Winsock DLL��ʹ��ʱ���ã��ͷ���Դ��

		�ú����ĺ���ԭ�ͣ�int WSACleanup(void);

		����ֵ���������ɹ������򷵻�0,���򷵻ش�����Ϣ.
		*/
		WSACleanup();
		return -1;
	}
	if (HIBYTE(wsaData.wVersion != 2) || LOBYTE(wsaData.wVersion) != 2)//���������Winsock�汾��֮ǰ���õ�Winsock�汾�Ƿ�һ��
	{
		WSACleanup();
		return -1;
	}

	/*
	����socket������Ŀ�ģ�����һ���׽���

	�ú����ĺ���ԭ�ͣ�SOCKET socket(int af,int type,int protocol);

	af������ָ�������ַ����,һ��ȡAF_INET,��ʾ���׽�����Internet���н���ͨ��

	type:����ָ���׽������ͣ���ȡֵΪSOCK_STREAM��ʾҪ�������׽���Ϊ���׽��֣���ȡֵΪSOCK_DGRAM���ʾҪ�������ݱ��׽���

	protocol:����ָ������Э�����ͣ�һ��ȡֵΪ0����ʾΪTCP/IPЭ��

	����ֵ���������ɹ��򷵻����������׽��־��SOCKET,���򽫲���INVALID_SOCKET����
	*/
	SOCKET socketCli = socket(AF_INET, SOCK_STREAM, 0);//�����ͻ������׽���
	if (INVALID_SOCKET == socketCli)//�������ɹ�
	{
		/*
		����closesocket������Ŀ�ģ��ر��׽������ͷ��׽�����ռ�õ���Դ�����ú������ÿ��ܻᵼ�����ݵĶ�ʧ
		�������ڵ��øú���֮ǰ����int shutdown(SOCKET s,int how)�������ж����ӣ�
		�ú����ĺ���ԭ�ͣ�int closesocket(SOCKET s);

		s:Ҫ�رյ��׽���

		����ֵ���������ɹ������򷵻�0,���򷵻ش�����Ϣ.
		*/
		closesocket(socketCli);
		WSACleanup();
		return -1;
	}

	/*
	sockaddr��sockaddr_in�ṹ��ıȽϣ�sockaddr��ַ�ṹ����ѡ�������Э��Ĳ�ͬ���仯����sockaddr_in�ṹ��־TCP/IP
	Э���µĵ�ַ������ͨ��ǿ������ת����sockaddr_in�ṹת����Ϊsockaddr�ṹ

	struct sockaddr{
	u_short  sa_family;
	char sa_data[14];
	};

	struct sockaddr_in{
	short      sin_family;//������ΪAF_Inet,��ʾ��socket����Internet��
	u_short      sin_port;//����ָ���˿ںţ�ע��Ҫ�������ֽ�˳��ת��Ϊ�����ֽ�˳��ͨ��u_short htons(int port)������
	struct in_addr    sin_addr;//���ڰ�һ��ip��ַ����Ϊһ��4�ֽڵ��޷��ų��������ͣ�ʹ��struct in_addr inet_addr(char[] ip)��������ʽipת��Ϊ32λ���޷��ų��������ͣ�
	char    sin_zero[8];//�䵱�����Ľ�ɫ����ʹ��sockaddr_in�ṹ��sockaddr�ṹ�ĳ��ȱ���һ�£�Ϊ��ǿ������ת����˳�����У�
	};
	*/
	sockaddr_in addrSer;//��������ַ��Э�顢�˿ںš�ip��ַ��

	char ip[64];
	int port;
	cout << "������ip��ַ\n";
	cin >> ip;//������ǵ�ʽip
	cout << "������˿ں�\n";
	cin >> port;//�����ֽ�˳��Ķ˿ں�
	addrSer.sin_addr.S_un.S_addr =inet_addr(ip);
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(port);

	char sendBuf[100];//���ͻ�����
	char recvBuf[100];//���ܻ�����
	ZeroMemory(recvBuf, 100);//ZeroMemory������΢��˾�����������SDK�е�һ���ꡣ ����������0�����һ���ڴ�����
	ZeroMemory(sendBuf, 100);

	/*
	����connect������Ŀ�ģ��ͻ���������������������󣬽���һ���˵��˵����ӡ�

	�ú����ĺ���ԭ�ͣ�int connect(SOCKET s,const struct sockaddr FAR *name,int nameLen);

	s:��ʾһ��δ���ӵ����ݱ������׽��ֵ������֣��ͻ��˽������������׽��֣�

	name:���TCP/IP���׽��ֵ�ַ�ṹ����ʶ������̵�ip��ַ�Ͷ˿ں���Ϣ(FAR��ʾ��Զָ��)

	nameLen:��ʶname�����ĳ���

	����ֵ���������ɹ������򷵻�0,
	����Ҫ���ӵļ����û�м���ָ���˿ڵ���һ�����򷵻ش�����ϢWSAECONREFUSEED,
	�����ӳ�ʱ�򷵻�WSAETIMEOUT��Ϣ.
	*/
	if (connect(socketCli, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) != 0)//�ͻ����������������������
	{
		cout << "δ���ӷ�����" << endl;
		closesocket(socketCli);

		WSACleanup();
		system("pause");
		return 0;
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvProc, (void*)socketCli, 0, NULL);//�������ս���
	while (true)
	{
		ZeroMemory(sendBuf, 100);//��շ��ͻ�����
		cin >> sendBuf;//�ӿ���̨����Ҫ���͵�����

					   /*
					   ����send������Ŀ�ģ���������

					   �ú����ĺ���ԭ�ͣ�int send(SOCKET s,const char FAR *buf,int len,int flags);

					   s:��ʾһ��Ҫ�������ݵ��׽��ֵ������֣��ͻ��˽������������׽��֣�

					   buf:����Ҫ�������ݵ��ַ�������

					   len:�������͵Ļ������ڵ��ַ���

					   flags: 0����ʾ��������Ϊ
					   MSG_DONTROUTE����ʾҪ����㲻Ҫ��������ȥ�����ݰ�·�ɳ�ȥ
					   MSG_OOB������Ӧ�ñ����ⷢ�ͣ��������ݣ�

					   ����ֵ���ɹ������򷵻ط������ݵ��ֽ���������������ͷ��ش�����ϢSOCKET_ERROR
					   */
		send(socketCli, sendBuf, strlen(sendBuf), 0);
	}

	closesocket(socketCli);

	WSACleanup();
	system("pause");
	return 0;
}
