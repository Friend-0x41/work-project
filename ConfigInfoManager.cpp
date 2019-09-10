#include "ConfigInfoManager.h"
#include <fstream>
#include "InputManager.h"
#include <cstdlib>
#ifdef __linux__
#include <string.h>
#include <unistd.h>
#endif // __linux__


ConfigInfoManager::ConfigInfoManager()
	:_configFileName("Config.bin")
{
	load();
}

std::string ConfigInfoManager::get_name()
{
	std::string name;
	if (_info.name[0] == '\0')
	{
		name = InputManager::getInstance()->get_name();
		if (!name.empty())
		{
			memcpy(_info.name, name.c_str(), name.length());
			save();
		}
	}
	else
	{
		name = _info.name;
	}
	return name;
}

std::string ConfigInfoManager::get_password()
{
	std::string password;
	if (_info.password[0] == '\0')
	{
		password = InputManager::getInstance()->get_password();
		if (!password.empty())
		{
			memcpy(_info.password, password.c_str(), password.length());
			save();
		}
	}
	else
	{
		password = _info.password;
	}
	return password;
}

int ConfigInfoManager::get_last_status()
{
	return _info.currentCommandType;
}

void ConfigInfoManager::set_last_status(int status)
{
	_info.currentCommandType = status;
	save();
}

void ConfigInfoManager::clear()
{
	memset(_info.name, 0, 21);
	memset(_info.password, 0, 21);
	save();
}

void ConfigInfoManager::load()
{
	std::ifstream in;
	in.open(_configFileName, std::ios_base::in);
	if (!in.is_open())
	{
		return;
	}
	in.read((char*)& _info, sizeof(ConfigInfo));
	in.close();
}

void ConfigInfoManager::save()
{
	std::ofstream out;
	out.open(_configFileName, std::ios_base::out);
	if (!out.is_open())
	{
		return;
	}
	out.write((char*)& _info, sizeof(ConfigInfo));
	out.close();
}
