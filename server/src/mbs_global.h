/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：global.h
*   Author       ：spider
*   Date Time    ：2020年05月06日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <netdb.h>
#include <time.h>

#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <iostream>
#include <string.h>

#include <signal.h>
#include <fcntl.h>

#define PORT 8888
#define BACKLOG 1024


#define PRINTINFO   \
     __FILE__ << ":" <<  __FUNCTION__<< ":" << __LINE__ << " ] "


const int BUFFSIZE = 4096;


//消息类型
enum CMD
{
    CMD_LOGIN,
    CMD_LOGIN_RET,
    CMD_LOGOUT,
    CMD_LOGOUT_RET,
    CMD_NEW_USER,
    CMD_ERROR
};


//包头
struct PackageHeader
{
    PackageHeader()
    {
        len = sizeof(PackageHeader);
        cmd = CMD_ERROR;
    }
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
    char data[1024 - 68];
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

    char data[1024 - 8];
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


//群发
struct newuser:public PackageHeader
{
    newuser()
    {
        len = sizeof(newuser);
        cmd = CMD_NEW_USER;
        sock = 0;
    }
    int sock;
};

