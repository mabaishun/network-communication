/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_processor.h
*   Author       ：spider
*   Date Time    ：2020年05月11日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include "mbs_client.h"

class Processor:public Client
{
    public:
        Processor();
        ~Processor();
        int processor(int sockfd);

        int recvdata(int sockfd);
        int message(PackageHeader* header);

        //发送数据
        int sendmessage(PackageHeader *ph);
    private:

};
