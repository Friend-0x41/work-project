#ifndef COREAPPLICATION_H_
#define COREAPPLICATION_H_

#include "Command.hpp"
#include <string>

class CoreApplication
{
public:

	CoreApplication(int argc, char* argv[]);

	int exec();
private:

	std::string _pa2;

	int _argc;
};

#endif