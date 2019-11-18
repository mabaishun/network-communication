#include "OSInfo.h"



OSInfo::OSInfo()
{
}


OSInfo::~OSInfo()
{
}

std::string OSInfo::getOsInfo()
{
	OSVERSIONINFO osver = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osver);

	std::string os_name;

	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
	{
		os_name = "Windows 2000";
	}
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
	{
		os_name = "windows XP";
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
	{
		os_name = "Windows 2003";
	}
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
	{
		os_name = "Windows vista";
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
	{
		os_name = "Windows 7";
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
	{
		os_name = "Windows 10";
	}
	else
	{
		os_name = "没能识别";
	}


	return os_name;
}

std::string OSInfo::getCpuInfo()
{
	std::string os_cpu;
	int cpuInfo[4]{ 0 };
	char cpu_manufacture[32]{ 0 };
	char cpu_type[32]{ 0 };
	char cpu_freq[32]{ 0 };

	__cpuid(cpuInfo, 0x80000002);
	memcpy(cpu_manufacture,cpuInfo, sizeof(cpuInfo));

	__cpuid(cpuInfo, 0x80000003);
	memcpy(cpu_type, cpuInfo, sizeof(cpuInfo));

	__cpuid(cpuInfo, 0x80000004);
	memcpy(cpu_freq, cpuInfo, sizeof(cpuInfo));

	os_cpu = "CPU manufacture: " + std::string(cpu_manufacture)
		+ "type: " + std::string(cpu_type) + "main frequency: " + std::string(cpu_freq);
	return os_cpu;
}

std::string OSInfo::getMemoryInfo()
{
	std::string memory_info;

	MEMORYSTATUSEX statusex;
	statusex.dwLength = sizeof(statusex);
	if (GlobalMemoryStatusEx(&statusex))
	{
		unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;
		double decimal_total = 0, decimal_avl = 0;

		remain_total = statusex.ullTotalPhys % GBYTES;
		total = statusex.ullTotalPhys / GBYTES;
		avl = statusex.ullAvailPhys / GBYTES;
		remain_avl = statusex.ullAvailPhys % GBYTES;

		if (remain_total > 0)
			decimal_total = (remain_total / MBYTES) / DKBYTES;
		if (remain_avl > 0)
			decimal_avl = (remain_avl / MBYTES) / DKBYTES;
	
		decimal_total += (double)total;
		decimal_avl += (double)avl;

		char buffer[kMaxInfoBuffer];
		sprintf_s(buffer, kMaxInfoBuffer, "total %0.2f GB (%.2f GB available)",decimal_total, decimal_avl);
		memory_info.append(buffer);
	}
	return memory_info;
}

std::string OSInfo::getHardDiskInfo()
{
	std::string hd_seiral = "HardDis  " + execCmd("wmic path win32_physicalmedia get SerialNumber");

	return hd_seiral;
}

std::string OSInfo::getNetworkInfo()
{
	std::string networkInfo;
	std::string IpInfo;
	std::string MacInfo;
	
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();

	unsigned long adapter_size = sizeof(IP_ADAPTER_INFO);

	int ret = GetAdaptersInfo(pIpAdapterInfo, &adapter_size);

	if (ret == ERROR_BUFFER_OVERFLOW)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[adapter_size];
		ret = GetAdaptersInfo(pIpAdapterInfo, &adapter_size);
	}

	if (ret == ERROR_SUCCESS)
	{
		int card_index = 0;

		while (pIpAdapterInfo)
		{
			IpInfo.clear();
			MacInfo.clear();
			networkInfo += "NetworkCard " + std::to_string(++card_index) + "\n";
			networkInfo += "Network Card Name: " + std::string(pIpAdapterInfo->AdapterName) + "\n";
			networkInfo += "Network Card Description: " + std::string(pIpAdapterInfo->Description) + "\n";

			PIP_ADDR_STRING pIpAddr = &(pIpAdapterInfo->IpAddressList);
			while (pIpAddr)
			{
				char local_ip[128]{ 0 };
				strcpy(local_ip, pIpAddr->IpAddress.String);
				IpInfo += std::string(local_ip) + "\n";
				pIpAddr = pIpAddr->Next;
				
			}

			char local_mac[128]{ 0 };
			int char_index = 0;
			for (int i = 0; i < pIpAdapterInfo->AddressLength; i++)
			{
				char temp_str[10]{ 0 };
				sprintf_s(temp_str, "%02X-", pIpAdapterInfo->Address[i]);
				strcpy(local_mac + char_index, temp_str);
				char_index += 3;
			}
			local_mac[17] = '\0';

			MacInfo += std::string(local_mac) + "\n";
			//break;

			pIpAdapterInfo = pIpAdapterInfo->Next;

			networkInfo += IpInfo + MacInfo;
		}
	}
	if (pIpAdapterInfo)
		delete pIpAdapterInfo;
	return networkInfo;
}

std::string OSInfo::getProcessInfo()
{
	int pid = GetCurrentProcessId();
	return std::to_string(pid);
}

std::string OSInfo::execCmd(const char * cmd)
{
	char buffer[128]{ 0 };
	std::string result;

	FILE *fp = _popen(cmd, "r");
	if (!fp) throw std::runtime_error("_popen() failed!");
	while (!feof(fp))
	{
		if (fgets(buffer, 128, fp) != NULL)
			result += buffer;
	}
	_pclose(fp);
	return result;
	return std::string();
}
