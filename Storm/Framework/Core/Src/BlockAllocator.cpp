#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace Storm::Core;
BlockAllocator::BlockAllocator(std::string name, size_t blockSize, size_t capacity)
	:mName(name), mData(nullptr), mBlockSize(blockSize), mCapacity(capacity), mFreeBlocks(capacity)
{
	ASSERT(blockSize > 0, "Invalid block size!");
	ASSERT(capacity > 0, "Invalid capacity!");

	mData = std::malloc(mCapacity * mBlockSize);

	for (std::size_t i = 0; i < capacity; ++i)
	{
		mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
	}

	if (!mName.empty())
	{
		LOG("%s::allocated %zu blocks, block size: %zu", mName.c_str(), capacity, blockSize);
	}
}
BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	:BlockAllocator("", blockSize, capacity)
{

}



BlockAllocator::~BlockAllocator()
{
	// TODO
	ASSERT(mFreeBlocks.size() == mCapacity, "BlockAllocator -- Not all blocks are freed. Potential memory leak!");
	free(mData);

	if (!mName.empty())
	{
		LOG("%s::destructed. Allocated: %zu, Freed: %zu", mName.c_str(), mBlockAllocated, mBlockFreed);
	}


	mData = nullptr;
	mBlockSize = 0;
	mCapacity = 0;

	for (auto ptr : mFreeBlocks)
	{
		ptr = nullptr;
	}
	mFreeBlocks.clear();

}

void BlockAllocator::SetName(std::string name)
{
	mName = std::move(name);

}

void* BlockAllocator::Allocate()
{
	// TODO

	if (mFreeBlocks.empty())
	{
		if (!mName.empty())
		{
			LOG("%s::no free blocks left", mName.c_str());
		}
		return nullptr;
	}

	void* ptr = mFreeBlocks.back();
	mFreeBlocks.pop_back();
	if (!mName.empty())
	{
		LOG("%s::Allocate %x", mName.c_str(), ptr);
		++mBlockAllocated;
	}
	return ptr;
}

// blockSize = 4
// blockCount = 10
//
// .... [                                   ]
//      |    |    |    |               |
//      0    4    8    12     ...      36
//      ^
//      mData
void BlockAllocator::Free(void* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	const auto start = static_cast<uint8_t*>(mData);
	const auto end = static_cast<uint8_t*>(mData) + mBlockSize * mCapacity;
	const auto current = static_cast<uint8_t*>(ptr);
	const auto diff = current - start;

	ASSERT(current >= start && current < end&& static_cast<std::size_t>(diff) % mBlockSize == 0, "BlockAllocator -- Invalid address being freed.");

	//ASSERT(static_cast<uint8_t*>(ptr) <= ((static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity - 1))), "Pointor beyond the block, not the block given");
	//ASSERT((static_cast<uint8_t*>(ptr) - static_cast<uint8_t*>(mData)) % mBlockSize == 0, "Pointor not at the boundary, not the block given");

	//for (int i = 0; i < mFreeBlocks.size(); ++i)
	//{
	//	ASSERT(ptr != mFreeBlocks[i], "The block is already free");
	//}

	if (!mName.empty())
	{
		LOG("%s::Free %x", mName.c_str(), ptr);
		++mBlockFreed;
	}

	mFreeBlocks.emplace_back(ptr);

	// TODO

}