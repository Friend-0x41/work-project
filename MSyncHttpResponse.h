#ifndef MYSYNCRESPONSE_H_
#define MYSYNCRESPONSE_H_

#include <string>

class MSyncHttpResponse
{
public:

	MSyncHttpResponse(char* data, size_t size)
		:_data(data), _size(size)
	{

	}

	MSyncHttpResponse()
		:_data(nullptr), _size(0)
	{

	}

	~MSyncHttpResponse()
	{
		delete[] _data;
	}

	char* data() const
	{
		return _data;
	}

	bool isNull() const
	{
		return !_data;
	}

	size_t size() const
	{
		return _size;
	}
private:

	char* _data = nullptr;

	size_t _size;
};

#endif