#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "SysteminfoLoader.h"
#include <vector>

class SystemInfo
{
public:
	SystemInfo()
	{
		_cpuInfos = SysteminfoLoader::getInstance()->get_cpuInfos();
		_diskInfos = SysteminfoLoader::getInstance()->get_diskInfos();
		_ip = SysteminfoLoader::getInstance()->get_ip();
		_mac = SysteminfoLoader::getInstance()->get_mac();
		_memoryInfo = SysteminfoLoader::getInstance()->get_memoryInfo();
		_netInfo = SysteminfoLoader::getInstance()->get_netInfos();
		_osInfo = SysteminfoLoader::getInstance()->get_OsInfo();
	}

	std::string get_json()
	{
		rapidjson::Document doc;
		doc.SetObject();
		auto& allocator = doc.GetAllocator();
		{
			rapidjson::Document arr(rapidjson::kArrayType);
			for (auto it = _cpuInfos.begin(); it != _cpuInfos.end(); ++it)
			{;
				arr.PushBack(it->generated_json_obj(allocator), allocator);
			}
			doc.AddMember("cpuInfo", arr, allocator);
		}
		{
			rapidjson::Document arr(rapidjson::kArrayType);
			for (auto it = _diskInfos.begin(); it != _diskInfos.end(); ++it)
			{
				arr.PushBack(it->generated_json_obj(allocator), allocator);
			}
			doc.AddMember("diskInfo", arr, allocator);
		}
		doc.AddMember("ip", rapidjson::Value(_ip.c_str(), _ip.length()), allocator);
		doc.AddMember("mac", rapidjson::Value(_mac.c_str(), _mac.length()), allocator);
		doc.AddMember("memoryInfo", _memoryInfo.generated_json_obj(allocator), allocator);
		{
			rapidjson::Document arr(rapidjson::kArrayType);
			for (auto it = _netInfo.begin(); it != _netInfo.end(); ++it)
			{
				arr.PushBack(it->generated_json_obj(allocator), allocator);
			}
			doc.AddMember("netInfo", arr, allocator);
		}
		;
		doc.AddMember("osInfo", _osInfo.generated_json_obj(allocator), allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		return buffer.GetString();
	}

private:

	std::vector<CpuInfo> _cpuInfos;
	std::vector<DiskInfo> _diskInfos;
	std::string _ip;
	std::string _mac;
	MemoryInfo _memoryInfo;
	std::vector<NetInfo> _netInfo;
	OsInfo _osInfo;
};

#endif