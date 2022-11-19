#pragma once

#include "BlockAllocator.h"

namespace Storm::Core
{
	// Description: A type aware block allocator. Essentially, it
	// uses the block allocator to allocate and free memory, but add
	// constructor/destructor calls on top of that.
	template <class DataType>
	class TypedAllocator : private BlockAllocator
	{
	public:
		TypedAllocator(size_t capacity)
			: BlockAllocator(sizeof(DataType), capacity)
		{}

		template <class... Args>
		DataType* New(Args&&... args)
		{
			DataType* instance = static_cast<DataType*>(Allocate());
			if (instance != nullptr)
				new (instance) DataType(std::forward<Args>(args)...);
			return instance;
		}

		void Delete(DataType* ptr)
		{
			if (ptr == nullptr)
				return;

			ptr->~DataType();
			Free(ptr);
		}
	};
}
