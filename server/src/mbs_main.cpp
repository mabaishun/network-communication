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

int main(int argc,char **argv)
{
    TcpServer server(nullptr,PORT);
    while(server.isrun())
    {
        server.run();
    }
    server.Close();
    return 0;
}
