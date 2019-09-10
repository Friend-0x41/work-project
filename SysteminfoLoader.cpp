#include "SysteminfoLoader.h"
#ifdef _WIN32
#include <tchar.h>
#include <Windows.h>
#include <cstdlib>
#pragma comment(lib, "Netapi32.lib")
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#elif __linux__
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

std::vector<CpuInfo> SysteminfoLoader::get_cpuInfos()
{
	std::vector<CpuInfo> infos;
#ifdef _WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int processorCount = sysInfo.dwNumberOfProcessors;
	char buf[1024];
	memset(buf, 0, 1024);
	for (int i = 0; i < processorCount; ++i)
	{
		CpuInfo cpuInfo;
		HKEY key;
		char numBuf[10];
		memset(numBuf, 0, 10);
		sprintf(numBuf, "%d", i);
		std::string s = "HARDWARE\\DESCRIPTION\\SYSTEM\\CentralProcessor\\" + std::string(numBuf);
		DWORD size = 1024;
		DWORD dwSzType = REG_DWORD;
		if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, s.c_str(), 0, KEY_READ, &key))
		{
			if (ERROR_SUCCESS == RegQueryValueEx(key, _T("~MHz"), 0, &dwSzType, (LPBYTE)& buf, &size))
			{
				cpuInfo.mhz = *((int*)buf);
			}
		}
		dwSzType = REG_SZ;
		memset(buf, 0, size);
		if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, s.c_str(), 0, KEY_READ, &key))
		{
			if (ERROR_SUCCESS == RegQueryValueEx(key, _T("Identifier"), 0, &dwSzType, (LPBYTE)& buf, &size))
			{
				cpuInfo.model = buf;
			}
		}
		dwSzType = REG_SZ;
		memset(buf, 0, size);
		if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, s.c_str(), 0, KEY_READ, &key))
		{
			if (ERROR_SUCCESS == RegQueryValueEx(key, _T("VendorIdentifier"), 0, &dwSzType, (LPBYTE)& buf, &size))
			{
				cpuInfo.vendor = buf;
			}
		}
		RegCloseKey(key);
		cpuInfo.cacheSize = 256;
		CpuUsageRate rate = get_cpu_usagerate();
		memset(numBuf, 0, 10);
		sprintf(numBuf, "%.1f%%", rate.sys);
		cpuInfo.sys = numBuf;
		memset(numBuf, 0, 10);
		sprintf(numBuf, "%.1f%%", rate.user);
		cpuInfo.use = numBuf;
		memset(numBuf, 0, 10);
		sprintf(numBuf, "%.1f%%", rate.idle);
		cpuInfo.wait = numBuf;
		memset(numBuf, 0, 10);
		sprintf(numBuf, "%.1f%%", rate.sys + rate.user);
		cpuInfo.combined = numBuf;
		infos.push_back(cpuInfo);
	}
#elif __linux__
	std::ifstream in;
	in.open("/proc/cpuinfo", std::ios_base::in);
	if (!in.is_open())
	{
		return infos;
	}
	std::string s;
	std::vector<std::map<std::string, std::string>> coreInfos;
	while (!std::getline(in, s).eof())
	{
		std::map<std::string, std::string> coreInfo;
		while (!s.empty())
		{
			size_t ti = s.find_first_of('\t');
			*(s.begin() + ti) = '\0';
			size_t i = s.find_first_of(':');
			std::string key(s.begin(), s.begin() + i);
			std::string value(s.begin() + i + 1, s.end());
			const char* str = key.c_str();
			coreInfo.insert(std::make_pair(str, value));
			std::getline(in, s);
		}
		coreInfos.push_back(coreInfo);
	}
	for (auto& coreInfo : coreInfos)
	{
		CpuInfo info;
		info.cacheSize = std::stoi(coreInfo.at("cache size"));
		info.combined = "0.0%";
		info.index = std::stoi(coreInfo.at("processor"));
		info.mhz = (int)std::stod(coreInfo.at("cpu MHz"));
		info.model = coreInfo.at("model name");
		info.nick = "0.0%";
		info.sys = "0.0%";
		info.use = "0.0%";
		info.vendor = coreInfo.at("vendor_id");
		info.wait = "0.0%";
		infos.push_back(info);
	}
	in.close();
