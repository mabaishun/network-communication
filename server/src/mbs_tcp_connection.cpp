/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_tcp_connection.cpp
*   Author       ：spider
*   Date Time    ：2020年05月12日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_tcp_connection.h"

TcpConnection::TcpConnection(int fd):sockfd(fd),offset(0)
{
    bzero(buffer,sizeof(buffer));
}

TcpConnection::~TcpConnection()
{

}


