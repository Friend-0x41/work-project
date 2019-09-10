#include "EnvironmenConfig.h"
#include <cstring>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#elif __linux__
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <linux/limits.h>
#endif

bool EnvironmenConfig::check()
{
#ifdef _WIN32
	char currentDir[1024];
	GetCurrentDirectory(1024, currentDir);
	char buf[4096] = { 0 };
	HKEY key;
	std::string s = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
	DWORD size = 4096;
	DWORD dwSzType = REG_SZ;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, s.c_str(), 0, KEY_READ, &key))
	{
		if (ERROR_SUCCESS == RegQueryValueEx(key, "Path", 0, &dwSzType, (LPBYTE)& buf, &size))
		{
			std::string path = buf;
			if (path.find(currentDir) == std::string::npos)
			{
				size_t pos = path.find_last_of(';');
				if (pos == path.length() - 1)
				{
					path = path + currentDir + ";";
				}
				else
				{
					path = path + ";" + currentDir;
				}
				if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, s.c_str(), 0, KEY_ALL_ACCESS, &key))
				{
					return false;
				}
				if (ERROR_SUCCESS == RegSetValueEx(key, "Path", 0, dwSzType, (const unsigned char*)path.c_str(), strlen(path.c_str())))
				{
					SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment"));
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
	}
	RegCloseKey(key);
	return false;
#elif __linux__
	return modiffyProfile();
#endif
}

#ifdef __linux__
bool EnvironmenConfig::modiffyProfile()
{
	std::ifstream in;
	in.open("/etc/environment", std::ios_base::in);
	if (!in.is_open())
	{
		return false;
	}
	std::string path;
	std::getline(in, path);
	in.close(); 
	size_t pos = path.find_last_of('\"');
	path = std::string(path.begin(), path.begin() + pos) + ":";
	char buf[PATH_MAX] = { 0 };
	int n = readlink("/proc/self/exe", buf, PATH_MAX);
	std::string dir = buf;
	pos = dir.find_last_of('/');
	dir = std::string(dir.begin(), dir.begin() + pos);
	if (dir.empty())
	{
		dir = "/";
	}
	if (path.find(dir) != std::string::npos)
	{
		return true;
	}
	path = path + dir + "\"";
	std::ofstream out;
	out.open("/etc/environment", std::ios_base::out);
	if (!out.is_open())
	{
		std::cout << "couldn't edit environment file, try again with sudo" << std::endl;
		return false;
	}
	out << path;
	out.close();
	return true;
}
#endif