#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32
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


enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR
};

//消息头
struct DataHeader
{
	short dataLength;	//数据长度
	short cmd;			//命令
};
//消息体
struct Login :public DataHeader
{
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
//数据是否发送成功
struct LoginResult : public DataHeader
{
	LoginResult() :result(0) {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
	}
	int result;
};
//登出
struct LogOut : public DataHeader
{
	LogOut() {
		dataLength = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
//登出结果
struct LogOutResult : public DataHeader
{
	LogOutResult() :result(0) {
		dataLength = sizeof(LogOutResult);
		cmd = CMD_LOGOUT_RESULT;
	}
	int result;
};
struct NewUser : public DataHeader
{
	NewUser() {
		dataLength = sizeof(DataHeader);
		cmd = CMD_NEW_USER_JOIN;
		result = 0;
	}
	int result;
};

bool g_bExit = false;


int processor(SOCKET _sock)
{
	char msgbuf[128]{};
	DataHeader dh{};
	int leng = sizeof(DataHeader);
	memset(&dh, 0, sizeof(DataHeader));
	//5 接受客户端的请求
	int nlen = recv(_sock, (char*)&dh, sizeof(DataHeader), 0);
	std::cout << "debug:接受到的指令: " << dh.cmd
		<< "长度:" << dh.dataLength << std::endl;
	if (nlen <= 0)
	{
		g_bExit = true;
		std::cout << "与服务器断开连接，任务结束" << std::endl;
	}
	switch (dh.cmd)
	{
	case CMD_LOGIN_RESULT:
	{
		recv(_sock, msgbuf, dh.dataLength - sizeof(DataHeader), 0);
		LoginResult *ret = (LoginResult*)msgbuf;
		std::cout << "收到客户端消息：CMD_LOGIN_RESULT：数据长度" << ret->dataLength << std::endl;

	}
	break;
	case CMD_LOGOUT_RESULT:
	{
		recv(_sock, msgbuf, dh.dataLength - sizeof(DataHeader), 0);
		LogOutResult *ret = (LogOutResult*)msgbuf;
		std::cout << "收到客户端消息：CMD_LOGOUT_RESULT：数据长度" << ret->dataLength << std::endl;

	}
	break;
	case CMD_NEW_USER_JOIN:
	{
		recv(_sock, msgbuf, dh.dataLength - sizeof(DataHeader), 0);
		NewUser *ret = (NewUser*)msgbuf;
		std::cout << "收到客户端消息：CMD_NEW_USER_JOIN：数据长度" << ret->dataLength << std::endl;

	}
	break;
	default:
		std::cout << "default " << std::endl;
	}
	return 0;
}
void cmdThread(SOCKET _sock)
{
	std::string cmd;
	while (std::cin >> cmd)
	{
		if (cmd == "exit")
		{
			std::cout << "线程退出" << std::endl;
			exit(1);
		}
		else if (cmd == "login")
		{
			Login login{  };
			strcpy(login.userName, "admin");
			strcpy(login.passWord, "12345");
			send(_sock, (const char*)&login, sizeof(Login), 0);
			//Sleep(500);
		}
		else if (cmd == "logout")
		{
			LogOut logout{  };
			strcpy(logout.userName, "admin");
			send(_sock, (const char*)&logout, sizeof(LogOut), 0);
			//Sleep(500);
		}
		else
		{
			std::cout << "不支持的命令" << std::endl;
		}
	}
}

int main()
{
	//启动windows socket 2.x 环境
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif

	///
	char _cmdBuf[128]{ 0 };

	//—— 用 Socket API 建立简易 TCP 客户
	//	1. 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "错误，套接字建立失败" << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "建立socket成功" << std::endl;
	}
	//	2. 连接服务器 ——connect
	sockaddr_in _sin{};
	char recvBuf[256]{ 0 };
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
	_sin.sin_addr.s_addr = inet_addr("192.168.6.89");
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
	fd_set fdreads;
	//启动线程
	std::thread td(cmdThread, _sock);
	td.detach();
	while (!g_bExit)
	{
		FD_ZERO(&fdreads);
		FD_SET(_sock, &fdreads);
		timeval t{ 1,0 };
		int ret = select(_sock + 1, &fdreads, nullptr, nullptr, &t);

		if (ret < 0)
		{
			std::cout << "select任务结束" << std::endl;
			break;
		}
		if (FD_ISSET(_sock, &fdreads))
		{
			FD_CLR(_sock, &fdreads);

			if (-1 == processor(_sock))
			{
				std::cout << "select任务结束" << std::endl;
				break;
			}
		}
	}

	//	7 关闭socket ——closesocket
#ifdef _WIN32
	closesocket(_sock);
#else
	close(_sock);
#endif

	///

	//清除 Windows socket 环境
#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}