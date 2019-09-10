#ifndef LOGTOOL_H_
#define LOGTOOL_H_

#include <iostream>
#include <string>
#include <fstream>
#include "SingleClass.hpp"

class LogTool : public SingleClass<LogTool>
{
public:

	LogTool();

	~LogTool();

	template<typename First, typename... Args>
	void Log(First head, Args...args)
	{
		if (!_out.is_open())
		{
			return;
		}
		if (_end)
		{
			_out << getCurrentTime() << " ";
			_end = false;
		}
		_out << head;
		_out.flush();
		Log(args...);
	}

	void setEndWithNewline(bool value);

	template<typename T>
	void Log(T arg)
	{
		_out << arg;
		if (_logNewline)
		{
			_out << '\n';
		}
		_out.flush();
		_end = true;
	}

	void log(const std::string msg);

private:

	std::string getCurrentTime();

	bool _logNewline;

	std::ofstream _out;

	const std::string _logFileName;

	bool _end = true;
};

#endif
