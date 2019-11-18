#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <iostream>
#include <iostream>
#include <string>


int main()
{
	//启动windows socket 2.x 环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
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
	char recvBuf[256]{0};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "错误，连接Socket失败" << std::endl;
	}
	else
	{
		std::cout << "连接服务器成功" << std::endl;
	}
	
	while (true)
	{
		//3 输入请求指令
		memset(_cmdBuf, 0, sizeof(_cmdBuf));
		fflush(stdin);
		scanf("%s", _cmdBuf);
		_cmdBuf[strlen(_cmdBuf)] = '\0';
		//4 处理请求
		if (0 == strcmp(_cmdBuf,"exit"))
		{
			break;
		}
		else
		{
			//5 向服务器发送请求指令
			send(_sock, _cmdBuf, strlen(_cmdBuf), 0);
		}
		//	6 接受服务器信息 ——recv
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
			std::cout << recvBuf << std::endl;
	}

	//	7 关闭socket ——closesocket
	closesocket(_sock);
	///

	//清除 Windows socket 环境
	WSACleanup();
	return 0;
}