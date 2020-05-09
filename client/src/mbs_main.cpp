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
    CMD_LOGIN_RET,
    CMD_LOGOUT,
    CMD_LOGOUT_RET,
    CMD_ERROR
};


//包头
struct PackageHeader
{
    short len;  //数据长度
    short cmd;  //消息类型
};

//登陆模拟，作为包体使用
struct login:public PackageHeader
{
    login()
    {
        len = sizeof(login);
        cmd = CMD_LOGIN;
    }
    char username[32];//用户名
    char password[32];//密码
};

//登录结果
struct loginret:public PackageHeader
{
    loginret()
    {
        len = sizeof(loginret);
        cmd = CMD_LOGIN_RET;
        result = 200;
    }
    int result;
};

//登出模拟
struct logout:public PackageHeader
{
    logout()
    {
        len = sizeof(logout);
        cmd = CMD_LOGOUT;
    }
    char username[32];  //用户名
};

//登出结果
struct logoutret:public PackageHeader
{
    logoutret()
    {
        len = sizeof(logoutret);
        cmd = CMD_LOGOUT_RET;
        result = 202;
    }
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
            login ln;
            strcpy(ln.username,"admin");
            strcpy(ln.password,"password");
            //向服务器发送请求指令
            send(sockfd,(char*)&ln,sizeof(login),0);

            //接受服务器返回结果
            loginret lnr;
            recv(sockfd,(char*)&lnr,sizeof(loginret),0);
            std::cout << "\t登录结果:" << lnr.result << std::endl; 
        }
        else if( 0 == strcmp(cmdbuff,"logout") )
        {
            logout lo;
            strcpy(lo.username,"admin");
            //向服务器发送请求指令
            send(sockfd,(char*)&lo,sizeof(logout),0);

            //接受服务器返回结果
            logoutret lor;
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