#endif
	return infos;
}

std::vector<DiskInfo> SysteminfoLoader::get_diskInfos()
{
	std::vector<DiskInfo> infos;
#ifdef _WIN32
	DWORD diskCount = GetLogicalDrives();
	int num = 0;
	while(diskCount)
	{
		if (diskCount & 1)
		{
			++num;
		}
		diskCount = diskCount >> 1;
	}
	char* buf = new char[num * 4 + 1];
	char* p = buf;
	memset(buf, 0, num * 4);
	GetLogicalDriveStrings(num * 4 + 1, buf);
	for (int i = 0; i < num; ++i)
	{
		DiskInfo info;
		info.devName = *p;
		info.index = i;
		info.dirName = *p;
		unsigned long long avail;
		unsigned long long total;
		unsigned long long free;
		bool b = GetDiskFreeSpaceEx(p, (PULARGE_INTEGER)&avail, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
		info.avail = avail / 1024 / 1024 / 1024;
		info.free = free / 1024 / 1024 / 1024;
		info.total = total / 1024 / 1024 / 1024;
		info.used = info.total - info.avail;
		info.type = GetDriveType(p);
		infos.push_back(info);
		p += 4;
	}
	delete [] buf;
#elif __linux__
	system("df > .df.txt");
	std::ifstream in;
	in.open(".df.txt", std::ios_base::in);
	std::string tmp;
	std::getline(in, tmp);
	std::getline(in, tmp);
	int index = 0;
	while (!std::getline(in, tmp).eof())
	{
		size_t pos = tmp.find_first_of(' ');
		pos = tmp.find_first_not_of(' ', pos);
		size_t endPos = tmp.find_first_of(' ', pos);
		unsigned long long size = std::stoll(std::string(tmp.begin() + pos, tmp.begin() + endPos));
		size = size / 1024 / 1024;
		if (size == 0)
		{
			continue;
		}
		DiskInfo info;
		info.total = size;
		pos = tmp.find_first_not_of(' ', endPos);
		endPos = tmp.find_first_of(' ', pos);
		unsigned long long used = std::stoll(std::string(tmp.begin() + pos, tmp.begin() + endPos)) / 1024 / 1024;
		info.free = info.total - used;
		info.avail = info.free;
		info.used = used;
		pos = tmp.find_first_of('%');
		info.dirName = std::string(tmp.begin() + pos + 1, tmp.end());
		info.index = index++;
		infos.push_back(info);
	}
	in.close();
	system("rm -f .df.txt");
#endif
	return infos;
}

std::string SysteminfoLoader::get_ip()
{
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2, 0);
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				LPCSTR ip = inet_ntoa(*(struct in_addr*) * hostinfo->h_addr_list);
				return ip;
			}
		}
		WSACleanup();
	}
	return "";
#elif __linux__
	auto names = get_netCard_name();
	if (names.size() == 0)
	{
		return "";
	}
	return get_netinfo(names[0]).address;
#endif
}

std::string SysteminfoLoader::get_mac()
{
#ifdef _WIN32
	return _get_mac();
#elif __linux__
	char buf[100] = { 0 };
	if (get_mac((char*)buf))
	{
		return buf;
	}
	return "";
#endif
}

MemoryInfo SysteminfoLoader::get_memoryInfo()
{
	MemoryInfo memoryInfo;
#ifdef _WIN32
	MEMORYSTATUS status;
	GlobalMemoryStatus(&status);
	memoryInfo.free = status.dwAvailPhys / 1024;
	memoryInfo.swap_total = status.dwTotalPageFile / 1024;
	memoryInfo.swap_free = status.dwAvailPageFile / 1024;
	memoryInfo.total = status.dwTotalPhys / 1024 ;
	memoryInfo.used = memoryInfo.total - memoryInfo.free;
#elif __linux__
	std::ifstream in;
	in.open("/proc/meminfo", std::ios_base::in);
	if (in.is_open())
	{
		std::map<std::string, long long> kvps;
		std::string tmp;
		while (!std::getline(in, tmp).eof())
		{
			size_t qi = tmp.find_first_of(':');
			tmp.at(qi) = '\0';
			std::string key(tmp.begin(), tmp.begin() + qi);
			std::string value(tmp.begin() + qi + 1, tmp.end());
			value += '\0';
			kvps.insert(std::make_pair(key, std::stoll(value)));
		}
		memoryInfo.free = kvps.at("MemFree");
		memoryInfo.swap_free = kvps.at("SwapFree");
		memoryInfo.swap_total = kvps.at("SwapTotal");
		memoryInfo.swap_used = memoryInfo.swap_total - memoryInfo.free;
		memoryInfo.total = kvps.at("MemTotal");
		memoryInfo.used = memoryInfo.total - memoryInfo.free;
		in.close();
	}
#endif
	return memoryInfo;
}

