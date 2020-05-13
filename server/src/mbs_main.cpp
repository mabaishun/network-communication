/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_main.cpp
*   Author       ：spider
*   Date Time    ：2020年05月06日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_global.h"
#include "mbs_tcp_server.h"
#include <thread>

bool IsRun = true;

void cmdThread()
{
    while(true)
    {
        char cmd[128];
        fgets(cmd,128,stdin);
        cmd[strlen(cmd) - 1] = '\0';
        if(strcmp(cmd,"exit") == 0)
        {
            IsRun = false;
            printf("线程退出\n");
            exit(1);
        }
        else
        {
            std::cout << "不支持的命令" << std::endl;
        }
    }
}




int main(int argc,char **argv)
{
    TcpServer server(nullptr,PORT);

    std::thread t(cmdThread);
    t.detach();

    while(IsRun)
    {
        server.run();
    }
    server.Close();
    return 0;
}
