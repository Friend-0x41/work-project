#include "ProcessStatusCheck.h"
#include <process.h>
#include <Tlhelp32.h>
#include <tchar.h>

bool ProcessStatusCheck::isRunning() const
{
	return GetProcessId() != 0;
}

void ProcessStatusCheck::stop()
{
	exit(-1);
}

DWORD ProcessStatusCheck::GetProcessId() const
{
	PROCESSENTRY32 pe;
	DWORD id = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(2, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe)) return 0;
	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE) break;
		if (strcmp(pe.szExeFile, _processName.c_str()) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	} while (TRUE);
	CloseHandle(hSnapshot);
	return id;
}
