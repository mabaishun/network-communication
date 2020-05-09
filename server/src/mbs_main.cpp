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

int main(int argc,char **argv)
{
    struct sockaddr_in server,client;
    int ret = 0;

    //1 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    std::cout << "创建套接字成功：" << sockfd << std::endl;

    bzero(&server,sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;


    //2 绑定服务器地址
    ret = bind(sockfd,(sockaddr*)&server,sizeof(struct sockaddr_in));
    if(ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    //3 启动监听
    ret = listen(sockfd,BACKLOG);
    if(ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    bzero(&client,sizeof(sockaddr_in));
    socklen_t socklen = sizeof(sockaddr_in);
    //4 等待链接到来
    int acceptfd = accept(sockfd,(sockaddr*)&client,&socklen);
    if (acceptfd == -1)
    {
        if(acceptfd == EINTR)
        {
            std::cout << PRINTINFO  << "accept 链接错误:" << strerror(errno) << std::endl;
            //continue;
        }
        else if(errno == EMFILE)
        {
            std::cout << PRINTINFO << "accept 链接错误:" << strerror(errno) << std::endl;
        }
        else if(errno == EAGAIN)
        {
            std::cout << PRINTINFO << "accpet 链接错误:" << strerror(errno) << std::endl;
            //break;
        }
        else
        {
            std::cout << PRINTINFO << "accept 链接错误" << std::endl;
            return -1;
        }
    }
    else
    {
        char dest[128];
        memset(dest, 0, 128);
        inet_ntop(AF_INET, &client.sin_addr, dest, 16);
        std::cout << "新客户端加入:socket = " << acceptfd << " Ip = " << dest << std::endl;
    }

    char recvbuff[128] = { 0 };
    while(true)
    {
        //5 接受客户端请求数据
        int nlen = recv(acceptfd,recvbuff,128,0);
        if(nlen <= 0)
        {
            std::cout << "客户端已退出,任务结束" << std::endl;
            break;
        }
        std::cout << "读取到客户端请求：" << recvbuff << std::endl;
        //6 处理客户端请求
        if (0 == strcmp(recvbuff,"name"))
        {
            //7.1 发送数据到客户端
            const char *msgbuf = "小强";
            send(acceptfd,msgbuf,strlen(msgbuf) + 1,0);
        }
        else if (0 == strcmp(recvbuff,"age"))
        {
            //7.2 发送数据到客户端
            const char *msgbuf = "80";
            send(acceptfd,msgbuf,strlen(msgbuf) + 1,0);
        }
        else
        {
            //7.3 发送数据到客户端
            const char *msgbuf = "???";
            send(acceptfd,msgbuf,strlen(msgbuf) + 1,0);
        }
    }
    //8 关闭链接套接字
    close(acceptfd);
    //9 关闭套接字
    close(sockfd);
    return 0;
}
