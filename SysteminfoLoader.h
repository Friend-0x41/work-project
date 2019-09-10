#ifndef SYSTEMINFO_LOADER_H_
#define SYSTEMINFO_LOADER_H_

#include "SingleClass.hpp"
#include "rapidjson/document.h"
#include <string>
#include "JsonTool.hpp"
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

struct CpuInfo
{
public:
	int cacheSize;

	std::string combined;

	int index = 0;

	int mhz = 0;

	std::string model;

	std::string nick;

	std::string sys;

	std::string use;

	std::string vendor;

	std::string wait;

	rapidjson::Value generated_json_obj(rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetObject();
		value.AddMember("cacheSize", cacheSize, allocator);
		value.AddMember("combined", rapidjson::Value(combined.c_str(), combined.length()), allocator);
		value.AddMember("index", index, allocator);
		value.AddMember("model", rapidjson::Value(model.c_str(), model.length()), allocator);
		value.AddMember("nick", rapidjson::Value(nick.c_str(), nick.length()), allocator);
		value.AddMember("sys", rapidjson::Value(sys.c_str(), sys.length()), allocator);
		value.AddMember("use", rapidjson::Value(use.c_str(), use.length()), allocator);
		value.AddMember("vendor", rapidjson::Value(vendor.c_str(), vendor.length()), allocator);
		value.AddMember("wait", rapidjson::Value(wait.c_str(), wait.length()), allocator);
		return value;
	}
};

struct DiskInfo
{
	int avail = 0;

	std::string devName;

	std::string dirName;

	int diskReads = 0;

	int diskWrites = 0;

	int flags = 0;

	int free = 0;

	int index = 0;

	std::string sysTypeName;

	int total = 0;

	int type = 0;

	std::string typeName;

	int used = 0;

	rapidjson::Value generated_json_obj(rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetObject();
		value.AddMember("avail", rapidjson::Value(avail), allocator);
		value.AddMember("devName", rapidjson::Value(devName.c_str(), devName.length()), allocator);
		value.AddMember("dirName", rapidjson::Value(dirName.c_str(), dirName.length()), allocator);
		value.AddMember("diskReads", rapidjson::Value(diskReads), allocator);
		value.AddMember("diskWrites", rapidjson::Value(diskWrites), allocator);
		value.AddMember("flags", rapidjson::Value(flags), allocator);
		value.AddMember("free", rapidjson::Value(free), allocator);
		value.AddMember("index", rapidjson::Value(index), allocator);
		value.AddMember("sysTypeName", rapidjson::Value(sysTypeName.c_str(), sysTypeName.length()), allocator);
		value.AddMember("total", rapidjson::Value(total), allocator);
		value.AddMember("type", rapidjson::Value(type), allocator);
		value.AddMember("typeName", rapidjson::Value(typeName.c_str(), typeName.length()), allocator);
		value.AddMember("used", rapidjson::Value(used), allocator);
		return value;
	}
};

struct MemoryInfo
{
	int free = 0;

	int swap_free = 0;

	int swap_total = 0;

	int swap_used = 0;

	int total = 0;

	int used = 0;

	rapidjson::Value generated_json_obj(rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetObject();
		value.AddMember("free", rapidjson::Value(free), allocator);
		value.AddMember("swap_free", rapidjson::Value(swap_free), allocator);
		value.AddMember("swap_total", rapidjson::Value(swap_total), allocator);
		value.AddMember("swap_used", rapidjson::Value(swap_used), allocator);
		value.AddMember("total", rapidjson::Value(total), allocator);
		value.AddMember("used", rapidjson::Value(used), allocator);
		return value;
	}
};

struct NetInfo
{
	std::string address;

	std::string broadcast;

	std::string description;

	std::string mac;

	std::string netMask;

	std::string type;

	rapidjson::Value generated_json_obj(rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetObject();
		value.AddMember("address", rapidjson::Value(address.c_str(), address.length()), allocator);
		value.AddMember("broadcast", rapidjson::Value(broadcast.c_str(), broadcast.length()), allocator);
		value.AddMember("description", rapidjson::Value(description.c_str(), description.length()), allocator);
		value.AddMember("mac", rapidjson::Value(mac.c_str(), mac.length()), allocator);
		value.AddMember("netMask", rapidjson::Value(netMask.c_str(), netMask.length()), allocator);
		value.AddMember("type", rapidjson::Value(type.c_str(), type.length()), allocator);
		return value;
	}
};

struct OsInfo
{
	std::string arch;

	std::string cpuEndian;

	std::string dataModel;

	std::string description;

	std::string vendorName;

	rapidjson::Value generated_json_obj(rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value value(rapidjson::kObjectType);
		value.SetObject();
		value.AddMember("arch", rapidjson::Value(arch.c_str(), arch.length()), allocator);
		value.AddMember("cpuEndian", rapidjson::Value(cpuEndian.c_str(), cpuEndian.length()), allocator);
		value.AddMember("dataModel", rapidjson::Value(dataModel.c_str(), dataModel.length()), allocator);
		value.AddMember("description", rapidjson::Value(description.c_str(), description.length()), allocator);
		value.AddMember("vendorName", rapidjson::Value(vendorName.c_str(),vendorName.length()), allocator);
		return value;
	}
};

struct CpuUsageRate
{
	float sys;

	float user;

	float idle;
};

class SysteminfoLoader : public SingleClass<SysteminfoLoader>
{
public:

	std::vector<CpuInfo> get_cpuInfos();

	std::vector<DiskInfo> get_diskInfos();

	std::string get_ip();

	std::string get_mac();

	MemoryInfo get_memoryInfo();

	std::vector<NetInfo> get_netInfos();

	OsInfo get_OsInfo();

private:

#ifdef _WIN32
	CpuUsageRate get_cpu_usagerate();

	std::string _get_mac() const;

	bool GetAdapterInfo(int adapterNum, std::string& macOUT) const;

	unsigned long long FileTimeSub(FILETIME ftEndTime, FILETIME ftStartTime);
	bool IsBit32();
#elif __linux__
	bool get_mac(char* mac);

	std::vector<std::string> get_netCard_name();

	NetInfo get_netinfo(std::string devName);
#endif
};

#endif
