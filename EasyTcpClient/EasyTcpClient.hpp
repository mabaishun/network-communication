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
	//虚析构函数，保证父类析构的时候能够执行到子类的析构函数
	virtual ~EasyTcpClient();

	//初始化socket
	int initSocket();

	//连接服务器
	int Connect(const char* ip, short port);

	//关闭Socket
	void Close();

	//发送数据
	int Send(DataHeader* dh);

	//接受数据
	int Recv();
	//响应
	void OnNetMsg(DataHeader* header);

	//查询网络消息
	bool OnRun();
	//是否运行
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

//初始化socket
inline int EasyTcpClient::initSocket()
{
	//启动windows socket 2.x 环境
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif
	if (_sock != INVALID_SOCKET)
	{
		std::cout << "关闭之前的连接" << std::endl;
		Close();
	}
	//—— 用 Socket API 建立简易 TCP 客户
//    1. 建立一个socket
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "错误，套接字建立失败" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "建立socket成功" << std::endl;
		return 0;
	}
}

//连接
inline int EasyTcpClient::Connect(const char* ip, short port)
{
	if (INVALID_SOCKET == _sock)
	{
		initSocket();
	}
	//    2. 连接服务器 ——connect
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
		std::cout << "错误，连接Socket失败" << std::endl;
	}
	else
	{
		std::cout << "连接服务器成功" << std::endl;
	}
	return ret;
}

//关闭
inline void EasyTcpClient::Close()
{
	if (_sock != INVALID_SOCKET)
	{
		//    7 关闭socket ——closesocket
#ifdef _WIN32
		closesocket(_sock);
		//清除 Windows socket 环境
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

//接受数据，处理粘包 拆分包
inline int EasyTcpClient::Recv()
{
	char msgbuf[128]{};


	//5 接受客户端的请求
	size_t nlen = recv(_sock, msgbuf, sizeof(DataHeader), 0);
	DataHeader* dh = (DataHeader*)msgbuf;
	std::cout << "debug:接受到的指令: " << dh->cmd
		<< "长度:" << dh->dataLength << std::endl;
	if (nlen <= 0)
	{
		Close();
		std::cout << "与服务器断开连接，任务结束" << std::endl;
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
		std::cout << "收到服务器消息：CMD_LOGIN_RESULT：数据长度" << ret->dataLength << std::endl;

	}
	break;
	case CMD_LOGOUT_RESULT:
	{
		LogOutResult* ret = (LogOutResult*)dh;
		std::cout << "收到服务器消息：CMD_LOGOUT_RESULT：数据长度" << ret->dataLength << std::endl;

	}
	break;
	case CMD_NEW_USER_JOIN:
	{
		NewUser* ret = (NewUser*)dh;
		std::cout << "收到服务器消息：CMD_NEW_USER_JOIN：数据长度" << ret->dataLength << std::endl;

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
			std::cout << _sock << " select任务结束" << std::endl;
			Close();
			return false;
		}
		if (FD_ISSET(_sock, &fdreads))
		{
			FD_CLR(_sock, &fdreads);

			if (-1 == Recv())
			{
				std::cout << "select任务结束" << std::endl;
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

