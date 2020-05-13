/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：mbs_time_stamp.cpp
*   Author       ：spider
*   Date Time    ：2020年05月13日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_time_stamp.h"

TimeStamp::TimeStamp()
{
    update();
}

TimeStamp::~TimeStamp()
{

}

void TimeStamp::update()
{
    begin = std::chrono::high_resolution_clock::now();
}
//获取秒数
double TimeStamp::getElapseSecond()
{
    return getElapseMicroSec() * 0.000001;
}
//获取毫秒
double TimeStamp::getElapseMilliSec()
{
    return getElapseMicroSec() * 0.001;
}
//获取微秒
long long TimeStamp::getElapseMicroSec()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - begin).count();
}
