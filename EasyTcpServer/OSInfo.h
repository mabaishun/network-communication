#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <string.h>

#include <intrin.h>

#pragma comment(lib,"iphlpapi.lib")
#pragma warning(disable: 4996)



static const int kMaxInfoBuffer = 256;
#define  GBYTES  1073741824  
#define  MBYTES  1048576  
#define  KBYTES  1024  
#define  DKBYTES 1024.0  


class OSInfo
{
public:
	OSInfo();
	~OSInfo();
	std::string getOsInfo();
	std::string getCpuInfo();
	std::string getMemoryInfo();
	std::string getHardDiskInfo();
	std::string getNetworkInfo();
	std::string getProcessInfo();
private:
	std::string execCmd(const char* cmd);
};

