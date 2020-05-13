/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_time_stamp.h
*   Author       ：spider
*   Date Time    ：2020年05月13日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once

#include <chrono>

class TimeStamp
{
    public:
        TimeStamp();
        ~TimeStamp();
        void update();
        //获取秒数
        double getElapseSecond();
        //获取毫秒
        double getElapseMilliSec();
        //获取微秒
        long long getElapseMicroSec();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> begin;
};
