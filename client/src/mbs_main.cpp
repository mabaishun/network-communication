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
        std::cout << "请输入命令：";
        //3. 输入请求命令
        //从终端读取数据
        fgets(cmdbuff,128,stdin);
        cmdbuff[strlen(cmdbuff) - 1] = '\0';
        std::cout << "\t从终端读取数据:" << cmdbuff << std::endl;
        //4 处理请求
        if( 0 == strcmp(cmdbuff,"exit") )
        {
            break;
        }
        else if( 0 == strcmp(cmdbuff,"login") )
        {
            login ln = {"admin","password"};
            PackageHeader ph;
            ph.cmd = CMD_LOGIN;
            ph.len = sizeof(login);
            //向服务器发送请求指令
            send(sockfd,(char*)&ph,sizeof(PackageHeader),0);
            send(sockfd,(char*)&ln,sizeof(login),0);

            //接受服务器返回结果
            PackageHeader phr = {0};
            loginret lnr = {0};
            recv(sockfd,(char*)&phr,sizeof(PackageHeader),0);
            recv(sockfd,(char*)&lnr,sizeof(loginret),0);
            std::cout << "\t登录结果:" << lnr.result << std::endl; 
        }
        else if( 0 == strcmp(cmdbuff,"logout") )
        {
            logout lo = { "admin" };
            PackageHeader ph;
            ph.cmd = CMD_LOGOUT;
            ph.len = sizeof(logout);
            //向服务器发送请求指令
            send(sockfd,(char*)&ph,sizeof(PackageHeader),0);
            send(sockfd,(char*)&lo,sizeof(logout),0);

            //接受服务器返回结果
            PackageHeader phr = {0};
            logoutret lor = {0};
            recv(sockfd,(char*)&phr,sizeof(PackageHeader),0);
            recv(sockfd,(char*)&lor,sizeof(logoutret),0);
            std::cout << "\t登出结果:" << lor.result << std::endl; 
        }
        else
        {
            std::cout << "不支持的命令,请重新输入" << std::endl;
        }

    }
    //7 关闭链接
    close(sockfd);

    std::cout << "任务结束" << std::endl;
    return 0;
}
