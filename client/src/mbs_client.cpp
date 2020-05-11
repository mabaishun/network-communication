/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_client.cpp
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_client.h"

Client::Client()
{

}
Client::~Client()
{

}

bool Client::run()
{
    if(isrun())
    {
        fd_set fdread;
        FD_ZERO(&fdread);
        FD_SET(sockfd,&fdread);

        timeval t { 2,0 };
        int ret = select(sockfd + 1,&fdread,nullptr,nullptr,&t);
        if(ret < 0)
        {
            std::cout << "socket [" << sockfd << "] " << "select任务结束" << std::endl;
            closes();
            return false;
        }
        if(FD_ISSET(sockfd,&fdread))
        {
            FD_CLR(sockfd,&fdread);
            if(-1 == processor(sockfd))
            {
                std::cout << "socket = [" << sockfd << "] select任务结束" << std::endl;
            }
        }
        return true;
    }
    return false;
}

bool Client::isrun()
{
    return sockfd != -1;
}
