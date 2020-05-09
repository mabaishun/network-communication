/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_server_base.h
*   Author       ：spider
*   Date Time    ：2020年05月08日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include "mbs_global.h"


class ServerBase
{
    public:
        ServerBase();
        ~ServerBase();
    private:
        int sockfd;
};
