#include "Version.h"
#include <fstream>

Version::Version()
	:_versionFileName("version.bin")
{

}

void Version::write()
{
	std::ofstream out;
	out.open(_versionFileName, std::ios_base::out | std::ios_base::binary);
	if (!out.is_open())
	{
		exit(-1);
	}
	out.write((char*)& _data, sizeof(VersionData));
	out.close();
}
