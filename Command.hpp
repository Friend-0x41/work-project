#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include <string>

class Command
{
public:
	enum class CommandType
	{
		AddMiner,
		Balance,
		Extract,
		ExtractCancel,
		ExtractPledage,
		Miner,
		Pledge,
		UpdateMiner,
		None
	};

	Command(const std::string& cmd)
	{
		_paramaters.push_back(get_command_type(cmd));
	}

	Command(int argc, char* argv[])
	{
		for (int i = 1; i < argc; ++i)
		{
			_paramaters.push_back(get_command_type(argv[i]));
		}
	}

	const CommandType get_command(int index)
	{
		if (index < 0 || index >= _paramaters.size())
		{
			return CommandType::None;
		}
		return _paramaters[index];
	}

	int get_command_count() const
	{
		return _paramaters.size();
	}

private:

	CommandType get_command_type(const std::string& command)
	{
		if (command == "addMiner")
		{
			return CommandType::AddMiner;
		}
		else if (command == "balance")
		{
			return CommandType::Balance;
		}
		else if (command == "extract")
		{
			return CommandType::Extract;
		}
		else if (command == "extractCancel")
		{
			return CommandType::ExtractCancel;
		}
		else if (command == "extractPledage")
		{
			return CommandType::ExtractPledage;
		}
		else if (command == "miner")
		{
			return CommandType::Miner;
		}
		else if (command == "pledge")
		{
			return CommandType::Pledge;
		}
		else if (command == "updateMiner")
		{
			return CommandType::UpdateMiner;
		}
		else
		{
			return CommandType::None;
		}
	}

private:

	std::vector<CommandType> _paramaters;
};

#endif