#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <iostream>
#include <iostream>
#include <string>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR
};


//��Ϣͷ
struct DataHeader
{
	short dataLength;	//���ݳ���
	short cmd;			//����
};
//��Ϣ��
struct Login :public DataHeader
{
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
//�����Ƿ��ͳɹ�
struct LoginResult : public DataHeader
{
	LoginResult() :result(0) {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
	}
	int result;
};
//�ǳ�
struct LogOut : public DataHeader
{
	LogOut() {
		dataLength = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
//�ǳ����
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

int processor(SOCKET _sock)
{
	char msgbuf[128]{};
	DataHeader dh{};
	int leng = sizeof(DataHeader);
	memset(&dh, 0, sizeof(DataHeader));
	//5 ���ܿͻ��˵�����
	int nlen = recv(_sock, (char*)&dh, sizeof(DataHeader), 0);
	std::cout << "debug:���ܵ���ָ��: " << dh.cmd
		<< "����:" << dh.dataLength << std::endl;
	if (nlen <= 0)
	{
		std::cout << "��������Ͽ����ӣ��������" << std::endl;
		return -1;
	}
	switch (dh.cmd)
	{
		case CMD_LOGIN_RESULT:
		{
			recv(_sock, msgbuf, dh.dataLength - sizeof(DataHeader),0);
			LoginResult *ret = (LoginResult*)msgbuf;
			std::cout << "�յ��ͻ�����Ϣ��CMD_LOGIN_RESULT�����ݳ���" << ret->dataLength << std::endl;

		}
		break;
		case CMD_LOGOUT_RESULT:
		{
			recv(_sock, msgbuf, dh.dataLength - sizeof(DataHeader), 0);
			LogOutResult *ret = (LogOutResult*)msgbuf;
			std::cout << "�յ��ͻ�����Ϣ��CMD_LOGOUT_RESULT�����ݳ���" << ret->dataLength << std::endl;

		}
		break;
		case CMD_NEW_USER_JOIN:
		{
			recv(_sock, msgbuf, dh.dataLength - sizeof(DataHeader), 0);
			NewUser *ret = (NewUser*)msgbuf;
			std::cout << "�յ��ͻ�����Ϣ��CMD_NEW_USER_JOIN�����ݳ���" << ret->dataLength << std::endl;

		}
		break;
		default:
			std::cout << "default " << std::endl;
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
	FD_SET fdreads;


	while (true)
	{
		FD_ZERO(&fdreads);
		FD_SET(_sock, &fdreads);
		timeval t{ 1,0 };
		int ret = select(_sock,&fdreads,nullptr,nullptr,&t);

		if (ret < 0)
		{
			std::cout << "select�������" << std::endl;
			break;
		}
		if (FD_ISSET(_sock, &fdreads))
		{
			FD_CLR(_sock, &fdreads);

			if (-1 == processor(_sock))
			{
				std::cout << "select�������" << std::endl;
				break;
			}
		}

		////4 ��������
		//if (0 == strcmp(_cmdBuf,"exit"))
		//{
		//	break;
		//}
		//else if(0 == strcmp(_cmdBuf,"login"))
		//{
		//	//5 ���������������ָ��
		//	Login login{  };
		//	strcpy(login.userName, "admin");
		//	strcpy(login.passWord, "12345");
		//	send(_sock, (const char*)&login, sizeof(Login), 0);
		//	//���ܷ��������ص�����
		//	LoginResult logres{ };
		//	recv(_sock, (char*)&logres, sizeof(LoginResult), 0);
		//	std::cout << "LoginResult: " << logres.result << std::endl;

		//}
		//else if (0 == strcmp(_cmdBuf, "logout"))
		//{
		//	LogOut logout{ };
		//	strcpy(logout.userName, "admin");
		//	send(_sock, (const char*)&logout, sizeof(LogOut), 0);
		//	
		//	DataHeader header{};
		//	LogOutResult  logres{};
		//	recv(_sock, (char*)&logres, sizeof(LogOutResult), 0);
		//	std::cout << "LogOutResult: " << logres.result << std::endl;
		//}
		//else
		//{
		//	std::cout << "��֧�ֵ�����" << std::endl;
		//}

		Login login{  };
		strcpy(login.userName, "admin");
		strcpy(login.passWord, "12345");
		send(_sock, (const char*)&login, sizeof(Login), 0);
		Sleep(500);

		std::cout << "����ʱ�䴦������ҵ��" << std::endl;

	}

	//	7 �ر�socket ����closesocket
	closesocket(_sock);
	///

	//��� Windows socket ����
	WSACleanup();
	return 0;
}