#include "LogTool.h"
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <ctime>
#endif

LogTool::LogTool()
	:_logFileName(".log")
{
	_logNewline = true;
	_out.open(_logFileName, std::ios_base::app);
}

LogTool::~LogTool()
{
	_out.close();
}

void LogTool::setEndWithNewline(bool value)
{
	_logNewline = value;
}

void LogTool::log(const std::string msg)
{
	if (!_out.is_open())
	{
		return;
	}
	_out << getCurrentTime() << " ";
	_out << msg;
	if (_logNewline)
	{
		_out << '\n';
	}
	_out.flush();
}

std::string LogTool::getCurrentTime()
{
	std::stringstream ss;
#ifdef _WIN32
	SYSTEMTIME time;
	GetLocalTime(&time);
	ss << time.wYear << "-";
	ss << time.wMonth << "-";
	ss << time.wDay << " ";
	ss << time.wHour << ":";
	ss << time.wMinute << ":";
	ss << time.wSecond;
#elif __linux__
	time_t t;
	time(&t);
	tm* time = localtime(&t);
	ss << 1900 + time->tm_year << "-";
	ss << 1 + time->tm_mon << "-";
	ss << time->tm_mday << " ";
	ss << time->tm_hour << ":";
	ss << time->tm_min << ":";
	ss << time->tm_sec;
#endif
	return ss.str();
}
