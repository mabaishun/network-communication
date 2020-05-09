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

//消息类型
enum CMD
{
    CMD_LOGIN,
    CMD_LOGOUT,
    CMD_ERROR
};


//包头
struct PackageHeader
{
    short len;  //数据长度
    short cmd;  //消息类型
};

//登陆模拟，作为包体使用
struct login
{
    char username[32];//用户名
    char password[32];//密码
};

//登录结果
struct loginret
{
    int result;
};

//登出模拟
struct logout
{
    char username[32];  //用户名
};

//登出结果
struct logoutret
{
    int result;
};


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

    while(true)
    {
        PackageHeader header = {0};
        //5 接受客户端请求数据
        int nlen = recv(acceptfd,(char*)&header,sizeof(PackageHeader),0);
        if(nlen <= 0)
        {
            std::cout << "客户端已退出" << std::endl;
            break;
        }
        std::cout << "读取到客户端数据长度：" << header.len << " 命令：" << header.cmd << std::endl;
        //6 处理客户端请求
        switch(header.cmd)
        {
            case CMD_LOGIN:
                {
                    login lin = { 0 };
                    nlen = recv(acceptfd,(char*)&lin,sizeof(login),0);
                    if(nlen < 0)
                    {
                        perror("recv");
                        break;
                    }
                    std::cout << "登录信息：\n\t用户名：" << lin.username << "\n\t密码：" << lin.password << std::endl; 
                    //忽略判断用户名密码是否正确
                    loginret lr = { 200 };
                    //7.1 发送数据到客户端
                    send(acceptfd,(char *)&header,sizeof(PackageHeader),0);
                    send(acceptfd,(char*)&lr,sizeof(loginret),0);
                }
                break;
            case CMD_LOGOUT:
                {
                    logout lo = {0};
                    recv(acceptfd,(char *)&lo,sizeof(logout),0);
                    std::cout << "\t" << lo.username << " 登出" << std::endl;
                    logoutret lr = { 202 };
                    //7.2 发送数据到客户端
                    send(acceptfd,(char *)&header,sizeof(PackageHeader),0);
                    send(acceptfd,(char*)&lr,sizeof(logoutret),0);
                }
                break;
            default:
                {
                    header.cmd = CMD_ERROR;
                    header.len = 0;
                    send(acceptfd,(char*)&header,sizeof(PackageHeader),0);
                }
                break;
        }
    }
    //8 关闭链接套接字
    close(acceptfd);
    std::cout << "任务结束" << std::endl;
    //9 关闭套接字
    close(sockfd);
    return 0;
}
