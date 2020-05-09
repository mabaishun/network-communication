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
    struct sockaddr_in server;
    int ret = 0;

    //1 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    std::cout << "创建套接字成功：" << sockfd << std::endl;
    
    bzero(&server,sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVERADDR);
    //2 链接服务器
    ret = connect(sockfd,(sockaddr*)&server,sizeof(sockaddr_in));
    if(ret == -1)
    {
        perror("connect");
        exit(-1);
    }

    char cmdbuff[128] = {0};
    while(true)
    {
        //3. 输入请求命令
        //从终端读取数据
        fgets(cmdbuff,128,stdin);
        cmdbuff[strlen(cmdbuff) - 1] = '\0';
        std::cout << "从终端读取数据:" << cmdbuff << std::endl;
        //4 处理请求
        if( 0 == strcmp(cmdbuff,"exit") )
        {
            break;
        }
        else
        {
            //5. 向服务器发送请求
            ret = send(sockfd,cmdbuff,strlen(cmdbuff) + 1,0);
            if(ret == -1)
            {
                perror("send");
                break;
            }
        }

        //6 接受服务器信息
        char recvbuff[256];
        bzero(recvbuff,256);
        ret = recv(sockfd,recvbuff,256,0);
        if(ret < 0)
        {
            perror("recv");
            exit(-1);
        }
        std::cout << "读取到服务器数据:" << recvbuff << std::endl;
    }


    //7 关闭链接
    close(sockfd);

    std::cout << "任务结束" << std::endl;
    return 0;
}
