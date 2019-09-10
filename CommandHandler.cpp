#include "CommandHandler.h"
#include <iostream>
#include "MSyncHttpRequest.h"
#include "MSyncHttpResponse.h"
#include "InputManager.h"
#include <memory>
#include "SysteminfoLoader.h"
#include <sstream>
#include "SystemInfo.hpp"
#include "ConfigInfoManager.h"
#include "LogTool.h"
#include "JsonTool.hpp"
#include <thread>
#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif

int BalanceHandler::exec()
{
	MSyncHttpRequest request;
	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}

	std::stringstream ss;
	ss << "name=" << name;
	ss << '&';
	ss << "password=" << password;
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::Balance), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	print_result(*response);
	waitKey();
	return 0;
}

void BalanceHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<BalanceResponseData> data = JsonTool::toJsonClass<BalanceResponseData>(response.data());
	if (!data->result)
	{
		ConfigInfoManager::getInstance()->clear();
		std::cout << "balance failed: " << data->message << std::endl;
	}
	else
	{
		std::cout << "balance success: " << std::endl;
		std::stringstream ss;
		ss << "address: " << data->data.address << "\n";
		ss << "balance: " << data->data.balance << '\n';
		ss << "pledage balance: " << data->data.pledage_balance << '\n';
		ss << "pledage balance used: " << data->data.pledage_balance_used << '\n';
		std::cout << ss.str();
	}
}

int ExtractHandler::exec()
{
	MSyncHttpRequest request;
	int amount = InputManager::getInstance()->get_double("please input amount:");
	if (amount <= 0)
	{
		return -1;
	}

	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}

	std::stringstream ss;
	ss << "amount=" << amount;
	ss << '&';
	ss << "name=" << name;
	ss << '&';
	ss << "password=" << password; 
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::Extract), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	print_result(*response);
	waitKey();
	return 0;
}

void ExtractHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<ExtractResponseData> data = JsonTool::toJsonClass<ExtractResponseData>(response.data());
	if (!data->result)
	{
		std::cout << "extract failed: " << data->message << std::endl;
	}
	else
	{
		std::cout << "extract successed: " << data->message << std::endl;
	}
}

int ExtractCancelHandler::exec()
{
	MSyncHttpRequest request;
	int amount = InputManager::getInstance()->get_double("please input amount:");
	if (amount <= 0)
	{
		return -1;
	}

	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}

	std::stringstream ss;
	ss << "amount=" << amount;
	ss << '&';
	ss << "name=" << name;
	ss << '&';
	ss << "password=" << password;
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::ExtractCancel), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	print_result(*response);
	waitKey();
	return 0;
}

void ExtractCancelHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<ExtractCancelResponseData> data = JsonTool::toJsonClass<ExtractCancelResponseData>(response.data());
	if (!data->result)
	{
		std::cout << "extract cancel failed: " << data->message << std::endl;
	}
	else
	{
		std::cout << "extract cancel successed: " << data->message << std::endl;
	}
}

int ExtractPledageHandler::exec()
{
	MSyncHttpRequest request;
	int amount = InputManager::getInstance()->get_double("please input amount:");
	if (amount <= 0)
	{
		return -1;
	}

	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}

	std::stringstream ss;
	ss << "amount=" << amount;
	ss << '&';
	ss << "name=" << name;
	ss << '&';
	ss << "password=" << password;
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::ExtractPledage), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	print_result(*response);
	waitKey();
	return 0;
}

void ExtractPledageHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<ExtractPledageResponseData> data = JsonTool::toJsonClass<ExtractPledageResponseData>(response.data());
	if (!data->result)
	{
		std::cout << "extract pledage failed: " << data->message << std::endl;
	}
	else
	{
		std::cout << "extract pledage successed: " << data->message << std::endl;
	}
}

