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
		std::cout << "_cmdBuf:" << _cmdBuf << std::endl;
		//4 处理请求
		if (0 == strcmp(_cmdBuf,"exit"))
		{
			break;
		}
		else if(0 == strcmp(_cmdBuf,"login"))
		{
			//5 向服务器发送请求指令
			Login login{  };
			strcpy(login.userName, "admin");
			strcpy(login.passWord, "12345");
			send(_sock, (const char*)&login, sizeof(Login), 0);
			//接受服务器返回的数据
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
			std::cout << "不支持的命令" << std::endl;
		}

	}

	//	7 关闭socket ——closesocket
	closesocket(_sock);
	///

	//清除 Windows socket 环境
	WSACleanup();
	return 0;
}