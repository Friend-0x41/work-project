#ifndef PROCESSSTATUSCHECK_H_
#define PROCESSSTATUSCHECK_H_

#include "SingleClass.hpp"
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif

class ProcessStatusCheck : public SingleClass<ProcessStatusCheck>
{
public:

	bool isRunning() const;

	void stop();

private:

	DWORD GetProcessId() const;

private:

	const std::string  _processName = "Updater.exe";

#ifdef _WIN32
	DWORD _currentId = 0;
#endif
};

#endif;