std::vector<NetInfo> SysteminfoLoader::get_netInfos()
{
	std::vector<NetInfo> infos;
#ifdef _WIN32
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO;
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			NetInfo info;
			//cout << "网卡名称：" << pIpAdapterInfo->AdapterName << endl;
			info.description = pIpAdapterInfo->Description;
			std::string type;
			switch (pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_OTHER:
				type = "OTHER";
				break;
			case MIB_IF_TYPE_ETHERNET:
				type = "ETHERNET";
				break;
			case MIB_IF_TYPE_TOKENRING:
				type = "TOKENRING";
				break;
			case MIB_IF_TYPE_FDDI:
				type = "FDDI";
				break;
			case MIB_IF_TYPE_PPP:
				type = "PPP";
				break;
			case MIB_IF_TYPE_LOOPBACK:
				type = "LOOPBACK";
				break;
			case MIB_IF_TYPE_SLIP:
				type = "SLIP";
				break;
			default:

				break;
			}
			info.type = type;
			std::string mac;
			char macBuf[100];
			char* p = macBuf;
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
			{
				if (i < pIpAdapterInfo->AddressLength - 1)
				{
					p += sprintf(p, "%02X", pIpAdapterInfo->Address[i]);
				}
				else
				{
					p += sprintf(p, "%02X\n", pIpAdapterInfo->Address[i]);
				}
			}
			macBuf[99] = '\0';
			info.mac = macBuf;
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			//cout << "该网卡上的IP数量：" << ++IPnumPerNetCard << endl;
			info.address = pIpAddrString->IpAddress.String;
			info.netMask = pIpAddrString->IpMask.String;
			info.broadcast = pIpAdapterInfo->GatewayList.IpAddress.String;
			//pIpAddrString = pIpAddrString->Next;
			pIpAdapterInfo = pIpAdapterInfo->Next;
			infos.push_back(info);
		}

	}
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
#elif __linux__
	auto names = get_netCard_name();
	for (auto name : names)
	{
		infos.push_back(get_netinfo(name));
	}
#endif
	return infos;
}

OsInfo SysteminfoLoader::get_OsInfo()
{
	OsInfo osInfo;
#ifdef _WIN32
	char buf[1024];
	memset(buf, 0, 1024);
	HKEY key;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key))
	{
		DWORD size = 1024;
		DWORD dwSzType = REG_SZ;
		if (ERROR_SUCCESS == RegQueryValueEx(key, _T("ProductName"), 0, &dwSzType, (LPBYTE)& buf, &size))
		{
			osInfo.description = buf;
			osInfo.vendorName = buf;
		}
	}
	RegCloseKey(key);
	if (IsBit32())
	{
		osInfo.arch = "x86";
		osInfo.dataModel = "32";
	}
	else
	{
		osInfo.arch = "x86_64";
		osInfo.dataModel = "64";
	}
#elif __linux__
	system("getconf LONG_BIT > .arch.txt");
	std::ifstream in;
	in.open(".arch.txt", std::ios_base::in);
	int arch;
	in >> arch;
	if (arch == 32)
	{
		osInfo.arch = "x86";
		osInfo.dataModel = "32";
	}
	else if (arch == 64)
	{
		osInfo.arch = "x86_64";
		osInfo.dataModel = "64";
	}
	in.close();
	system("rm -f .arch.txt");
	system("cat /etc/issue > .version.txt");
	in.open(".version.txt", std::ios_base::in);
	std::string tmp;
	std::getline(in, tmp);
	size_t pos = tmp.find('\\');
	if (pos == std::string::npos)
	{
		osInfo.vendorName = tmp;
	}
	else
	{
		osInfo.vendorName = std::string(tmp.begin(), tmp.begin() + pos);
	}
	in.close();
	system("cat /proc/version > .version.txt");
	in.open(".version.txt", std::ios_base::in);
	std::getline(in, tmp);
	pos = tmp.find('(');
	if (pos == std::string::npos)
	{
		osInfo.description = tmp;
	}
	else
	{
		osInfo.description = std::string(tmp.begin(), tmp.begin() + pos);
	}
	in.close();
	system("rm -f .version.txt");
