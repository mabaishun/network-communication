/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：uban_tcp_server.cpp
*   Author       ：spider
*   Date Time    ：2020年04月22日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include <iostream>
#include "mbs_tcp_server.h"
#include "mbs_processor_message.h"

static int count = 0;

TcpServer::TcpServer(const char * host,const uint16_t port)
{
    if(listen(host,port) < 0)
    {
        std::cout << PRINTINFO << "监听失败" << std::endl;
    }
}

TcpServer::~TcpServer()
{
    Close();
}

bool TcpServer::run()
{
    if(!isrun())
    {
        return false;
    }
    while(true)
    {
        fd_set fdr;
        fd_set fdw;
        fd_set fde;

        FD_ZERO(&fdr);
        FD_ZERO(&fdw);
        FD_ZERO(&fde);

        FD_SET(listenfd,&fdr);
        FD_SET(listenfd,&fdr);
        FD_SET(listenfd,&fdr);

        int maxsock = listenfd;
        for(int i = 0;i < (int)fd.size();i++)
        {
            FD_SET(fd[i]->getsock(),&fdr);
            if(fd[i]->getsock() > maxsock)
            {
                maxsock = fd[i]->getsock();
            }
        }

        timeval tim{1,0};
        int ret = select(maxsock + 1,&fdr,&fdw,&fde,&tim);
        if(ret  == -1)
        {
            std::cout << PRINTINFO << listenfd << " select任务结束" << std::endl;
            break;
        }
        if(ret == 0)
        {
            //std::cout << PRINTINFO << "select 任务超时" << std::endl;
            continue;
        }
        //std::cout << "select ret = [ " << ret << " ] count [ " << count++ << " ]" << std::endl;
        if(FD_ISSET(listenfd,&fdr))
        {
            FD_CLR(listenfd,&fdr);
            int connfd = accept();
            if(connfd != -1)
            {
                fd.push_back(new TcpConnection(connfd));
                newuser nu;
                nu.sock = connfd;
                sendall(&nu);
                //TcpConnection conn(connfd);
                //_pool->enqueue(&process,&conn);
                //TcpConnection conn(connfd);
                //conn.read();
                //char head[sizeof(PacketHeader) + 1];
                //PacketHeader ph;
                //std::cout << "开始读取数据" << std::endl;
                //int ret = read(connfd,head,sizeof(PacketHeader));
                //std::cout << "读取数据结束" << std::endl;
                //if(ret == -1)
                //{
                //    std::cout << "读取数据错误" << strerror(errno) << std::endl;
                //}
            }
        }
            for(int i = 0;i < (int)fd.size();i++)
            {
                if(FD_ISSET(fd[i]->getsock(),&fdr))
                {
                    //std::cout << "开始处理业务" << std::endl;
                    ProcessMessage pm(fd[i]);
                    if(pm.process() == -1)
                    {
                        close(fd[i]->getsock());
                        auto it = fd.begin() + i;
                        if(it != fd.end())
                        {
                            delete fd[i];
                            fd.erase(it);
                        }
                    }
                }
            }

    }
    return true;
}


void TcpServer::Close()
{
    if(listenfd != -1)
    {
        for(int i = 0;i < (int)fd.size();i++)
        {
            ::close(fd[i]->getsock());
        }
        ::close(listenfd);
    }
    fd.clear();
}

bool TcpServer::isrun()
{
    return listenflag;
}

void TcpServer::sendall(PackageHeader *ph)
{
    if (isrun() && ph)
    {
        for(int i = 0;i < (int)fd.size();i++)
        {
            int len = send(fd[i]->getsock(),(char*)ph,ph->len,0);
            if(len <= 0)
            {
                std::cout << "群发消息到 [" << fd[i] << "] 错误" << std::endl;
            }
        }
    }
}
