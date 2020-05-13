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
    //接受客户端数据
    int len = recv(sockfd,tempbuff,BUFFSIZE,0);
    if(len <= 0)
    {
        std::cout << "客户端 socket = [" << sockfd << "] 任务结束" << std::endl;
        return -1;
    }

    memcpy(buffer + offset,tempbuff,len);

    offset += len;

    while(offset >= sizeof(PackageHeader))
    {
        PackageHeader* ph = (PackageHeader*)buffer;
        if(offset >= ph->len)
        {
            offset -= ph->len;
            message(ph);    
            memcpy(buffer,buffer + ph->len,offset);
        }
        else
        {
            break;
        }
    }
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
                //std::cout << "\t登录结果:" << lin->result << std::endl; 
                //忽略判断用户名密码是否正确
            }
            break;
        case CMD_LOGOUT_RET:
            {
                logoutret *lo = (logoutret*)header ;
                //std::cout << "\t登出结果：" << lo->result << std::endl;
            }
            break;
        case CMD_NEW_USER:
            {
                newuser * nu = (newuser*)header;
                //std::cout << "\tsocket = [" << nu->sock << "] 登录" << std::endl;
            }
            break;
        case CMD_ERROR:
            {
                std::cout << "\t消息错误" << std::endl;
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