#endif
	int i = 1;
	char* p = (char*)& i;
	if (*p == 1)
	{
		osInfo.cpuEndian = "Little";
	}
	else
	{
		osInfo.cpuEndian = "Big";
	}
	return osInfo;
}

#ifdef _WIN32
CpuUsageRate SysteminfoLoader::get_cpu_usagerate()
{
	Sleep(10);
	FILETIME ftStartIdleTime, ftStartKernelTime, ftStartUserTime;
	FILETIME ftEndIdleTime, ftEndKernelTime, ftEndUserTime;

	GetSystemTimes(&ftStartIdleTime, &ftStartKernelTime, &ftStartUserTime);
	Sleep(100);
	GetSystemTimes(&ftEndIdleTime, &ftEndKernelTime, &ftEndUserTime);

	unsigned long long nDeltaIdleTime = FileTimeSub(ftEndIdleTime, ftStartIdleTime);
	unsigned long long nDeltaKernelTime = FileTimeSub(ftEndKernelTime, ftStartKernelTime);
	unsigned long long nDeltaUserTime = FileTimeSub(ftEndUserTime, ftStartUserTime);

	if (nDeltaKernelTime + nDeltaUserTime == 0)
	{
		return CpuUsageRate();
	}

	float nCPUUsageRate = ((nDeltaKernelTime + nDeltaUserTime - nDeltaIdleTime) * 100.0) / (nDeltaKernelTime + nDeltaUserTime);
	char buf[100];
	memset(buf, 0, 100);
	sprintf(buf, "%.1f", nCPUUsageRate);
	CpuUsageRate rate;
	rate.idle = 1 - std::stof(buf);
	memset(buf, 0, 100);
	sprintf(buf, "%.1f", (double)nDeltaKernelTime / (double)(nDeltaUserTime + nDeltaKernelTime) * (1 - rate.idle));
	rate.sys = std::atof(buf);
	rate.user = 1 - rate.idle - rate.sys;
	return rate;
}

std::string SysteminfoLoader::_get_mac() const
{
	std::string s;
	LANA_ENUM adapterList;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char*)& adapterList;
	Ncb.ncb_length = sizeof(adapterList);
	Netbios(&Ncb);
	// 取得MAC
	for (int i = 0; i < adapterList.length; ++i)
	{
		if (GetAdapterInfo(adapterList.lana[i], s))
			return s;
	}
	return std::string();
}

bool SysteminfoLoader::GetAdapterInfo(int adapterNum, std::string& macOUT) const
{
	NCB Ncb;
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBRESET; 
	Ncb.ncb_lana_num = adapterNum;
	if (Netbios(&Ncb) != NRC_GOODRET)
		return false;

	// 准备取得接口卡的状态块
	memset(&Ncb, sizeof(Ncb), 0);
	Ncb.ncb_command = NCBASTAT;
	Ncb.ncb_lana_num = adapterNum;
	strcpy((char*)Ncb.ncb_callname, "*");
	struct ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER nameBuff[30];
	}adapter;
	memset(&adapter, sizeof(adapter), 0);
	Ncb.ncb_buffer = (unsigned char*)& adapter;
	Ncb.ncb_length = sizeof(adapter);
	if (Netbios(&Ncb) != 0)
		return false;
	char acMAC[32];
	sprintf(acMAC, "%02X%02X%02X%02X%02X%02X",
		int(adapter.adapt.adapter_address[0]),
		int(adapter.adapt.adapter_address[1]),
		int(adapter.adapt.adapter_address[2]),
		int(adapter.adapt.adapter_address[3]),
		int(adapter.adapt.adapter_address[4]),
		int(adapter.adapt.adapter_address[5]));
	macOUT = acMAC;
	return true;
}
#endif

