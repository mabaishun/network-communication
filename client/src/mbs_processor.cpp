/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_processor.cpp
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_processor.h"

//指令输出
static std::string print(int cmd)
{
    std::string str;
    switch(cmd)
    {
        case CMD_LOGIN:
            str = "CMD_LOGIN";
            break;
        case CMD_LOGIN_RET:
            str = "CMD_LOGIN_RET";
            break;
        case CMD_LOGOUT:
            str = "CMD_LOGOUT";
            break;
        case CMD_LOGOUT_RET:
            str = "CMD_LOGOUT_RET";
            break;
        case CMD_ERROR:
            str = "CMD_ERROR";
            break;
        case CMD_NEW_USER:
            str = "CMD_NEW_USER";
            break;
        default:
            str = "";
            break;
    }
    return str;
}


Processor::Processor()
{

}
Processor::~Processor()
{

}

int Processor::processor(int sockfd)
{
    return recvdata(sockfd);
}

int Processor::recvdata(int sockfd)
{
    //缓冲区
    char buffer[4096] = {};
    int len = recv(sockfd,buffer,sizeof(PackageHeader),0);
    if(len <= 0)
    {
        std::cout << "与服务器断开链接" << std::endl;
        return -1;
    }
    //std::cout << "len = [ " << len << " ]" << std::endl;
    PackageHeader *ph = (PackageHeader*)buffer;
    std::cout << "读取到服务器数据长度：" << ph->len << " 命令：" << print(ph->cmd)<< std::endl;
    len = recv(sockfd,buffer + sizeof(PackageHeader),ph->len - sizeof(PackageHeader),0);
    if(len <= 0)
    {
        std::cout << "数据不完整" << std::endl;
        return -1;
    }
    message(ph);
    return 0;
}

int Processor::message(PackageHeader* header)
{
    //6 处理客户端请求
    switch(header->cmd)
    {
        case CMD_LOGIN_RET:
            {
                loginret *lin = (loginret*)header;
                std::cout << "\t登录结果:" << lin->result << std::endl; 
                //忽略判断用户名密码是否正确
            }
            break;
        case CMD_LOGOUT_RET:
            {
                logoutret *lo = (logoutret*)header ;
                std::cout << "\t登出结果：" << lo->result << std::endl;
            }
            break;
        case CMD_NEW_USER:
            {
                newuser * nu = (newuser*)header;
                std::cout << "\tsocket = [" << nu->sock << "] 登录" << std::endl;
            }
            break;
        default:
            {
                header->cmd = CMD_ERROR;
                header->len = 0;
                sendmessage(header);
            }
            break;
    }
    
    return 0;
}

int Processor::sendmessage(PackageHeader *ph)
{
    if (isrun() && ph)
        return send(sockfd,(const char*)ph,ph->len,0);
    return -1;
}
