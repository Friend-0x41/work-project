#include "InputManager.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <thread>
#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#else
#include <string.h>
#include <termio.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef __linux__
static char getch()
{
	termios tm, tm_old;
	int fd = 0;
	char ch;
	if(tcgetattr(fd,&tm) < 0)
	{
		exit(-1);
	}
	tm_old = tm;
	cfmakeraw(&tm);
	if(tcsetattr(fd,TCSANOW, &tm) < 0)
	{
		exit(-1);
	}
	ch = getchar();
	if(tcsetattr(fd,TCSANOW, &tm_old) < 0)
	{
		exit(-1);
	}
	return ch;
}

#endif

std::string InputManager::get_name() const
{
	std::cout << "please input name: " << std::flush;
	std::string name;
	std::getline(std::cin, name);
	if (name.length() > _lengthLimit)
	{
		return std::string();
	}
	return name;
}

std::string InputManager::get_command() const
{
	std::cout << "command: " << std::flush;
	std::string s;
	std::getline(std::cin, s);
	return s;
}

std::string InputManager::get_password() const
{
	std::cout << "please input password: " << std::flush;
	char buf[_lengthLimit];
	memset(buf, 0, _lengthLimit);
	int count = 0;
	char ch = 0;
	char end = '\r';
	int skip = 0;
	while ((ch = getch()) != end)
	{
		if (skip)
		{
#ifdef __linux__
			if(skip == 3)
			{
				if(ch >= 'A' && ch <= 'Z')
				{
					skip = 0;
					continue;
				}
			}
			else if(skip == 2)
			{
				if(ch == 91)
				{
					++skip;	
					continue;
				}
			}
#endif
			--skip;
			continue;
		}
#ifdef _WIN32
		if (ch == '\0' || ch < 0)
#elif __linux__
		if (ch == 27)
#endif
		{
#ifdef _WIN32
			skip = 1;
#elif __linux__
			skip = 2;
#endif
			continue;
		}
#ifdef _WIN32
		if (ch == '\b')
#elif __linux__
		if (ch == 127)
#endif
		{
			if (count == 0)
			{
				continue;
			}
			putchar('\b');
			putchar(' ');
			putchar('\b');
			buf[--count] = '\0';
			continue;
		}
		if (count == (_lengthLimit - 1))
		{
			continue;
		}
		if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z') && (ch != '_') || (ch == ' '))
		{
			continue;
		}
		putchar('*');
		buf[count++] = ch;
	}
	if (count >= _lengthLimit)
	{
		std::cout << "\npassword too large " << std::endl;
		return std::string();
	}
	putchar('\n');
	buf[_lengthLimit - 1] = '\0';
	return std::string(buf);
}

double InputManager::get_double(const std::string& msg) const
{
	std::cout << msg << std::flush;
	double n;
	std::cin >> n;
	std::cin.get();
	if (n < 0)
	{
		return -1;
	}
	return n;
}

int InputManager::get_int(const std::string& msg) const
{
	std::cout << msg << std::flush;
	int n;
	std::cin >> n;
	std::cin.get();
	if (n < 0)
	{
		return -1;
	}
	return n;
}

std::string InputManager::get_dir() const
{
	std::cout << "please input path: ";
	std::string tmp;
	std::getline(std::cin, tmp);
#ifdef _WIN32
	WIN32_FIND_DATA fd = { 0 };
	HANDLE h = FindFirstFile(tmp.c_str(), &fd);
	if(h != INVALID_HANDLE_VALUE)
	{
		FindClose(h);
		return tmp;
	}
#elif __linux__
	if (access(tmp.c_str(), F_OK | R_OK | W_OK) != -1)
	{
		return tmp;
	}
#endif
	return std::string();
}

void InputManager::waitKey() const noexcept
{
	while (getch() == '\0')
	{
#ifdef _WIN32
		Sleep(20);
#elif __linux__
		sleep(20);
#endif
	}
}
