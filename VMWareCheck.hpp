#ifndef VMWARECHECK_H_
#define VMWARECHECK_H_

#include "SingleClass.hpp"
#ifdef _WIN32
#include <Windows.h>
#endif

class VMWareCheck : public SingleClass<VMWareCheck>
{
public :

	bool check()
	{
#ifdef _WIN32
		
#elif __linux__

#endif
		return true;
	}
#ifdef _WIN32

#elif __linux__

#endif
};

#endif