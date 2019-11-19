
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string.h>
//#include "OSInfo.h"



enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};


//消息头
struct DataHeader
{
	short dataLength;	//数据长度
	short cmd;			//命令
};
//消息体
struct Login:public DataHeader
{
	Login(){
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
//数据是否发送成功
struct LoginResult: public DataHeader
{
	LoginResult() :result(0) {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
	}
	int result;
};
//登出
struct LogOut: public DataHeader
{
	LogOut() {
		dataLength = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
//登出结果
struct LogOutResult: public DataHeader
{
	LogOutResult() :result(0){
		dataLength = sizeof(LogOutResult);
		cmd = CMD_LOGOUT_RESULT;
	}
	int result;
};



int main()
{
	//启动windows socket 2.x 环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	///

	//OSInfo info;
	//—— 用Socket API 建立简易 TCP 服务端
	//	1. 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "错误，套接字建立失败" << std::endl;
		exit(1);
	}
	//	2. 绑定接受客户端连接的端口 ——bind
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);	//host to net usingned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))
	{
		std::cout << "ERROR,套接字绑定网络端口失败" << std::endl;
		exit(1);
	}
	else {
		std::cout << "绑定网络端口成功" << std::endl;
	}
	//	3. 监听网络端口 ——listen
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		std::cout << "监听网络端口失败" << std::endl;
		exit(1);
	}
	else {
		std::cout << "监听网络端口成功" << std::endl;
	}
	//	4. 等待接受客户端连接 ——accept
	sockaddr_in _clientAddr;
	int len = sizeof(_clientAddr);
	char msgbuf[] = "Hello,I'm Server.";
	DataHeader dh{};
	int leng = sizeof(DataHeader);

	while (true)
	{
		SOCKET accsock = accept(_sock, (sockaddr*)&_clientAddr, &len);
		if (INVALID_SOCKET == accsock)
		{
			std::cout << "建立连接失败" << std::endl;
			exit(1);
		}
		std::cout << "新客户端加入，IP = " << inet_ntoa(_clientAddr.sin_addr) << std::endl;
		while (true)
		{
			memset(&dh, 0, sizeof(DataHeader));
			//5 接受客户端的请求
			int nlen = recv(accsock, (char*)&dh,sizeof(DataHeader) , 0);
			std::cout << "debug:接受到的指令: " << dh.cmd 
				 << "长度:" << dh.dataLength << std::endl;
			if (nlen <= 0)
			{
				std::cout << "客户端关闭" << std::endl;
				break;
			}
			switch (dh.cmd)
			{
			case CMD_LOGIN:
			{
				Login login{};
				recv(accsock, (char*)&login + leng, sizeof(Login) - leng, 0);
				
				std::cout << "用户名：" << login.userName << "\n"
					<< "密码：" << login.passWord << std::endl;

				LoginResult logret{};
				send(accsock, (char*)&logret, sizeof(logret),0);
			}
			break;
			case CMD_LOGOUT:
			{
				LogOut logout{};
				recv(accsock, (char*)&logout +leng, sizeof(LogOut) - leng, 0);

				std::cout << "用户 " << logout.userName << " 登出" << std::endl;
				//忽略判断用户名密码
				LogOutResult logret{  };
				send(accsock, (char*)&logret, sizeof(logret), 0);
			}
			break;
			default:
				dh.cmd = CMD_ERROR;
				dh.dataLength = 0;
				std::cout << "不能解析的指令" << std::endl;
				break;
			}
		}
		closesocket(accsock);
	}
	//	6. 关闭 socket  ——closesocket
	closesocket(_sock);
	///

	//清除 Windows socket 环境
	WSACleanup();
	return 0;
}

