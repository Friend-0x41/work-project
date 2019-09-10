#pragma once

#include "SingleClass.hpp"
#include <string>
#include <iostream>
#include "InputManager.h"
#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#endif

struct MemoryLock
{
	int mark = 0;
};

class SingleLock
{
public:

	SingleLock()
		:_fileName("launchermemorylock0x666"), _lock(0)
#ifdef _WIN32
		, hMapFile(nullptr)
#endif
	{

	}
#ifdef _WIN32
	void check_lock()
	{
		HANDLE hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(MemoryLock),
			_fileName.c_str());
		if (hMapFile == 0)
		{
			clear();
			exit(-1);
		}
		_lock = (MemoryLock*)MapViewOfFile(
			hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			sizeof(MemoryLock)
		);
		if (!_lock)
		{
			clear();
			exit(-1);
		}
		if (_lock->mark == get_key())
		{
			std::cout << "Only one instance is allowed to run" << std::endl;
			clear();
			InputManager::getInstance()->waitKey();
			exit(-1);
		}
		else
		{
			_lock->mark = get_key();
		}
	}

	~SingleLock()
	{
		clear();
	}
#elif __linux__
	void check_lock()
	{
		std::string path = "/var/tmp/" + _fileName;
		_fd = open(path.c_str(), O_CREAT | O_RDWR, 0666);
		if (_fd == -1)
		{
			clear();
			exit(-1);
		}
		if(flock(_fd,LOCK_EX | LOCK_NB) == -1)
		{
			std::cout << "Only one instance is allowed to run" << std::endl;
			clear();
			InputManager::getInstance()->waitKey();
			exit(-1);
		}
	}
#endif

private:

	void clear()
	{
#ifdef _WIN32
		UnmapViewOfFile(_lock);
		CloseHandle(hMapFile);
#elif __linux__
		flock(_fd, LOCK_UN);
		close(_fd);
#endif
		_lock = nullptr;
	}

private:
#ifdef _WIN32
	HANDLE hMapFile;
#else
	int _fd;
#endif

	MemoryLock* _lock;

#ifdef _WIN32
	constexpr int get_key() const
	{
		return 0x6666233;
	}
#endif

	const std::string _fileName;
};
