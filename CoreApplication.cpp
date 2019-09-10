#include "CoreApplication.h"
#include "LogTool.h"
#include <memory>
#include "CommandHandler.h"
#include "EnvironmenConfig.h"
#include "InputManager.h"
#include "ConfigInfoManager.h"

CoreApplication::CoreApplication(int argc, char* argv[])
	:_pa2(argv[2]), _argc(argc)
{

}

int CoreApplication::exec()
{
	std::shared_ptr<CommandHandlerBase> ptr;

	if (_argc == 3 && ConfigInfoManager::getInstance()->get_last_status() == (int)Command::CommandType::Miner
		&& _pa2 == "jiovmoerajoin54v485a748541v587e89a454v8e4a564d584c8541e8a74654")
	{
		ptr = std::make_shared<MinerHandler>();
		if (ptr->exec())
		{
			std::cout << "exec error" << std::endl;
		}
	}
	while (true)
	{
		Command command(InputManager::getInstance()->get_command());
		/*if (_command.get_command_count() == 0)
		{
			EnvironmenConfig ec;
			if (ec.check())
			{
				std::cout << "set environment success" << std::endl;
				InputManager::getInstance()->waitKey();
			}
			std::cout << "set environment failed" << std::endl;
			std::cout << "\npress any key..." << std::endl;
		}
		if (_command.get_command_count() > 1)
		{
			std::cout << "too much paramater" << std::endl;
		}*/
		if (command.get_command(0) == Command::CommandType::None)
		{
			continue;
		}
		switch (command.get_command(0))
		{
		case Command::CommandType::AddMiner:
			ptr = std::make_shared<AddMinerHandler>();
			break;
		case Command::CommandType::Balance:
			ptr = std::make_shared<BalanceHandler>();
			break;
		case Command::CommandType::Extract:
			ptr = std::make_shared<ExtractHandler>();
			break;
		case Command::CommandType::ExtractCancel:
			ptr = std::make_shared<ExtractCancelHandler>();
			break;
		case Command::CommandType::ExtractPledage:
			ptr = std::make_shared<ExtractPledageHandler>();
			break;
		case Command::CommandType::Miner:
			ptr = std::make_shared<MinerHandler>();
			break;
		case Command::CommandType::Pledge:
			ptr = std::make_shared<PledgeHandler>();
			break;
		case Command::CommandType::UpdateMiner:
			ptr = std::make_shared<UpdateMinerHandler>();
			break;
		default:
			std::cout << "paramater error" << std::endl;
			continue;
		}
		ConfigInfoManager::getInstance()->set_last_status((int)command.get_command(0));
		if (ptr->exec())
		{
			std::cout << "exec error" << std::endl;
		}
	}
	return 0;
}
