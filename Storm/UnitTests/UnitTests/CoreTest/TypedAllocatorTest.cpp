#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Storm::Core;

namespace CoreTest
{
	TEST_CLASS(TypedAllocatorTest)
	{
	public:
		class Bar
		{
		public:
			static int counter; 
			Bar() { counter = 0; }
			Bar(const Bar&) { counter = 1; }
			Bar(Bar&&) { counter = 2; }
		};

		class Foo
		{
		public:
			int a, b;
			Foo() : a(42), b(7) {}
			Foo(int i, int j) : a(i), b(j) {}
			Foo(Bar b) : a(12), b(34) {}
			Foo(std::string str) : a(12), b(34) {}
			~Foo() { a = 0xfeeefeee, b = 0xdeadbeef; }
		};

		TEST_METHOD(TestNew)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New(); // default constructor Foo()
			Assert::IsNotNull(ptr);
			Assert::IsTrue(ptr->a == 42);
			Assert::IsTrue(ptr->b == 7);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNull(ptr2);

			typedAllocator.Delete(ptr);
		}

		TEST_METHOD(TestOverloadedNew)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New(36, 48); // overloaded constructor Foo(int, int)
			Assert::IsNotNull(ptr);
			Assert::AreEqual(ptr->a, 36);
			Assert::AreEqual(ptr->b, 48);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNull(ptr2);
			typedAllocator.Delete(ptr);
		}

		TEST_METHOD(TestForwardingNew)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New(Bar()); // overloaded constructor Foo(Bar), Bar should be moved
			Assert::IsNotNull(ptr);
			Assert::AreEqual(ptr->a, 12);
			Assert::AreEqual(ptr->b, 34);
			Assert::AreEqual(Bar::counter, 2);
			typedAllocator.Delete(ptr);

		}

		TEST_METHOD(TestDelete)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);

			typedAllocator.Delete(ptr); // destructor called
			Assert::AreEqual((unsigned int)ptr->a, 0xfeeefeee);
			Assert::AreEqual((unsigned int)ptr->b, 0xdeadbeef);
		}

		TEST_METHOD(TestRecycling)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr1 = typedAllocator.New();
			Assert::IsNotNull(ptr1);

			typedAllocator.Delete(ptr1);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNotNull(ptr2);
			Assert::IsTrue(ptr1 == ptr2);

			typedAllocator.Delete(ptr2);
		}
	};

	int TypedAllocatorTest::Bar::counter = 0;
}
