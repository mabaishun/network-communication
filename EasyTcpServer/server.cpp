
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string.h>
#include "OSInfo.h"



int main()
{
	//启动windows socket 2.x 环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	///

	OSInfo info;
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
	char _recvBuf[128]{0};
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
			memset(_recvBuf, 0, sizeof(_recvBuf));
			//5 接受客户端的请求
			int nlen = recv(accsock, _recvBuf, 128, 0);
			_recvBuf[strlen(_recvBuf)] = '\0';
			std::cout << strlen(_recvBuf) << std::endl;
			std::cout << "debug:接受到的指令:" << _recvBuf << std::endl;
			std::cout << "strcmp(_recvBuf, \"getName\"):" << strcmp(_recvBuf, "getName") << std::endl;
			if (nlen <= 0)
			{
				std::cout << "客户端已退出，任务结束" << std::endl;
				break;
			}
			//6 处理请求
			else if (0 == strcmp(_recvBuf, "version"))
			{
				std::string ver = info.getOsInfo();
				send(accsock, ver.c_str(), ver.length() + 1, 0);
			}
			else if (0 == strcmp(_recvBuf, "cpu"))
			{
				std::string cpu = info.getCpuInfo();
				send(accsock, cpu.c_str(), cpu.length() + 1, 0);
			}
			else if (0 == strcmp(_recvBuf, "memory"))
			{
				std::string memory = info.getMemoryInfo();
				send(accsock, memory.c_str(), memory.length() + 1, 0);
			}
			else if (0 == strcmp(_recvBuf, "hard"))
			{
				std::string hard = info.getHardDiskInfo();
				send(accsock, hard.c_str(), hard.length() + 1, 0);
			}
			else if (0 == strcmp(_recvBuf, "network"))
			{
				std::string network = info.getNetworkInfo();
				std::cout << network << std::endl;
				send(accsock, network.c_str(), network.length() + 1, 0);
			}
			else if (0 == strcmp(_recvBuf, "process"))
			{
				std::string pro = info.getProcessInfo();
				send(accsock, pro.c_str(), pro.length() + 1, 0);
			}
			else
			{
				strcpy_s(msgbuf,"不能解析的指令");
				send(accsock, msgbuf, strlen(msgbuf) + 1, 0);
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

