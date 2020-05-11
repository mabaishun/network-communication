/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_client.cpp
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_client_base.h"

ClientBase::ClientBase():sockfd(-1)
{
    bzero(&server,sizeof(struct sockaddr_in));
}
ClientBase::~ClientBase()
{

}
void ClientBase::clientsocket()
{
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    std::cout << "创建套接字 [" << sockfd << "] 成功" << std::endl;
}

void ClientBase::resocket()
{
    //重置socket，首先关闭，然后再创建
    closes();
    clientsocket();
}

bool ClientBase::connection(const std::string& ip,const short port)
{
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVERADDR);
    int ret = connect(sockfd,(sockaddr*)&server,sizeof(sockaddr_in));

    if(ret == -1)
    {
        perror("connect");
        return -1;
    }
    return sockfd;
}

bool ClientBase::reconnect()
{
    int ret = connect(sockfd,(sockaddr*)&server,sizeof(sockaddr_in));

    if(ret == -1)
    {
        perror("connect");
        return -1;
    }
    return sockfd;
}

void ClientBase::closes()
{
    if(sockfd != -1)
    {
        close(sockfd);
    }
}
