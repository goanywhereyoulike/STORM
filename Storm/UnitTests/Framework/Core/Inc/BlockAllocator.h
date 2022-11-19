#pragma once

namespace Storm::Core
{
	// Description: This is a basic block allocator which performs a single
	// allocation and deallocation on construction/destruction. Blocks are
	// assigned and recycled to the user.
	//
	// Usage:
	//		BlockAllocator blockAllocator(10, 10);
	//		void* memory = blockAllocator.Allocate();
	//		....
	//		blockAllocator.Free(memory);
	//
	class BlockAllocator
	{
	public:

		enum LogVerbodity
		{
			None,
			Light,
			Full
		};
		BlockAllocator(size_t blockSize, size_t capacity); // Allocate all memory here
		BlockAllocator(std::string name, size_t blockSize, size_t capacity); // Allocate all memory here
		virtual ~BlockAllocator();

		// No copy/move/assignment
		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;
		BlockAllocator(BlockAllocator&&) = delete;
		BlockAllocator& operator=(BlockAllocator&&) = delete;

		void SetName(std::string name);
		void* Allocate();
		void Free(void* ptr);

	private:
		std::string mName;
		std::vector<void*> mFreeBlocks;
		void* mData = nullptr;
		size_t mBlockSize = 0;	// How many bytes for a single block
		size_t mCapacity = 0;	// How many blocks
		size_t mBlockAllocated = 0;
		size_t mBlockFreed = 0;
	};
}

#define MEMORY_POOL_DECLARE\
	static Storm::Core::BlockAllocator sAllocator;\
	static void* operator new(std::size_t size);\
	static void operator delete(void* ptr);

#define MEMORY_POOL_DEFINE(Class, Capacity)\
	Storm::Core::BlockAllocator Class::sAllocator(#Class##"Allocator",sizeof(Class),Capacity);\
	void* Class::operator new(std::size_t size) {return sAllocator.Allocate();}\
	void Class::operator delete(void* ptr) {sAllocator.Free(ptr);} 