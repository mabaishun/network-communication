/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：global.h
*   Author       ：spider
*   Date Time    ：2020年05月06日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <iostream>
#include <string.h>


#define PORT 8888
#define SERVERADDR "127.0.0.1"

#define PRINTINFO   \
     __FILE__ << ":" <<  __FUNCTION__<< ":" << __LINE__ << " ] "

