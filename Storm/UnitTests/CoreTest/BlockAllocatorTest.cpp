#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Storm::Core;

namespace CoreTest
{
	TEST_CLASS(BlockAllocatorTest)
	{
	public:
		TEST_METHOD(OneBlockTest)
		{
			BlockAllocator blockAllocator(16, 1);

			void* ptr1 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr1);

			void* ptr2 = blockAllocator.Allocate();
			Assert::IsNull(ptr2);

			blockAllocator.Free(ptr1);

		}

		TEST_METHOD(TwoBlockTest)
		{
			BlockAllocator blockAllocator(16, 2);

			void* ptr1 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr1);

			void* ptr2 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr2);

			Assert::IsTrue(ptr1 != ptr2);

			ptrdiff_t diff = static_cast<uint8_t*>(ptr1) - static_cast<uint8_t*>(ptr2);
			Assert::IsTrue(abs(diff) >= 16);

			void* ptr3 = blockAllocator.Allocate();
			Assert::IsNull(ptr3);

			blockAllocator.Free(ptr1);
			blockAllocator.Free(ptr2);


		}

		TEST_METHOD(FreeTest)
		{
			BlockAllocator blockAllocator(16, 1);

			void* ptr1 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr1);

			void* ptr2 = blockAllocator.Allocate();
			Assert::IsNull(ptr2);

			blockAllocator.Free(ptr1);

			ptr2 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr2);

			Assert::IsTrue(ptr1 == ptr2);

			blockAllocator.Free(ptr2);
		}

		//TEST_METHOD(FreeTest2)
		//{
		//	BlockAllocator blockAllocator(16, 2);

		//	void* ptr1 = blockAllocator.Allocate();
		//	Assert::IsNotNull(ptr1);

		//	blockAllocator.Free(ptr1);

		//	void* ptr3 = blockAllocator.Allocate();
		//	Assert::IsNotNull(ptr3);

		//	Assert::IsTrue(ptr1 == ptr3);
		//}
	};
}
