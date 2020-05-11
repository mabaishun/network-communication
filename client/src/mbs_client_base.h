/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_client.h
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once
#include "mbs_global.h"

class ClientBase
{
    public:
        ClientBase();
        virtual ~ClientBase();
        void clientsocket();
        void resocket();
        //返回套接字
        bool connection(const std::string& ip = SERVERADDR,const short port = PORT);
        //返回套接字
        bool reconnect();
        void closes();
    protected:
        int sockfd;
        sockaddr_in server;
};
