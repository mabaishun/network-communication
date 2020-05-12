/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：uban_tcp_base.h
*   Author       ：spider
*   Date Time    ：2020年04月22日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include "mbs_global.h"

class TcpBase
{
    public:
        TcpBase();
        virtual ~TcpBase();
        int listen(const char *host,const uint16_t port);
        //开始提供创建链接服务
        int accept();
        virtual void Close();
    protected:
        bool listenflag;
        int listenfd;//套接字
        struct sockaddr_in cliaddr;//客户端地址
        socklen_t addrlen;//客户端地址长度
};
