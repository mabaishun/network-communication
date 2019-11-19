
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string.h>
#include <vector>
//#include "OSInfo.h"



enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};


//��Ϣͷ
struct DataHeader
{
	short dataLength;	//���ݳ���
	short cmd;			//����
};
//��Ϣ��
struct Login:public DataHeader
{
	Login(){
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
//�����Ƿ��ͳɹ�
struct LoginResult: public DataHeader
{
	LoginResult() :result(0) {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
	}
	int result;
};
//�ǳ�
struct LogOut: public DataHeader
{
	LogOut() {
		dataLength = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
//�ǳ����
struct LogOutResult: public DataHeader
{
	LogOutResult() :result(0){
		dataLength = sizeof(LogOutResult);
		cmd = CMD_LOGOUT_RESULT;
	}
	int result;
};

std::vector<SOCKET> g_clients;

int processor(SOCKET accsock)
{
	char msgbuf[] = "Hello,I'm Server.";
	DataHeader dh{};
	int leng = sizeof(DataHeader);
	memset(&dh, 0, sizeof(DataHeader));
	//5 ���ܿͻ��˵�����
	int nlen = recv(accsock, (char*)&dh, sizeof(DataHeader), 0);
	std::cout << "debug:���ܵ���ָ��: " << dh.cmd
		<< "����:" << dh.dataLength << std::endl;
	if (nlen <= 0)
	{
		std::cout << "�ͻ��˹ر�" << std::endl;
		return -1;
	}
	switch (dh.cmd)
	{
	case CMD_LOGIN:
	{
		Login login{};
		recv(accsock, (char*)&login + leng, sizeof(Login) - leng, 0);

		std::cout << "�û�����" << login.userName << "\n"
			<< "���룺" << login.passWord << std::endl;

		LoginResult logret{};
		send(accsock, (char*)&logret, sizeof(logret), 0);
	}
	break;
	case CMD_LOGOUT:
	{
		LogOut logout{};
		recv(accsock, (char*)&logout + leng, sizeof(LogOut) - leng, 0);

		std::cout << "�û� " << logout.userName << " �ǳ�" << std::endl;
		//�����ж��û�������
		LogOutResult logret{  };
		send(accsock, (char*)&logret, sizeof(logret), 0);
	}
	break;
	default:
		dh.cmd = CMD_ERROR;
		dh.dataLength = 0;
		std::cout << "���ܽ�����ָ��" << std::endl;
		break;
	}
	return 1;
}

int main()
{
	//����windows socket 2.x ����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	///

	//OSInfo info;
	//���� ��Socket API �������� TCP �����
	//	1. ����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "�����׽��ֽ���ʧ��" << std::endl;
		exit(1);
	}
	//	2. �󶨽��ܿͻ������ӵĶ˿� ����bind
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);	//host to net usingned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))
	{
		std::cout << "ERROR,�׽��ְ�����˿�ʧ��" << std::endl;
		exit(1);
	}
	else {
		std::cout << "������˿ڳɹ�" << std::endl;
	}
	//	3. ��������˿� ����listen
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		std::cout << "��������˿�ʧ��" << std::endl;
		exit(1);
	}
	else {
		std::cout << "��������˿ڳɹ�" << std::endl;
	}
	//	4. �ȴ����ܿͻ������� ����accept
	sockaddr_in _clientAddr;
	int len = sizeof(_clientAddr);

	

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexce;

	SOCKET accsock;
	while (true)
	{

		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexce);

		FD_SET(_sock,&fdread);
		FD_SET(_sock, &fdwrite);
		FD_SET(_sock, &fdexce);

		for (size_t i = 0; i < g_clients.size(); i++)
		{
			FD_SET(g_clients[i], &fdread);
		}
		timeval time{ 1,0 };
		int ret = select(_sock + 1,&fdread,&fdwrite,&fdexce,&time);
		if (ret < 0)
		{
			std::cout << "select������������" << std::endl;
			exit(1);
		}

		if (FD_ISSET(_sock, &fdread))
		{
			FD_CLR(_sock,&fdread);
			accsock = accept(_sock, (sockaddr*)&_clientAddr, &len);
			if (INVALID_SOCKET == accsock)
			{
				std::cout << "��������ʧ��" << std::endl;
				exit(1);
			}
			std::cout << "�¿ͻ��˼��룬IP = " << inet_ntoa(_clientAddr.sin_addr) << std::endl;
			g_clients.push_back(accsock);
		}

		for (size_t i = 0; i < fdread.fd_count; i++)
		{
			if (-1 == processor(fdread.fd_array[i]))
			{
				auto iter = find(g_clients.begin(), g_clients.end(), fdread.fd_array[i]);
				if (iter != g_clients.end())
				{
					g_clients.erase(iter);
				}
			}
		}

		std::cout << "����ʱ�䴦������ҵ��" << std::endl;
	}
	for (size_t i = 0;i <  g_clients.size(); i++)
	{
		FD_SET(g_clients[i], &fdread);
		closesocket(g_clients[i]);
	}
	//	6. �ر� socket  ����closesocket
	closesocket(_sock);
	///

	//��� Windows socket ����
	WSACleanup();
	return 0;
}

