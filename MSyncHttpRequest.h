#ifndef MYSYNCHTTPREQUEST_H_
#define MYSYNCHTTPREQUEST_H_

#include "MSyncHttpResponse.h"
#include <map>
#include <memory>

class MSyncHttpRequest
{
public:

	MSyncHttpRequest();

	std::shared_ptr<MSyncHttpResponse> post(const std::string& ip, int port, const std::string& path, const std::string& data);

	std::shared_ptr<MSyncHttpResponse> get(const std::string& ip, int port, const std::string& path);

	std::string convert_string_to_http(const std::string& s);
private:

	void init();

	std::map<int, std::string> _httpConvertStrings;
};

#endif