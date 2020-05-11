/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_client.h
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include "mbs_client_base.h"

class Client:public ClientBase
{
    public:
        Client();
        virtual ~Client();
        bool run();
        bool isrun();
        virtual int processor(int sockfd) = 0;
    private:
};
