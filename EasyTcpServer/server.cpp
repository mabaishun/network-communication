
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
	//����windows socket 2.x ����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	///

	OSInfo info;
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
	char msgbuf[] = "Hello,I'm Server.";
	char _recvBuf[128]{0};
	while (true)
	{
		SOCKET accsock = accept(_sock, (sockaddr*)&_clientAddr, &len);
		if (INVALID_SOCKET == accsock)
		{
			std::cout << "��������ʧ��" << std::endl;
			exit(1);
		}
		std::cout << "�¿ͻ��˼��룬IP = " << inet_ntoa(_clientAddr.sin_addr) << std::endl;
		while (true)
		{
			memset(_recvBuf, 0, sizeof(_recvBuf));
			//5 ���ܿͻ��˵�����
			int nlen = recv(accsock, _recvBuf, 128, 0);
			_recvBuf[strlen(_recvBuf)] = '\0';
			std::cout << strlen(_recvBuf) << std::endl;
			std::cout << "debug:���ܵ���ָ��:" << _recvBuf << std::endl;
			std::cout << "strcmp(_recvBuf, \"getName\"):" << strcmp(_recvBuf, "getName") << std::endl;
			if (nlen <= 0)
			{
				std::cout << "�ͻ������˳����������" << std::endl;
				break;
			}
			//6 ��������
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
				strcpy_s(msgbuf,"���ܽ�����ָ��");
				send(accsock, msgbuf, strlen(msgbuf) + 1, 0);
			}
		}
		closesocket(accsock);
	}
	//	6. �ر� socket  ����closesocket
	closesocket(_sock);
	///

	//��� Windows socket ����
	WSACleanup();
	return 0;
}

