#ifndef JSONTOOL_H_
#define JSONTOOL_H_

#include "LogTool.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <memory>
#include <iostream>
#include <vector>
#include <sstream>

class JsonObject
{
	friend class JsonClassBase;
private:

	JsonObject(const std::string& json)
	{
		_doc.Parse(json.c_str());
	}

	JsonObject()
	{
		//_doc.SetObject();
	}

	std::string to_json() const
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		_doc.Accept(writer);
		return buffer.GetString();
	}

	rapidjson::Value& get_value(const std::string& key)
	{
		return _doc[key.c_str()];
	}

	void add_value(const std::string& key, rapidjson::Value&& value)
	{
		if (!_doc.HasMember(key.c_str()))
		{
			_doc.AddMember(rapidjson::Value(key.c_str(), key.length()), rapidjson::Value(std::move(value)), _doc.GetAllocator());
		}
	}

	bool from_json(const std::string& json)
	{
		//_doc.Clear();
		_doc.Parse(json.c_str());
		return !_doc.HasParseError();
	}

	bool has_value(const std::string& key)
	{
		return _doc.HasMember(key.c_str());
	}
private:

	rapidjson::Document _doc;
};

struct BalanceResponseSubData
{
	std::string address;

	double balance;

	std::string pledage_balance;

	std::string pledage_balance_used;

	void set_content(rapidjson::Value& value)
	{
		if (value.IsNull())
		{
			return;
		}
		if (!value["address"].IsNull())
			address = value["address"].GetString();
		if (!value["balance"].IsNull())
			balance = std::stod(value["balance"].GetString());
		if (!value["pledage balance"].IsNull())
			pledage_balance = value["pledage balance"].GetString();
		if (!value["pledage balance used"].IsNull())
			pledage_balance_used = value["pledage balance used"].GetString();
	}
};

class JsonClassBase
{
public:

	bool Parse(const std::string& json)
	{
		return _jsonObject.from_json(json);
	}

	bool has_value(const std::string& key)
	{
		return _jsonObject.has_value(key);
	}

	virtual void to_object(const std::string& json) = 0;

	virtual std::string to_json() const
	{
		return _jsonObject.to_json();
	}

	int get_int(const std::string& key)
	{
		if (!_jsonObject.get_value(key).IsNull())
		{
			return _jsonObject.get_value(key).GetInt();
		}
		return 0;
	}

	double get_double(const std::string& key)
	{
		if (!_jsonObject.get_value(key).IsNull())
		{
			return _jsonObject.get_value(key).GetDouble();
		}
		return 0;
	}

	std::string get_string(const std::string& key)
	{
		if (!_jsonObject.get_value(key).IsNull())
		{
			return _jsonObject.get_value(key).GetString();
		}
		return std::string();
	}

	bool get_bool(const std::string& key)
	{
		if (!_jsonObject.get_value(key).IsNull())
		{
			return _jsonObject.get_value(key).GetBool();
		}
		return false;
	}

	rapidjson::Value& get_object(const std::string& key)
	{
		return _jsonObject.get_value(key);
	}

protected:

	JsonObject _jsonObject;
};

struct BalanceResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		data.set_content(get_object("data"));
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	BalanceResponseSubData data;
};

struct AddMinerResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		data= get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	std::string data;
};

struct ExtractResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		//data = get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	//std::string data;
};

struct ExtractCancelResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		//data = get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	//std::string data;
};

struct ExtractPledageResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		//data = get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	//std::string data;
};

struct PledageResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		//data = get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	//std::string data;
};

struct MinerResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", get_string("message"), message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		//data = get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	//std::string data;
};

struct UpdateMinerResponseData : public JsonClassBase
{
	virtual void to_object(const std::string& json) override
	{
		if (json.find("error") != std::string::npos)
		{
			result = false;
			Parse(json);
			message = get_string("message");
			LogTool::getInstance()->Log("Server error: ", get_string("error"), ",", "message: ", message);
			return;
		}
		if (!Parse(json))
		{
			result = false;
			message = "Server Error";
			LogTool::getInstance()->log(json);
			return;
		}
		result = get_bool("result");
		message = get_string("message");
		code = get_int("code");
		//data = get_string("data");
	}

	bool ok() const
	{
		return result;
	}

	bool result;

	std::string message;

	int code;

	//std::string data;
};

class JsonTool
{
public:

	template<typename T>
	static std::shared_ptr<T> toJsonClass(const std::string& json)
	{
		auto ptr = std::make_shared<T>();
		ptr->to_object(json);
		return ptr;
	}
};

#endif