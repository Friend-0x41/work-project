#ifndef CHUNKMANAGER_H_
#define CHUNKMANAGER_H_

#include "SingleClass.hpp"
#include <cstdlib>

class ChunkManager : public SingleClass<ChunkManager>
{
	struct ChunkHead
	{
		unsigned long long total;

		unsigned long long size;

		unsigned long long self;
	};
public:

	void check(size_t totalSize);

private:

	bool generate_chunk();

	bool write_chunk(int index);

	static const size_t _peerAmount = 4096;

	size_t _totalSize;

	size_t _perSize;
};

#endif