/**
 * 小对象管理器
 */

#ifndef __BLOCKALLOCATOR_H__
#define __BLOCKALLOCATOR_H__

#include <cstdint>
#include <cstddef>
#include "Utils/Singleton.hpp"

/// This is a small object allocator used for allocating small
/// objects that persist for more than one time step.
/// See: http://www.codeproject.com/useritems/Small_Block_Allocator.asp
class BlockAllocator
{
	static const int kChunkSize = 16 * 1024;
	static const int kMaxBlockSize = 1024;
	static const int kBlockSizes = 15;
	static const int kChunkArrayIncrement = 128;

public:
	BlockAllocator();
	~BlockAllocator();

public:
	void* Allocate(int size);
	void Clear();
	void Free(void *p);
private:

	struct Block
	{
		char idx;
		char reversed[3];
		void* mem;
		Block *next;
	};

	struct Chunk
	{
		uint32_t block_size;
		Block *blocks;
	};

	void Free(void *p, int size);
	int				num_chunk_count_;
	int				num_chunk_space_;
	struct Chunk*	chunks_;
	struct Block*	free_lists_[kBlockSizes];
	static int		block_sizes_[kBlockSizes];
	static uint8_t	s_block_size_lookup_[kMaxBlockSize + 1];
	static bool		s_block_size_lookup_initialized_;
};

#define UseBlockAllocator \
public: \
void* operator new(std::size_t size) \
{ \
	return Singleton<BlockAllocator>::instance()->Allocate(size); \
} \
void operator delete(void* p) throw() \
{ \
	Singleton<BlockAllocator>::instance()->Free(p); \
}

#endif