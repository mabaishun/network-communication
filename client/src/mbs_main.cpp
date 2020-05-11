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
#include <thread>

#include "mbs_processor.h"

void cmdThread(Processor *p)
{
    char buffer[128] = {0};
    while(true)
    {
        fgets(buffer,128,stdin);
        buffer[strlen(buffer) - 1] = '\0';
        if(strcmp(buffer,"exit") == 0)
        {
            std::cout << "退出" << std::endl;
            exit(1);
        }
        else if(strcmp(buffer,"login") == 0)
        {
            login lo;
            strcpy(lo.username,"admin");
            strcpy(lo.password,"password");
            p->sendmessage(&lo);
        }
        else if(strcmp(buffer,"logout") == 0)
        {
            logout lo;
            strcpy(lo.username,"admin");
            p->sendmessage(&lo);
        }
        else 
        {
            std::cout << "不能识别的请求" << std::endl;
        }
    }
}


int main(void)
{
    Processor p1;
    Processor p2;
    p1.clientsocket();
    p2.clientsocket();
    p1.connection();
    p2.connection();
    //std::thread t1(cmdThread,&p1);
    //std::thread t2(cmdThread,&p2);
    //t1.detach();
    //t2.detach();

    login lo;
    strcpy(lo.username,"admin");
    strcpy(lo.password,"password");


    while(p1.isrun())
    {
        p1.run();
        p2.run();
        p1.sendmessage(&lo);
        p2.sendmessage(&lo);
    }
   
    p1.closes();
    p2.closes();
    return 0;
}


#if 0


//指令输出
std::string print(int cmd)
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
        default:
            str = "";
            break;
    }
    return str;
}
int process(int acceptfd)
{
    PackageHeader header = {0};
    //5 接受客户端请求数据
    int nlen = recv(acceptfd,(char*)&header,sizeof(PackageHeader),0);
    if(nlen <= 0)
    {
        std::cout << "与服务器断开链接:" << nlen << std::endl;
        return -1;
    }
    std::cout << "读取到客户端数据长度：" << header.len << " 命令：" << print(header.cmd)<< std::endl;
    //6 处理客户端请求
    switch(header.cmd)
    {
        case CMD_LOGIN_RET:
            {
                loginret lin = {};
                nlen = recv(acceptfd,(char*)&lin + sizeof(PackageHeader),sizeof(loginret) - sizeof(PackageHeader),0);
                if(nlen < 0)
                {
                    perror("recv");
                    break;
                }
                std::cout << "\t登录结果:" << lin.result << std::endl; 
                //忽略判断用户名密码是否正确
            }
            break;
        case CMD_LOGOUT_RET:
            {
                logoutret lo = {};
                recv(acceptfd,(char *)&lo + sizeof(PackageHeader),sizeof(logoutret) - sizeof(PackageHeader),0);
                std::cout << "\t登出结果：" << lo.result << std::endl;
            }
            break;
        case CMD_NEW_USER:
            {
                newuser nu = {};
                recv(acceptfd,(char *)&nu + sizeof(PackageHeader),sizeof(newuser) - sizeof(PackageHeader),0);
                std::cout << nu.sock << "登录" << std::endl;
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
    return -1;
}








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

    std::thread th(cmdThread,sockfd);
    th.detach();

    while(true)
    {
        fd_set rdfd;
        
        FD_ZERO(&rdfd);

        FD_SET(sockfd,&rdfd);
        timeval t = {1,0};
        ret = select(sockfd + 1,&rdfd,nullptr,nullptr,&t);
        if(ret < 0)
        {
            std::cout << "select错误" << std::endl;
            break;
        }
        if(ret == 0)
        {
            std::cout << "空闲处理" << std::endl;
            std::cout << "select超时" << std::endl;
            continue;
        }

        if(FD_ISSET(sockfd,&rdfd))
        {
            FD_SET(sockfd,&rdfd);
            if(process(sockfd) == -1)
            {
                std::cout << "select任务结束" << std::endl;
                break;
            }
        }

    }
    //7 关闭链接
    close(sockfd);

    std::cout << "任务结束" << std::endl;
    return 0;
}
#endif 
