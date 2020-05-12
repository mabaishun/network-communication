/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_processor_message.cpp
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_processor_message.h"
#include <unistd.h>


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



ProcessMessage::ProcessMessage(int sockfd):fd(sockfd)
{

}

ProcessMessage::~ProcessMessage()
{
}
int ProcessMessage::process()
{
    //缓冲区
    char buffer[4096] = {};
    //接受客户端数据
    int len = recv(fd,buffer,sizeof(PackageHeader),0);
    if(len <= 0)
    {
        std::cout << "客户端 socket = [" << fd << "] 任务结束" << std::endl;
        return -1;
    }
    //std::cout << "len = [ " << len << " ]" << std::endl; 
    PackageHeader *ph = (PackageHeader*)buffer;
    std::cout << "读取到客户端数据长度：" << ph->len << " 命令：" << print(ph->cmd)<< std::endl;
     len = recv(fd,buffer + sizeof(PackageHeader),ph->len - sizeof(PackageHeader),0);
     if(len <= 0)
     {
         std::cout << "数据不完整" << std::endl;
     }
     netmessage(ph);
    return 0;
}
int ProcessMessage::recvdata()
{
    return 0;
}
void ProcessMessage::netmessage(PackageHeader *ph)
{
    switch(ph->cmd)
    {
        case CMD_LOGIN:
            {
                login *lin = (login*)ph;
                std::cout << "登录信息：\n\t用户名：" << lin->username << "\n\t密码：" << lin->password << std::endl; 
                //忽略判断用户名密码是否正确
                loginret lr = {};
                lr.result = 200;
                //7.1 发送数据到客户端
                send(fd,(char*)&lr,sizeof(loginret),0);
            }
            break;
        case CMD_LOGOUT:
            {
                logout *lo = (logout*)ph;
                std::cout << "\t" << lo->username << " 登出" << std::endl;
                logoutret lr = {};
                lr.result = 202;
                //7.2 发送数据到客户端
                send(fd,(char*)&lr,sizeof(logoutret),0);
            }
            break;
        default:
            {
                ph->cmd = CMD_ERROR;
                ph->len = 0;
                send(fd,(char*)ph,sizeof(PackageHeader),0);
            }
            break;
    }
}

int ProcessMessage::senddata(PackageHeader *ph)
{
    if(ph)
    {
        return send(fd,(char *)ph,ph->len,0);
    }
    return -1;
}
