#include "MSyncHttpRequest.h"
#include "msyncsocket.h"
#include <sstream>
#include "LogTool.h"
#include <iostream>
#ifdef __linux__
#include <string.h>
#endif

MSyncHttpRequest::MSyncHttpRequest()
{
	init();
}

std::shared_ptr<MSyncHttpResponse> MSyncHttpRequest::post(const std::string& ip, int port, const std::string& path, const std::string& data)
{
	std::stringstream ss;
	ss << "POST ";
	ss << path;
	ss << '?';
	ss << data;
	ss << " HTTP/1.0\r\n";
	ss << "Host: " << ip << "\r\n";
	ss << "Connection:close\r\n\r\n";
	ss << "Accept-Encoding:gzip, deflate\r\n";
	ss << "Accept-Language:zh-CN,en,*\r\n";
	ss << "Content-Length:" << 0 << "\r\n";
	ss << "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n";
	ss << "User-Agent:Mozilla/5.0\r\n\r\n";
	//std::cout << ss.str() << std::endl;
	MSyncSocket ms(ip, port);
	if (!ms.connect())
	{
		LogTool::getInstance()->Log("Connect error: ", ms.getErrorCode());
		return std::make_shared<MSyncHttpResponse>();
	}
	//std::cout << data << std::endl;
	std::string s = ss.str();
	if (!ms.send(s.c_str(), s.length()))
	{
		LogTool::getInstance()->Log("Send error: ", ms.getErrorCode());
		return std::make_shared<MSyncHttpResponse>();
	}
	const size_t read_size = 1024;
	char buf[read_size];
	memset(buf, 0, read_size);
	int sz = ms.receive(buf, read_size);
	if (sz <= 0)
	{
		LogTool::getInstance()->Log("no data received");
		return std::make_shared<MSyncHttpResponse>();
	}
	size_t pos = 0;
	while (pos < sz)
	{
		if ((pos >= (read_size - 1)) || ((pos + 3) >= (read_size - 1)))
		{
			LogTool::getInstance()->Log("no data found");
			return std::make_shared<MSyncHttpResponse>();
		}
		if ((buf[pos] == '\r') && (buf[pos + 3] == '\n'))
		{
			pos = pos + 4;
			break;
		}
		++pos;
	}
	if (pos > sz)
	{
		LogTool::getInstance()->Log("no data found");
		return std::make_shared<MSyncHttpResponse>();
	}

	char* cp = new char[sz + 1];
	memset(cp, 0, sz + 1);
	memcpy(cp, buf + pos, sz);
	pos = sz;
	return std::make_shared<MSyncHttpResponse>(cp, sz);
}

std::shared_ptr<MSyncHttpResponse> MSyncHttpRequest::get(const std::string& ip, int port, const std::string& path)
{
	std::stringstream ss;
	ss << "GET ";
	ss << path;
	ss << '?';
	ss << " HTTP/1.0\r\n";
	ss << "Host: " << ip << "\r\n";
	ss << "Connection:close\r\n\r\n";
	ss << "Accept-Encoding:gzip, deflate\r\n";
	ss << "Accept-Language:zh-CN,en,*\r\n";
	ss << "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n";
	ss << "User-Agent:Mozilla/5.0\r\n\r\n";
	//std::cout << ss.str() << std::endl;
	MSyncSocket ms(ip, port);
	if (!ms.connect())
	{
		LogTool::getInstance()->Log("Connect error: ", ms.getErrorCode());
		return std::make_shared<MSyncHttpResponse>();
	}
	//std::cout << data << std::endl;
	std::string s = ss.str();
	if (!ms.send(s.c_str(), s.length()))
	{
		LogTool::getInstance()->Log("Send error: ", ms.getErrorCode());
		return std::make_shared<MSyncHttpResponse>();
	}
	const size_t read_size = 4096;
	char buf[read_size];
	int sz = 0;
	sz = ms.receive(buf, read_size);
	if (sz <= 0)
	{
		LogTool::getInstance()->Log("no data received");
		return std::make_shared<MSyncHttpResponse>();
	}
	size_t pos = 0;
	while (pos < sz)
	{
		if ((pos >= (read_size - 1)) || ((pos + 3) >= (read_size - 1)))
		{
			LogTool::getInstance()->Log("no data found");
			return std::make_shared<MSyncHttpResponse>();
		}
		if ((buf[pos] == '\r') && (buf[pos + 3] == '\n'))
		{
			pos = pos + 4;
			break;
		}
		++pos;
	}
	if (pos > sz)
	{
		LogTool::getInstance()->Log("no data found");
		return std::make_shared<MSyncHttpResponse>();
	}
	std::string head(buf, buf + pos);
	size_t start = head.find("Content-Length") + 15;
	if (start == std::string::npos)
	{
		return std::make_shared<MSyncHttpResponse>();
	}
	size_t end = head.find_first_of('\r', start);
	if (end == std::string::npos)
	{
		return std::make_shared<MSyncHttpResponse>();
	}
	size_t length = std::stoll(std::string(head.begin() + start, head.begin() + end));
	char* cp = new char[length + 1];
	memset(cp, 0, length + 1);
	memcpy(cp, buf + pos, sz - pos);
	if (sz - pos == length)
	{
		return std::make_shared<MSyncHttpResponse>(cp, length);
	}
	size_t responseSize = sz - pos;
	while ((sz = ms.receive(buf, read_size)) > 0)
	{
		memcpy(cp + responseSize, buf, sz);
		responseSize += sz;
	}
	return std::make_shared<MSyncHttpResponse>(cp, responseSize);
}

void MSyncHttpRequest::init()
{
	_httpConvertStrings.insert(std::make_pair(' ', "%20"));
	_httpConvertStrings.insert(std::make_pair('"', "%22"));
	_httpConvertStrings.insert(std::make_pair('#', "%23"));
	_httpConvertStrings.insert(std::make_pair('%', "%25"));
	_httpConvertStrings.insert(std::make_pair('(', "%28"));
	_httpConvertStrings.insert(std::make_pair(')', "%29"));
	_httpConvertStrings.insert(std::make_pair('+', "%2B"));
	_httpConvertStrings.insert(std::make_pair(',', "%2C"));
	_httpConvertStrings.insert(std::make_pair('/', "%2F"));
	_httpConvertStrings.insert(std::make_pair(':', "%3A"));
	_httpConvertStrings.insert(std::make_pair(';', "%3B"));
	_httpConvertStrings.insert(std::make_pair('<', "%3C"));
	_httpConvertStrings.insert(std::make_pair('=', "%3D"));
	_httpConvertStrings.insert(std::make_pair('>', "%3E"));
	_httpConvertStrings.insert(std::make_pair('?', "%3F"));
	_httpConvertStrings.insert(std::make_pair('@', "%40"));
	_httpConvertStrings.insert(std::make_pair('\\', "%5C"));
	_httpConvertStrings.insert(std::make_pair('|', "%7C"));
	_httpConvertStrings.insert(std::make_pair('{', "%7B"));
	_httpConvertStrings.insert(std::make_pair('}', "%7D"));
	_httpConvertStrings.insert(std::make_pair('[', "%5B"));
	_httpConvertStrings.insert(std::make_pair(']', "%5D"));
}

std::string MSyncHttpRequest::convert_string_to_http(const std::string& s)
{
	{
		std::stringstream ss;
		for (auto it = s.cbegin(); it != s.cend(); ++it)
		{
			if (_httpConvertStrings.find(*it) != _httpConvertStrings.cend())
			{
				ss << _httpConvertStrings[*it];
				continue;
			}
			ss << *it;
		}
		return ss.str();
	}
}
