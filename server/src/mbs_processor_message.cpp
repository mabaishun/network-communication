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



ProcessMessage::ProcessMessage(TcpConnection *con):conn(con)
{
    bzero(tempbuff,sizeof(tempbuff));
}

ProcessMessage::~ProcessMessage()
{
}
int ProcessMessage::process(int &count)
{
    //接受客户端数据
    int len = recv(conn->getsock(),tempbuff,BUFFSIZE,0);
    if(len <= 0)
    {
        std::cout << "客户端 socket = [" << conn->getsock() << "] 任务结束" << std::endl;
        return -1;
    }

    memcpy(conn->getbuff() + conn->getoffset(),tempbuff,len);

    conn->setoffset(conn->getoffset() + len);

    while(conn->getoffset() >= (int)sizeof(PackageHeader))
    {
        PackageHeader* ph = (PackageHeader*)conn->getbuff();
        if(conn->getoffset() >= ph->len)
        {
            int size = conn->getoffset() - ph->len;
            count++;
            netmessage(ph);    
            memcpy(conn->getbuff(),conn->getbuff() + ph->len,conn->getoffset());
            conn->setoffset(size);
        }
        else
        {
            break;
        }
    }
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
                //std::cout << "登录信息：\n\t用户名：" << lin->username << "\n\t密码：" << lin->password << std::endl; 
                //忽略判断用户名密码是否正确
                //loginret lr = {};
                //lr.result = 200;
                //7.1 发送数据到客户端
                //send(conn->getsock(),(char*)&lr,sizeof(loginret),0);
            }
            break;
        case CMD_LOGOUT:
            {
                logout *lo = (logout*)ph;
                //std::cout << "\t" << lo->username << " 登出" << std::endl;
                //logoutret lr = {};
                //lr.result = 202;
                //7.2 发送数据到客户端
                //send(conn->getsock(),(char*)&lr,sizeof(logoutret),0);
            }
            break;
        default:
            {
                std::cout << "收到为定义消息" << std::endl;
                ph->cmd = CMD_ERROR;
                ph->len = 0;
                //send(conn->getsock(),(char*)ph,sizeof(PackageHeader),0);
            }
            break;
    }
}

int ProcessMessage::senddata(PackageHeader *ph)
{
    if(ph)
    {
        return send(conn->getsock(),(char *)ph,ph->len,0);
    }
    return -1;
}
