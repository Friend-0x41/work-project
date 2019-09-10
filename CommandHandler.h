#ifndef COMMANDHANDLER_BASE_H
#define COMMANDHANDLER_BASE_H

#include <map>
#include <string>
#include "MSyncHttpResponse.h"
#include <sstream>

enum CommandType
{
	None,
	Balance,
	AddMiner,
	Extract,
	ExtractCancel,
	ExtractPledage,
	Miner,
	Pledge,
	UpdateMiner,
};

class HttpPostApi
{
public:

	HttpPostApi()
	{
		init();
	}

	std::string get_path(CommandType type)
	{
		return _paths[type];
	}

	std::string ip() const
	{
		//return "192.168.1.32";		//本地测试
		return "39.108.26.235";		//正式
		//return "192.168.31.60";		//后端测试
		//return "47.112.123.101";		//远程测试
	}

	int port() const
	{
		//return 8613;
		return 8084;
		//return 8613;
		//return 8613;
	}
private:

	/*void init()
	{
		_paths.insert(std::make_pair(CommandType::AddMiner, std::string("/miner/addMiner")));
		_paths.insert(std::make_pair(CommandType::Balance, std::string("/miner/balance")));
		_paths.insert(std::make_pair(CommandType::Extract, std::string("/miner/extract")));
		_paths.insert(std::make_pair(CommandType::ExtractCancel, std::string("/miner/extractCancel")));
		_paths.insert(std::make_pair(CommandType::ExtractPledage, std::string("/miner/extractPledage")));
		_paths.insert(std::make_pair(CommandType::Miner, std::string("/miner/miner")));
		_paths.insert(std::make_pair(CommandType::Pledge, std::string("/miner/pledge")));
		_paths.insert(std::make_pair(CommandType::UpdateMiner, std::string("/miner/updateMiner")));
	}*/

	//正式
	void init()
	{
		_paths.insert(std::make_pair(CommandType::AddMiner, std::string("/minerserver/miner/addMiner")));
		_paths.insert(std::make_pair(CommandType::Balance, std::string("/minerserver/miner/balance")));
		_paths.insert(std::make_pair(CommandType::Extract, std::string("/minerserver/miner/extract")));
		_paths.insert(std::make_pair(CommandType::ExtractCancel, std::string("/minerserver/miner/extractCancel")));
		_paths.insert(std::make_pair(CommandType::ExtractPledage, std::string("/minerserver/miner/extractPledage")));
		_paths.insert(std::make_pair(CommandType::Miner, std::string("/minerserver/miner/miner")));
		_paths.insert(std::make_pair(CommandType::Pledge, std::string("/minerserver/miner/pledge")));
		_paths.insert(std::make_pair(CommandType::UpdateMiner, std::string("/minerserver/miner/updateMiner")));
	}
private:

	std::map<CommandType, std::string> _paths;
};

class CommandHandlerBase
{
public:

	virtual int exec() = 0;

	virtual void print_result(const MSyncHttpResponse& resopnse) = 0;

	void waitKey();
protected:

	HttpPostApi _api;
};

class BalanceHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class ExtractHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class ExtractCancelHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class ExtractPledageHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class PledgeHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class AddMinerHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class MinerHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

class UpdateMinerHandler : public CommandHandlerBase
{
public:

	virtual int exec() override;

	virtual void print_result(const MSyncHttpResponse& response) override;
};

#endif