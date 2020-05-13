/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_processor_message.h
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include "mbs_global.h"
#include "mbs_tcp_connection.h"

class ProcessMessage
{
    public:
        ProcessMessage(TcpConnection *con);
        ~ProcessMessage();
        int process();
        //读取数据
        int recvdata();
        //处理网络消息
        virtual void netmessage(PackageHeader *ph);
        //发送网络消息
        int senddata(PackageHeader *ph);
    private:
        TcpConnection *conn;
        char tempbuff[BUFFSIZE];
};