int PledgeHandler::exec()
{
	MSyncHttpRequest request;
	int size = InputManager::getInstance()->get_int("please input size:");
	if (size <= 0)
	{
		return -1;
	}

	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}

	std::stringstream ss;
	ss << "size=" << size;
	ss << '&';
	ss << "name=" << name;
	ss << '&';
	ss << "password=" << password;
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::Pledge), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	print_result(*response);
	waitKey();
	return 0;
}

void PledgeHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<PledageResponseData> data = JsonTool::toJsonClass<PledageResponseData>(response.data());
	if (!data->result)
	{
		std::cout << " pledage failed: " << data->message << std::endl;
	}
	else
	{
		std::cout << "pledage successed: " << data->message << std::endl;
	}
}

int AddMinerHandler::exec()
{
	MSyncHttpRequest request;
	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}
	SystemInfo si;
	//std::cout << si.get_json() << std::endl;
	std::stringstream ss;
	ss << "name=" << name;
	ss << '&';
	ss << "osReturn=";
	ss << request.convert_string_to_http(si.get_json());
	ss << '&';
	ss << "password=" << password;
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::AddMiner), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	//std::cout << response.data() << std::endl;
	print_result(*response);
	waitKey();
	return 0;
}

void AddMinerHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<AddMinerResponseData> ptr = JsonTool::toJsonClass<AddMinerResponseData>(response.data());
	if (!ptr->ok())
	{
		ConfigInfoManager::getInstance()->clear();
		std::cout << "add miner failed: " << ptr->message << std::endl;
	}
	else
	{
		std::cout << "add miner success: " << ptr->message << std::endl;
	}
}

int MinerHandler::exec()
{
	MSyncHttpRequest request;
	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}
	SystemInfo si;

	while (true)
	{
		std::stringstream ss;
		ss << "name=" << name;
		ss << '&';
		ss << "osReturn=";
		ss << request.convert_string_to_http(si.get_json());
		ss << '&';
		ss << "password=" << password;
		auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::Miner), ss.str());
		if (response->isNull())
		{
			return -1;
		}
		print_result(*response);
		std::this_thread::sleep_for(std::chrono::seconds(120));
	}
	return 0;
}

void MinerHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<MinerResponseData> ptr = JsonTool::toJsonClass<MinerResponseData>(response.data());
	if (!ptr->ok())
	{
		std::cout << "miner failed: " << ptr->message << std::endl;
	}
	else
	{
		std::cout << "miner success: " << ptr->message << std::endl;
	}
}

int UpdateMinerHandler::exec()
{
	MSyncHttpRequest request;
	std::string name = ConfigInfoManager::getInstance()->get_name();
	if (name.empty())
	{
		return -1;
	}
	std::string password = ConfigInfoManager::getInstance()->get_password();
	if (password.empty())
	{
		return -1;
	}
	SystemInfo si;

	std::stringstream ss;
	ss << "name=" << name;
	ss << '&';
	ss << "osReturn=";
	ss << request.convert_string_to_http(si.get_json());
	ss << '&';
	ss << "password=" << password;
	auto response = request.post(_api.ip(), _api.port(), _api.get_path(CommandType::UpdateMiner), ss.str());
	if (response->isNull())
	{
		return -1;
	}
	print_result(*response);
	waitKey();
	return 0;
}

void UpdateMinerHandler::print_result(const MSyncHttpResponse& response)
{
	std::shared_ptr<UpdateMinerResponseData> ptr = JsonTool::toJsonClass<UpdateMinerResponseData>(response.data());
	if (!ptr->ok())
	{
		ConfigInfoManager::getInstance()->clear();
		std::cout << "update miner failed: " << ptr->message << std::endl;
	}
	else
	{
		std::cout << "update miner success: " << ptr->message << std::endl;
	}
}

void CommandHandlerBase::waitKey()
{
	/*std::cout << "\npress any key..." << std::endl;
	InputManager::getInstance()->waitKey();*/
}
