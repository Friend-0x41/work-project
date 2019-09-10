#ifndef USERINFOMANAGER_H_
#define USERINFOMANAGER_H_

#include "SingleClass.hpp"
#include <string>

class ConfigInfoManager : public SingleClass<ConfigInfoManager>
{
	struct ConfigInfo
	{
		char name[21] = { 0 };
		char password[21] = { 0 };
		int currentCommandType = 0;
	};
public:

	ConfigInfoManager();

	std::string get_name();

	std::string get_password();

	int get_last_status();

	void set_last_status(int status);

	void clear();

private:

	void load();

	void save();

private:

	ConfigInfo _info;

	const std::string _configFileName;
};

#endif