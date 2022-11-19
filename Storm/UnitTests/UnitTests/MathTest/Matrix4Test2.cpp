#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Storm::Math;

namespace MathTest
{
	TEST_CLASS(Matrix4Test2)
	{
	public:
		template <class T>
		void Compare(const T& a, const T& b)
		{
			for (size_t i = 0; i < std::size(a.v); ++i)
				Assert::AreEqual(a.v[i], b.v[i], 0.00001f);
		}

		TEST_METHOD(TestIdentity)
		{
			Compare(
				Matrix4::Identity,
				{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }
			);
		}

		TEST_METHOD(TestTranslation)
		{
			Compare(
				Matrix4::Translation({ 2.2f, 3.3f, 4.4f }),
				{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2.2f, 3.3f, 4.4f, 1 }
			);
		}

		TEST_METHOD(TestScalingWithScalar)
		{
			Compare(
				Matrix4::Scaling(0.77f),
				{ 0.77f, 0, 0, 0, 0, 0.77f, 0, 0, 0, 0, 0.77f, 0, 0, 0, 0, 1 }
			);
		}

		TEST_METHOD(TestScalingWithVector)
		{
			Compare(
				Matrix4::Scaling({ 1.2f, 2.3f, 3.4f }),
				{ 1.2f, 0, 0, 0, 0, 2.3f, 0, 0, 0, 0, 3.4f, 0, 0, 0, 0, 1 }
			);
		}

		TEST_METHOD(TestRotationX)
		{
			Compare(
				Matrix4::RotationX(0.47f),
				{ 1, 0, 0, 0, 0, cos(0.47f), sin(0.47f), 0, 0, -sin(0.47f), cos(0.47f), 0, 0, 0, 0, 1 }
			);
		}

		TEST_METHOD(TestRotationY)
		{
			Compare(
				Matrix4::RotationY(0.33f),
				{ cos(0.33f), 0, -sin(0.33f), 0, 0, 1, 0, 0, sin(0.33f), 0, cos(0.33f), 0, 0, 0, 0, 1 }
			);
		}

		TEST_METHOD(TestRotationZ)
		{
			Compare(
				Matrix4::RotationZ(0.26f),
				{ cos(0.26f), sin(0.26f), 0, 0, -sin(0.26f), cos(0.26f), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }
			);
		}

		TEST_METHOD(TestRotationAxis)
		{
			Compare(Matrix4::RotationAxis(Vector3::XAxis, 0.47f), Matrix4::RotationX(0.47f));
			Compare(Matrix4::RotationAxis(Vector3::YAxis, 0.33f), Matrix4::RotationY(0.33f));
			Compare(Matrix4::RotationAxis(Vector3::ZAxis, 0.26f), Matrix4::RotationZ(0.26f));
			Compare(
				Matrix4::RotationAxis(Normalize({ 1.0f, 1.0f, 1.0f }), 0.22f),
				{
					0.983931601f,
					0.134029120f,
					-0.117960744f,
					0.000000000f,
					-0.117960744f,
					0.983931601f,
					0.134029120f,
					0.000000000f,
					0.134029120f,
					-0.117960744f,
					0.983931601f,
					0.000000000f,
					0.000000000f,
					0.000000000f,
					0.000000000f,
					1.00000000f
				}
			);
		}

		TEST_METHOD(TestMultiplicationByScalar)
		{
			Compare(
				Matrix4::Identity * 3.7f,
				{
					3.7f, 0.0f, 0.0f, 0.0f,
					0.0f, 3.7f, 0.0f, 0.0f,
					0.0f, 0.0f, 3.7f, 0.0f,
					0.0f, 0.0f, 0.0f, 3.7f
				}
			);
		}

		TEST_METHOD(TestMultiplicationByMatrix4)
		{
			Compare(
				Matrix4::RotationX(0.47f) * Matrix4::RotationY(0.33f),
				{
					0.946042359f,
					0.0f,
					-0.324043036f,
					0.0f,
					0.146754652f,
					0.891568303f,
					0.428449601f,
					0.0f,
					0.288906485f,
					-0.452886283f,
					0.843461394f,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f,
				}
			);
		}

		TEST_METHOD(TestTransformCoord)
		{
			Compare(
				TransformCoord({ 0, 1, 0 }, Matrix4::RotationX(Constants::HalfPi) * Matrix4::Translation({ 1, 0, 1 })),
				{ 1, 0, 2 }
			);
		}

		TEST_METHOD(TestTransformNormal)
		{
			Compare(
				TransformNormal({ 0, 1, 0 }, Matrix4::RotationZ(Constants::HalfPi) * Matrix4::Translation({ 1, 0, 1 })),
				{ -1, 0, 0 }
			);
		}

		TEST_METHOD(TestDeterminant)
		{
			Assert::AreEqual(
				Determinant({ 1, -1, 2, 1, 2, -3, 1, 3, 1, 2, 3, 1, 0, 1, 0, 3 }),
				23.0f,
				0.00001f
			);
		}

		TEST_METHOD(TestTranspose)
		{
			Compare(
				Transpose({ 1, -1, 2, 1, 2, -3, 1, 3, 1, 2, 3, 1, 0, 1, 0, 3 }),
				{ 1, 2, 1, 0, -1, -3, 2, 1, 2, 1, 3, 0, 1, 3, 1, 3 }
			);
		}

		TEST_METHOD(TestAdjoint)
		{
			Compare(
				Adjoint({ 1, -1, 2, 1, 2, -3, 1, 3, 1, 2, 3, 1, 0, 1, 0, 3 }),
				{ -41, 22, 20, -15, -15, 3, 9, -1, 22, -9, -4, 3, 5, -1, -3, 8 }
			);
		}

		TEST_METHOD(TestInverse)
		{
			Compare(
				Inverse({ 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 1, 2, -5, 8, 1 }),
				{ 0, -1, 0, 0, 1, 0, 0, 0, 0.625f, 0.25f, -0.125f, 0.125f, 0, 0, 1, 0 }
			);
		}




	};
}