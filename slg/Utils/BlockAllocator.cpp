﻿#include <limits.h>
#include <memory.h>
#include <stddef.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "BlockAllocator.h"


int BlockAllocator::block_sizes_[kBlockSizes] =
{
	16,		// 0
	32,		// 1
	64,		// 2
	96,		// 3
	128,	// 4
	160,	// 5
	192,	// 6
	224,	// 7
	256,	// 8
	320,	// 9
	384,	// 10
	448,	// 11
	512,	// 12
	640,	// 13
	1024	// 14
};

bool BlockAllocator::s_block_size_lookup_initialized_;

uint8_t BlockAllocator::s_block_size_lookup_[kMaxBlockSize + 1];

BlockAllocator::BlockAllocator()
{
	assert(kBlockSizes < UCHAR_MAX);

	num_chunk_space_ = kChunkArrayIncrement;
	num_chunk_count_ = 0;
	chunks_ = (Chunk *)malloc(num_chunk_space_ * sizeof(Chunk));

	memset(chunks_, 0, num_chunk_space_ * sizeof(Chunk));
	memset(free_lists_, 0, sizeof(free_lists_));

	if (s_block_size_lookup_initialized_ == false)
	{
		int j = 0;
		for (int i = 1; i <= kMaxBlockSize; ++i)
		{
			assert(j < kBlockSizes);
			if (i <= block_sizes_[j])
			{
				s_block_size_lookup_[i] = (uint8_t)j;
			}
			else
			{
				++j;
				s_block_size_lookup_[i] = (uint8_t)j;
			}
		}
		s_block_size_lookup_initialized_ = true;
	}
}

BlockAllocator::~BlockAllocator()
{
	for (int i = 0; i < num_chunk_count_; ++i)
	{
		::free(chunks_[i].blocks);
	}
	::free(chunks_);
}

void* BlockAllocator::Allocate(int size)
{
	if (size <= 0)
	{
		return nullptr;
	}

	size += sizeof(uint32_t);
	assert(size <= kMaxBlockSize);

	int index = s_block_size_lookup_[size];
	assert(0 <= index && index < kBlockSizes);

	if (free_lists_[index])
	{
		Block *block = free_lists_[index];
		free_lists_[index] = block->next;
		return block->mem;
	}
	else
	{
		if (num_chunk_count_ == num_chunk_space_)
		{
			Chunk *oldChunks = chunks_;
			num_chunk_space_ += kChunkArrayIncrement;
			chunks_ = (Chunk *)malloc(num_chunk_space_ * sizeof(Chunk));
			memcpy(chunks_, oldChunks, num_chunk_count_ * sizeof(Chunk));
			memset(chunks_ + num_chunk_count_, 0, kChunkArrayIncrement * sizeof(Chunk));
			::free(oldChunks);
		}

		Chunk *chunk = chunks_ + num_chunk_count_;
		chunk->blocks = (Block *)malloc(kChunkSize);
#if defined(_DEBUG)
		memset(chunk->blocks, 0xcd, kChunkSize);
#endif
		int block_size = block_sizes_[index];
		chunk->block_size = block_size;
		int block_count = kChunkSize / block_size;
		assert(block_count * block_size <= kChunkSize);
		for (int i = 0; i < block_count - 1; ++i)
		{
			Block *block = (Block *)((uint8_t *)chunk->blocks + block_size * i);
			block->idx = index;
			block->mem = (uint32_t*)block+1;
			Block *next = (Block *)((uint8_t *)chunk->blocks + block_size * (i + 1));
			block->next = next;
		}
		Block *last = (Block *)((uint8_t *)chunk->blocks + block_size * (block_count - 1));
		last->idx = index;
		last->mem = (uint32_t*)last + 1;
		last->next = nullptr;

		free_lists_[index] = chunk->blocks->next;
		++num_chunk_count_;

		return chunk->blocks->mem;
	}
}

void BlockAllocator::Free(void *p)
{
	uint32_t* pBlock = (uint32_t*)p;
	Block* block = (Block*)(pBlock - 1);
	block->mem = p;
	Free(block, block->idx);
}

void BlockAllocator::Free(void *p, int index)
{
	assert(0 <= index && index < kBlockSizes);


#ifdef _DEBUG
	int block_size = block_sizes_[index];
	bool found = false;
	for (int i = 0; i < num_chunk_count_; ++i)
	{
		Chunk *chunk = chunks_ + i;
		if (chunk->block_size != block_size)
		{
			assert((uint8_t *)p + block_size <= (uint8_t *)chunk->blocks ||
				(uint8_t *)chunk->blocks + kChunkSize <= (uint8_t *)p);
		}
		else
		{
			if ((uint8_t *)chunk->blocks <= (uint8_t *)p && (uint8_t *)p + block_size <= (uint8_t *)chunk->blocks + kChunkSize)
			{
				found = true;
				break;
			}
		}
	}

	assert(found);

#endif
	Block *block = (Block *)p;
	block->next = free_lists_[index];
	free_lists_[index] = block;
}

void BlockAllocator::Clear()
{
	for (int i = 0; i < num_chunk_count_; ++i)
	{
		::free(chunks_[i].blocks);
	}

	num_chunk_count_ = 0;
	memset(chunks_, 0, num_chunk_space_ * sizeof(Chunk));
	memset(free_lists_, 0, sizeof(free_lists_));
}