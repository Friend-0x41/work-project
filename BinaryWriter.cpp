#include "BinaryWriter.h"
#include <cstring>

BinaryWriter::BinaryWriter(const std::string& filePath)
	:_filePath{ filePath }, _bufferSize(4096), _buf{ new char[_bufferSize] }
{
	_out.open(filePath, std::ios_base::out | std::ios_base::binary);
	if (!_out.is_open())
	{
		std::cout << "can't open file to write " << filePath << std::endl;
	}
}

bool BinaryWriter::write(char* buf, size_t size)
{
	int newSize = _bufferSize;
	if (size > _bufferSize)
	{
		while (newSize < size / _deep)
		{
			newSize *= 2;
		}
	}
	if (newSize != _bufferSize)
	{
		_bufferSize = newSize;
		delete[] _buf;
		_buf = new char[_bufferSize];
	}
	try
	{
		write_binary(buf, size);
	}
	catch (WriteFailed wf)
	{
		std::cout << wf.msg << std::endl;
		return false;
	}
	catch (...)
	{
		std::cout << "write file failed" << std::endl;
		return false;
	}
	return true;
}

void BinaryWriter::write_binary(char* buf, size_t size)
{
	if (!_out.is_open())
	{
		std::cout << "can't open file to write " << _filePath << std::endl;
	}
	size_t free = _bufferSize - _size;
	if (size > free)
	{
		write_binary(buf, free);
		write_binary(buf + free, size - free);
	}
	else
	{
		memcpy(_buf + _size, buf, size);
		_size += size;
	}
	if (_size == _bufferSize)
	{
		write_buffer();
	}
}

void BinaryWriter::write_buffer()
{
	if (!_out.is_open())
	{
		std::cout << "can't open file to write " << _filePath << std::endl;
	}
	_out.write(_buf, _size);
	_size = 0;
	if (_out.bad())
	{
		throw WriteFailed(_filePath + "write file failed");
	}
}