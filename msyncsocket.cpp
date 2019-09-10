#include "msyncsocket.h"

MSyncSocket::MSyncSocket(const std::string& ip, int port)
	:_errorCode(0)
{
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	createSocketConnection(ip, port);
}

MSyncSocket::MSyncSocket(const std::string& domain)
	:_errorCode(0)
{
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	hostent* host = gethostbyname(domain.c_str());
	if (!host)
	{
		_errorCode = -1;
		return;
	}
	std::string ip = host->h_addr_list[0];
	createSocketConnection(ip, 80);
}

void MSyncSocket::createSocketConnection(const std::string& ip, int port)
{
	_socketClient = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	_serverAddr.sin_family = PF_INET;
	inet_pton(PF_INET, ip.c_str(), &_serverAddr.sin_addr);
	_serverAddr.sin_port = htons(port);
#ifdef _WIN32
	int timeout = 3000; //3s
	int ret = setsockopt(_socketClient, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#elif __linux__
	struct timeval timeout = { 3,0 };//3s
	int ret = setsockopt(_socketClient, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#endif
}

MSyncSocket::~MSyncSocket()
{
	closeConnection();
#ifdef _WIN32
	WSACleanup();
#endif
}

bool MSyncSocket::send(const char* data, size_t len)
{
#ifdef _WIN32
	_errorCode = ::send(_socketClient, data, len, 0);
#else
	_errorCode = write(_socketClient, data, len);
#endif
	return _errorCode > 0;
}

size_t MSyncSocket::receive(char* out, size_t len)
{
	memset(out, 0, len);
#ifdef _WIN32
	return recv(_socketClient, out, len, 0);
#else
	return read(_socketClient, out, len);
#endif
}

int MSyncSocket::getErrorCode() const noexcept
{
	return _errorCode;
}

bool MSyncSocket::connect()
{ 
	_errorCode = ::connect(_socketClient, (sockaddr*)& _serverAddr, sizeof(sockaddr));
	return _errorCode == 0;
}

void MSyncSocket::closeConnection()
{
#ifdef _WIN32
	closesocket(_socketClient);
#else
	close(_socketClient);
#endif
	_errorCode = -1;
}
