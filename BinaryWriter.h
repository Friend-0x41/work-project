#ifndef BINARYWRITER_H_
#define BINARYWRITER_H_

#include <fstream>
#include <iostream>
#include <string>

class WriteFailed
{
public:

	WriteFailed(const std::string& m)
		:msg(m)
	{

	}

	std::string msg;
};

class BinaryWriter
{
public:
	BinaryWriter(const std::string& filePath);

	bool write(char* buf, size_t size);

	~BinaryWriter()
	{
		flush();
		_out.close();
		delete[] _buf;
	}

	void flush()
	{
		if (_size == 0)
		{
			return;
		}
		write_buffer();
	}

private:

	void write_binary(char* buf, size_t size);

	void write_buffer();

	size_t _bufferSize;

	char* _buf;

	size_t _size = 0;

	std::ofstream _out;

	std::string _filePath;

	const static int _deep = 64;
};

#endif