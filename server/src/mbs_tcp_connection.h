/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_tcp_connection.h
*   Author       ：spider
*   Date Time    ：2020年05月12日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include "mbs_global.h"

//链接管理类
class TcpConnection
{
    public:
        TcpConnection(int fd = -1);
        ~TcpConnection();

        inline const int getsock() { return sockfd; }
        inline char * getbuff() { return buffer; }

        inline int getoffset() { return offset; }
        inline void setoffset(int pos) { offset = pos; }
    private:
        int sockfd; //套接字
        //第二缓冲区
        char buffer[BUFFSIZE * 10];
        int offset;
};
