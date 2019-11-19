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
		std::cout << "_cmdBuf:" << _cmdBuf << std::endl;
		//4 ��������
		if (0 == strcmp(_cmdBuf,"exit"))
		{
			break;
		}
		else if(0 == strcmp(_cmdBuf,"login"))
		{
			//5 ���������������ָ��
			Login login{  };
			strcpy(login.userName, "admin");
			strcpy(login.passWord, "12345");
			send(_sock, (const char*)&login, sizeof(Login), 0);
			//���ܷ��������ص�����
			LoginResult logres{ };
			recv(_sock, (char*)&logres, sizeof(LoginResult), 0);
			std::cout << "LoginResult: " << logres.result << std::endl;

		}
		else if (0 == strcmp(_cmdBuf, "logout"))
		{
			LogOut logout{ };
			strcpy(logout.userName, "admin");
			send(_sock, (const char*)&logout, sizeof(LogOut), 0);
			
			DataHeader header{};
			LogOutResult  logres{};
			recv(_sock, (char*)&logres, sizeof(LogOutResult), 0);
			std::cout << "LogOutResult: " << logres.result << std::endl;
		}
		else
		{
			std::cout << "��֧�ֵ�����" << std::endl;
		}

	}

	//	7 �ر�socket ����closesocket
	closesocket(_sock);
	///

	//��� Windows socket ����
	WSACleanup();
	return 0;
}