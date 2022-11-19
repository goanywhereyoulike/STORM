#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Storm::Math;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			Vector3 v0;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);

			Vector3 v1(3.14f);
			Assert::AreEqual(v1.x, 3.14f);
			Assert::AreEqual(v1.y, 3.14f);
			Assert::AreEqual(v1.z, 3.14f);

			Vector3 v2(1.2f,2.3f,-3.4f);
			Assert::AreEqual(v2.x, 1.2f);
			Assert::AreEqual(v2.y, 2.3f);
			Assert::AreEqual(v2.z, -3.4f);
		}

		TEST_METHOD(TestStatics)
		{
			Vector3 zero = Vector3::Zero;
			Assert::AreEqual(zero.x, 0.0f);
			Assert::AreEqual(zero.y, 0.0f);
			Assert::AreEqual(zero.z, 0.0f);

			Vector3 one = Vector3::One;
			Assert::AreEqual(one.x, 1.0f);
			Assert::AreEqual(one.y, 1.0f);
			Assert::AreEqual(one.z, 1.0f);


		}
		TEST_METHOD(TestNegation)
		{
			Vector3 v0(1.0f);
			v0 = -v0;
			Assert::AreEqual(v0.x, -1.0f);
			Assert::AreEqual(v0.y, -1.0f);
			Assert::AreEqual(v0.z, -1.0f);


		}
		TEST_METHOD(TestAddition)
		{
			Vector3 v1(1.01f);
			Vector3 v2(3.14f);
			Vector3 v3 = v1 + v2;
			Assert::AreEqual(v3.x, 4.15f);
			Assert::AreEqual(v3.y, 4.15f);
			Assert::AreEqual(v3.z, 4.15f);

		}
		TEST_METHOD(TestSubtraction)
		{
			Vector3 v1(1.01f);
			Vector3 v2(3.14f);
			Vector3 v3 = v2 - v1;
			Assert::AreEqual(v3.x, 2.13f);
			Assert::AreEqual(v3.y, 2.13f);
			Assert::AreEqual(v3.z, 2.13f);

		}
		TEST_METHOD(TestScalarMultiplication)
		{
			Vector3 v1(1.01f);
			float n = 3.0f;
			Vector3 v3 = v1 * n;
			Assert::AreEqual(v3.x, 3.03f);
			Assert::AreEqual(v3.y, 3.03f);
			Assert::AreEqual(v3.z, 3.03f);

		}
		TEST_METHOD(TestScalarDivision)
		{
			Vector3 v1(3.03f);
			float n = 3.0f;
			Vector3 v3 = v1 / n;
			Assert::AreEqual(v3.x, 1.01f);
			Assert::AreEqual(v3.y, 1.01f);
			Assert::AreEqual(v3.z, 1.01f);


		}
		TEST_METHOD(TestAdditionAssignment)
		{
			Vector3 v1(1.01f);
			Vector3 v2(3.14f);
			v2 += v1;
			Assert::AreEqual(v2.x, 4.15f);
			Assert::AreEqual(v2.y, 4.15f);
			Assert::AreEqual(v2.z, 4.15f);


		}
		TEST_METHOD(TestSubtractionAssignment)
		{
			Vector3 v1(1.01f);
			Vector3 v2(3.14f);
			v2-=v1;
			Assert::AreEqual(v2.x, 2.13f);
			Assert::AreEqual(v2.y, 2.13f);
			Assert::AreEqual(v2.z, 2.13f);

		}
		TEST_METHOD(TestScalarMutiplicationAssignment)
		{
			Vector3 v1(1.01f);
			float n = 3.0f;
			v1 *=  n;
			Assert::AreEqual(v1.x, 3.03f);
			Assert::AreEqual(v1.y, 3.03f);
			Assert::AreEqual(v1.z, 3.03f);

		}
		TEST_METHOD(TestScalarDivisionAssignment)
		{
			Vector3 v1(3.03f);
			float n = 3.0f;
			v1 /= n;
			Assert::AreEqual(v1.x, 1.01f);
			Assert::AreEqual(v1.y, 1.01f);
			Assert::AreEqual(v1.z, 1.01f);

		}
		TEST_METHOD(TestDotProduct)
		{
			Vector3 v0(1.0f);
			Vector3 v1(2.0f);
			float r = Dot(v0,v1);
			Assert::AreEqual(r, 6.0f);



		}
		TEST_METHOD(TestMagnitudeSqrProduct)
		{
			Vector3 v0(1.0f);
			
			float r = MagnitudeSqr(v0);
			Assert::AreEqual(r, 3.0f);

		}
		TEST_METHOD(TestMagnitudeProduct)
		{
			Vector3 v0(3.0f,4.0f,0.0f);

			float r = Magnitude(v0);
			Assert::AreEqual(r, 5.0f);

		}
		TEST_METHOD(TestDistanceSqrProduct)
		{
			Vector3 v0(3.0f, 4.0f, 0.0f);
			Vector3 v1(4.0f, 5.0f, 0.0f);

			float r = DistanceSqr(v0,v1);
			Assert::AreEqual(r, 2.0f);

		}
		TEST_METHOD(TestDistanceProduct)
		{
			Vector3 v0(3.0f, 4.0f, 0.0f);
			Vector3 v1(6.0f, 8.0f, 0.0f);

			float r = Distance(v0, v1);
			Assert::AreEqual(r, 5.0f);

		}
		TEST_METHOD(TestNormalizeProduct)
		{
			Vector3 v0(3.0f, 4.0f, 0.0f);

			Vector3 r = Normalize(v0);
			Assert::AreEqual(r.x, 0.6f);
			Assert::AreEqual(r.y, 0.8f);
			Assert::AreEqual(r.z, 0.0f);

		}

		TEST_METHOD(TestCrossProduct)
		{
			Vector3 v0(3.0f, 4.0f, 0.0f);
			Vector3 v1(1.0f, 2.0f, 0.0f);

			Vector3 r = Cross(v0, v1);
			Assert::AreEqual(r.x, 0.0f);
			Assert::AreEqual(r.y, 0.0f);
			Assert::AreEqual(r.z, 2.0f);

		}
		TEST_METHOD(TestLerpProduct)
		{
			Vector3 v0(0.0f, 0.0f, 0.0f);
			Vector3 v1(3.0f, 4.0f, 0.0f);
			float t = 0.5f;

			Vector3 r = Lerp(v0, v1,t);
			Assert::AreEqual(r.x, 1.5f);
			Assert::AreEqual(r.y, 2.0f);
			Assert::AreEqual(r.z, 0.0f);

		}


	};
}
