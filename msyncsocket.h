#ifndef MSYNCSOCKET_H_
#define MSYNCSOCKET_H_

#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#endif

class MSyncSocket
{
public:

	MSyncSocket(const std::string& ip, int port);

	MSyncSocket(const std::string& domain);

	~MSyncSocket();

	bool send(const char* data, size_t len);

	size_t receive(char* out, size_t len);

	int getErrorCode() const noexcept;

	bool connect();
private:

	void createSocketConnection(const std::string& ip, int port);

	void closeConnection();
private:

#ifdef _WIN32
	SOCKET _socketClient;
#else
	int _socketClient;
#endif

	sockaddr_in _serverAddr;

	int _errorCode;
 };

#endif
