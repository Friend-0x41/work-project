#include "ChunkManager.h"
#include <fstream>
#include <iostream>
#include "BinaryWriter.h"

void ChunkManager::check(size_t totalSize)
{
	_totalSize = totalSize;
	_perSize = _totalSize * 1024 * 1024 * 1024 / _peerAmount;
	std::ifstream in;
	char buffer[100] = { 0 };
	for (size_t i = 0; i < _peerAmount; ++i)
	{
		int end = sprintf(buffer, "%lld_%lld.bin", i, _peerAmount);
		buffer[end] = '\0';
		in.open(buffer, std::ios_base::in | std::ios_base::binary);
		if (!in.is_open())
		{
			generate_chunk();
			return;
		}
		in.read(buffer, sizeof(ChunkHead));
		ChunkHead* phead = (ChunkHead*)buffer;
		if (phead->size != _perSize || phead->self != i || phead->total != _peerAmount)
		{
			in.close();
			generate_chunk();
			return;
		}
		in.seekg(0, std::ios_base::end);
		int n = in.tellg();
		if (in.tellg() != _perSize)
		{
			in.close();
			generate_chunk();
			return;
		}
		in.close();
	}
	std::cout << "check success" << std::endl;
}

bool ChunkManager::generate_chunk()
{
	for (size_t i = 0; i < _peerAmount; ++i)
	{
		if (!write_chunk(i))
		{
			return false;
		}
	}
	std::cout << "file generate success" << std::endl;
	return true;
}

bool ChunkManager::write_chunk(int index)
{
	char buffer[100] = { 0 };
	std::ofstream out; int end = sprintf(buffer, "%d_%lld.bin", index, _peerAmount);
	buffer[end] = '\0';
	out.open(buffer, std::ios_base::out | std::ios_base::binary);
	if (!out.is_open())
	{
		std::cout << "can.t open file to write " << buffer << std::endl;
		return false;
	}
	BinaryWriter writer(buffer);
	ChunkHead head;
	head.self = index;
	head.size = _perSize;
	head.total = _peerAmount;
	if (!writer.write((char*)& head, sizeof(ChunkHead)))
	{
		std::cout << "can.t open file to write " << buffer << std::endl;
		return false;
	}
	for (int num = 0; num < (_perSize - sizeof(ChunkHead)); num += sizeof(int))
	{
		if (!writer.write((char*)& num, sizeof(int)))
		{
			std::cout << "can.t open file to write " << buffer << std::endl;
			return false;
		}
	}
	return true;
}
