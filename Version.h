#ifndef VERSION_H_
#define VERSION_H_

#include <string>

class Version
{
	struct VersionData
	{
		char data[100] = "4.1.5";
	};
public:

	Version();

	void write();

private:

	const std::string _versionFileName;

	std::string _currentVersion;

	VersionData _data;
};

#endif