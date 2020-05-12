/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：uban_tcp_server.h
*   Author       ：spider
*   Date Time    ：2020年04月22日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once
#include "mbs_tcp_base.h"


class TcpServer : public TcpBase
{
    public:
        TcpServer(const char* host,const uint16_t port);
        ~TcpServer();

        bool run();

        bool isrun();

        virtual void Close();
        //群发
        virtual void sendall(PackageHeader* ph);
    private:
        std::vector<int> fd;
};
