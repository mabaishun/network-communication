#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <iostream>
#include <iostream>
#include <string>


int main()
{
	//����windows socket 2.x ����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	///
	char _cmdBuf[128]{ 0 };

	//���� �� Socket API �������� TCP �ͻ�
	//	1. ����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "�����׽��ֽ���ʧ��" << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "����socket�ɹ�" << std::endl;
	}
	//	2. ���ӷ����� ����connect
	sockaddr_in _sin{};
	char recvBuf[256]{0};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "��������Socketʧ��" << std::endl;
	}
	else
	{
		std::cout << "���ӷ������ɹ�" << std::endl;
	}
	
	while (true)
	{
		//3 ��������ָ��
		memset(_cmdBuf, 0, sizeof(_cmdBuf));
		fflush(stdin);
		scanf("%s", _cmdBuf);
		_cmdBuf[strlen(_cmdBuf)] = '\0';
		//4 ��������
		if (0 == strcmp(_cmdBuf,"exit"))
		{
			break;
		}
		else
		{
			//5 ���������������ָ��
			send(_sock, _cmdBuf, strlen(_cmdBuf), 0);
		}
		//	6 ���ܷ�������Ϣ ����recv
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
			std::cout << recvBuf << std::endl;
	}

	//	7 �ر�socket ����closesocket
	closesocket(_sock);
	///

	//��� Windows socket ����
	WSACleanup();
	return 0;
}