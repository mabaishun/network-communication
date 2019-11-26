

#include "EasyTcpClient.hpp"

bool g_bExit = false;

void cmdThread(EasyTcpClient* client)
{
	std::string cmd;
	while (std::cin >> cmd)
	{
		if (cmd == "exit")
		{
			client->Close();
			std::cout << "线程退出" << std::endl;
			break;
		}
		else if (cmd == "login")
		{
			Login login{  };
			strcpy(login.userName, "admin");
			strcpy(login.passWord, "12345");
			client->Send(&login);
			//Sleep(500);
		}
		else if (cmd == "logout")
		{
			LogOut logout{  };
			strcpy(logout.userName, "admin");
			client->Send(&logout);
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
	EasyTcpClient client;
	client.initSocket();
	client.Connect("127.0.0.1", 8888);
	//启动线程
	std::thread td(cmdThread, &client);
	td.detach();
	while (client.isRun())
	{
		client.OnRun();
	}

	///

	client.Close();
	return 0;
}