#ifdef _WIN32
unsigned long long SysteminfoLoader::FileTimeSub(FILETIME ftEndTime, FILETIME ftStartTime)
{
	unsigned long long nDeltaTime;

	unsigned long long nEndTime = (unsigned long long)ftEndTime.dwHighDateTime << 32 | ftEndTime.dwLowDateTime;
	unsigned long long nStartTime = (unsigned long long)ftStartTime.dwHighDateTime << 32 | ftStartTime.dwLowDateTime;

	nDeltaTime = nEndTime - nStartTime;

	return nDeltaTime;
}

bool SysteminfoLoader::IsBit32()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return false;
	}
	else
	{
		return true;
	}
}
#endif

#ifdef __linux__
bool SysteminfoLoader::get_mac(char* mac)

{
	struct ifreq tmp;
	int sock_mac;
	char mac_addr[30];
	sock_mac = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_mac == -1) {
		perror("create socket fail\n");
		return false;
	}
	memset(&tmp, 0, sizeof(tmp));
	auto names = get_netCard_name();
	if (names.size() == 0)
	{
		return false;
	}
	strncpy(tmp.ifr_name, names[0].c_str(), sizeof(tmp.ifr_name) - 1);
	if ((ioctl(sock_mac, SIOCGIFHWADDR, &tmp)) < 0) {
		printf("mac ioctl error\n");
		return false;
	}
	sprintf(mac_addr, "%02x%02x%02x%02x%02x%02x",
		(unsigned char)tmp.ifr_hwaddr.sa_data[0],
		(unsigned char)tmp.ifr_hwaddr.sa_data[1],
		(unsigned char)tmp.ifr_hwaddr.sa_data[2],
		(unsigned char)tmp.ifr_hwaddr.sa_data[3],
		(unsigned char)tmp.ifr_hwaddr.sa_data[4],
		(unsigned char)tmp.ifr_hwaddr.sa_data[5]
	);
	close(sock_mac);
	memcpy(mac, mac_addr, strlen(mac_addr));
	return true;
}

std::vector<std::string> SysteminfoLoader::get_netCard_name()
{
	std::ifstream in;
	in.open("/proc/net/dev", std::ios_base::in);
	if (!in.is_open())
	{
		return std::vector<std::string>();
	}
	std::vector<std::string> names;
	std::string tmp;
	while (!std::getline(in, tmp).eof())
	{
		size_t pos = tmp.find(':');
		if (pos == std::string::npos)
		{
			continue;
		}
		names.push_back(std::string(tmp.begin(), tmp.begin() + pos));
	}
	in.close();
	return names;
}

NetInfo SysteminfoLoader::get_netinfo(std::string devName)
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
	    fprintf(stderr, "Create socket failed!errno=%d", errno);
	    return NetInfo();
	}
	NetInfo netInfo;
	struct ifreq ifr;
	unsigned char mac[6];
	unsigned long nIP, nNetmask, nBroadIP;
	
	strcpy(ifr.ifr_name, devName.c_str());
	if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)
	{
	    return NetInfo();
	}
	memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));
	char mac_tmp[100] = { 0 };
	sprintf(mac_tmp, "%02x%02x%02x%02x%02x%02x",
	        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	netInfo.mac = mac_tmp;
	strcpy(ifr.ifr_name, devName.c_str());
	if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
	{
	    nIP = 0;
	}
	else
	{
	    nIP = *(unsigned long*)& ifr.ifr_broadaddr.sa_data[2];
	}
	netInfo.address = inet_ntoa(*(in_addr*)& nIP);
	
	strcpy(ifr.ifr_name, devName.c_str());
	if (ioctl(s, SIOCGIFBRDADDR, &ifr) < 0)
	{
	    nBroadIP = 0;
	}
	else
	{
	    nBroadIP = *(unsigned long*)& ifr.ifr_broadaddr.sa_data[2];
	}
	netInfo.broadcast = inet_ntoa(*(in_addr*)& nBroadIP);
	
	strcpy(ifr.ifr_name, devName.c_str());
	if (ioctl(s, SIOCGIFNETMASK, &ifr) < 0)
	{
	    nNetmask = 0;
	}
	else
	{
	    nNetmask = *(unsigned long*)& ifr.ifr_netmask.sa_data[2];
	}
	netInfo.netMask = inet_ntoa(*(in_addr*)& nNetmask);
	close(s);
	return netInfo;
}
#endif