#include "msyncsocket.h"
#include "MSyncHttpRequest.h"
#include "LogTool.h"
#include "JsonTool.hpp"
#include "CoreApplication.h"
#include "SingleLock.hpp"
#include "VMWareCheck.hpp"
#include "ProcessStatusCheck.h"
#include <thread>
#include "Version.h"

int main(int argc, char* argv[])
{
#ifdef WIN32
	system("CHCP 65001");
#endif
	/*if (argc != 3)
	{
		return -1;
	}
	std::string p1 = argv[1];
	std::string p2 = argv[2];
	if (p1 != "mniojek489416514854581jhioajhc" && (p2 != "789a4c8e4a51dv5e48ag415a64e8c1456ea4" && p2 != "jiovmoerajoin54v485a748541v587e89a454v8e4a564d584c8541e8a74654"))
	{
		return -2;
	}
	SingleLock lock;
	lock.check_lock();
	if (!VMWareCheck::getInstance()->check())
	{
		std::cout << "system exception" << std::endl;
		return -1;
	}*/
	Version version;
	version.write();
	/*std::thread t{ []()
	{
		while (true)
		{
			if (!ProcessStatusCheck::getInstance()->isRunning())
			{
				ProcessStatusCheck::getInstance()->stop();
			}
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}
	} };
	if (t.joinable())
	{
		t.detach();
	}*/
	CoreApplication app(argc, argv);
	return app.exec();
}