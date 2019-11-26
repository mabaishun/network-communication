#ifndef __EASYTCPCLIENT_HPP__
#define __EASYTCPCLIENT_HPP__

#include "messageHeader.hpp"



#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define SOCKET int
#define INVALID_SOCKET SOCKET(~0)
#define SOCKET_ERROR    (-1)

#endif


#include <iostream>
#include <iostream>
#include <string>
#include <thread>


class EasyTcpClient
{
public:
	EasyTcpClient();
	//��������������֤����������ʱ���ܹ�ִ�е��������������
	virtual ~EasyTcpClient();

	//��ʼ��socket
	int initSocket();

	//���ӷ�����
	int Connect(const char* ip, short port);

	//�ر�Socket
	void Close();

	//��������
	int Send(DataHeader* dh);

	//��������
	int Recv();
	//��Ӧ
	void OnNetMsg(DataHeader* header);

	//��ѯ������Ϣ
	bool OnRun();
	//�Ƿ�����
	bool isRun();
private:
	SOCKET _sock;
};

EasyTcpClient::EasyTcpClient()
{
	_sock = INVALID_SOCKET;
}

EasyTcpClient::~EasyTcpClient()
{
	Close();
}

//��ʼ��socket
inline int EasyTcpClient::initSocket()
{
	//����windows socket 2.x ����
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif
	if (_sock != INVALID_SOCKET)
	{
		std::cout << "�ر�֮ǰ������" << std::endl;
		Close();
	}
	//���� �� Socket API �������� TCP �ͻ�
//    1. ����һ��socket
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "�����׽��ֽ���ʧ��" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "����socket�ɹ�" << std::endl;
		return 0;
	}
}

//����
inline int EasyTcpClient::Connect(const char* ip, short port)
{
	if (INVALID_SOCKET == _sock)
	{
		initSocket();
	}
	//    2. ���ӷ����� ����connect
	sockaddr_in _sin{};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	_sin.sin_addr.s_addr = inet_addr(ip);
#endif

	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "��������Socketʧ��" << std::endl;
	}
	else
	{
		std::cout << "���ӷ������ɹ�" << std::endl;
	}
	return ret;
}

//�ر�
inline void EasyTcpClient::Close()
{
	if (_sock != INVALID_SOCKET)
	{
		//    7 �ر�socket ����closesocket
#ifdef _WIN32
		closesocket(_sock);
		//��� Windows socket ����
		WSACleanup();
#else
		close(_sock);
#endif
		_sock = INVALID_SOCKET;
	}
}

inline int EasyTcpClient::Send(DataHeader* dh)
{
	if (isRun() && dh)
	{
		return send(_sock, (const char*)dh, dh->dataLength, 0);
	}
	Close();
	return SOCKET_ERROR;
}

//�������ݣ�����ճ�� ��ְ�
inline int EasyTcpClient::Recv()
{
	char msgbuf[128]{};


	//5 ���ܿͻ��˵�����
	size_t nlen = recv(_sock, msgbuf, sizeof(DataHeader), 0);
	DataHeader* dh = (DataHeader*)msgbuf;
	std::cout << "debug:���ܵ���ָ��: " << dh->cmd
		<< "����:" << dh->dataLength << std::endl;
	if (nlen <= 0)
	{
		Close();
		std::cout << "��������Ͽ����ӣ��������" << std::endl;
	}
	recv(_sock, msgbuf + sizeof(DataHeader), dh->dataLength - sizeof(DataHeader), 0);

	OnNetMsg(dh);
	return 0;
}

inline void EasyTcpClient::OnNetMsg(DataHeader * dh)
{
	std::cout << "dh->cmd: " << dh->cmd << std::endl;
	switch (dh->cmd)
	{
	case CMD_LOGIN_RESULT:
	{
		LoginResult* ret = (LoginResult*)dh;
		std::cout << "�յ���������Ϣ��CMD_LOGIN_RESULT�����ݳ���" << ret->dataLength << std::endl;

	}
	break;
	case CMD_LOGOUT_RESULT:
	{
		LogOutResult* ret = (LogOutResult*)dh;
		std::cout << "�յ���������Ϣ��CMD_LOGOUT_RESULT�����ݳ���" << ret->dataLength << std::endl;

	}
	break;
	case CMD_NEW_USER_JOIN:
	{
		NewUser* ret = (NewUser*)dh;
		std::cout << "�յ���������Ϣ��CMD_NEW_USER_JOIN�����ݳ���" << ret->dataLength << std::endl;

	}
	break;
	default:
		std::cout << "default " << std::endl;
		break;
	}
}

inline bool EasyTcpClient::OnRun()
{
	if (isRun())
	{

		fd_set fdreads;
		FD_ZERO(&fdreads);
		FD_SET(_sock, &fdreads);
		timeval t{ 1,0 };
		int ret = select(_sock + 1, &fdreads, nullptr, nullptr, nullptr);

		if (ret < 0)
		{
			std::cout << _sock << " select�������" << std::endl;
			Close();
			return false;
		}
		if (FD_ISSET(_sock, &fdreads))
		{
			FD_CLR(_sock, &fdreads);

			if (-1 == Recv())
			{
				std::cout << "select�������" << std::endl;
				return false;
			}
		}
		return true;
	}
	return false;
}

inline bool EasyTcpClient::isRun()
{
	return _sock != INVALID_SOCKET;
}



#endif // !__EASYTCPCLIENT_HPP__

