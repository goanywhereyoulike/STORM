#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Storm::Math;

namespace MathTest
{
	TEST_CLASS(QuaterionTest)
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			Quaternion q0;
			Assert::AreEqual(q0.w, 1.0f);
			Assert::AreEqual(q0.x, 0.0f);
			Assert::AreEqual(q0.y, 0.0f);
			Assert::AreEqual(q0.z, 0.0f);

			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(q1.w, 1.0f);
			Assert::AreEqual(q1.x, 2.0f);
			Assert::AreEqual(q1.y, 3.0f);
			Assert::AreEqual(q1.z, 4.0f);
		}

		TEST_METHOD(TestStatics)
		{
			Quaternion identity = Quaternion::Identity;
			Assert::AreEqual(identity.w, 1.0f);
			Assert::AreEqual(identity.x, 0.0f);
			Assert::AreEqual(identity.y, 0.0f);
			Assert::AreEqual(identity.z, 0.0f);
		}

		TEST_METHOD(TestNegation)
		{
			Quaternion q0(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q1 = -q0;

			Assert::AreEqual(q1.w, -1.0f);
			Assert::AreEqual(q1.x, -2.0f);
			Assert::AreEqual(q1.y, -3.0f);
			Assert::AreEqual(q1.z, -4.0f);
		}

		TEST_METHOD(TestAddition)
		{
			Quaternion q0(1.0f, 1.0f, 1.0f, 1.5f);
			Quaternion q1(1.0f, -1.0f, 0.0f, -0.5f);
			Quaternion q2 = q0 + q1;

			Assert::AreEqual(q2.w, 2.0f);
			Assert::AreEqual(q2.x, 0.0f);
			Assert::AreEqual(q2.y, 1.0f);
			Assert::AreEqual(q2.z, 1.0f);
		}

		TEST_METHOD(TestSubtraction)
		{
			Quaternion q0(1.0f, 1.0f, 1.0f, 1.5f);
			Quaternion q1(1.0f, -1.0f, 0.0f, -0.5f);
			Quaternion q2 = q0 - q1;

			Assert::AreEqual(q2.w, 0.0f);
			Assert::AreEqual(q2.x, 2.0f);
			Assert::AreEqual(q2.y, 1.0f);
			Assert::AreEqual(q2.z, 2.0f);
		}

		TEST_METHOD(TestScalarMultiply)
		{
			Quaternion q0(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q1 = q0 * 0.5f;

			Assert::AreEqual(q1.w, 0.5f);
			Assert::AreEqual(q1.x, 1.0f);
			Assert::AreEqual(q1.y, 1.5f);
			Assert::AreEqual(q1.z, 2.0f);
		}

		TEST_METHOD(TestMultiply)
		{
			Quaternion q0(1.5f, 1.0f, 1.0f, 1.0f);
			Quaternion q1(-0.5f, 1.0f, -1.0f, 0.0f);
			Quaternion q2 = q0 * q1;

			Assert::AreEqual(q2.w, -0.75f);
			Assert::AreEqual(q2.x, 0.0f);
			Assert::AreEqual(q2.y, -3.0f);
			Assert::AreEqual(q2.z, 1.5f);
		}

		TEST_METHOD(TestFromAxisAngle)
		{
			Quaternion q0 = Quaternion::RotationAxis({ 1.0f, -1.0f, 2.0f }, 1.5f);

			Assert::AreEqual(q0.w, 0.7316889f, 0.000001f);
			Assert::AreEqual(q0.x, 0.2782779f, 0.000001f);
			Assert::AreEqual(q0.y, -0.2782779f, 0.000001f);
			Assert::AreEqual(q0.z, 0.5565557f, 0.000001f);
		}

		TEST_METHOD(TestFromMatrix)
		{
			Quaternion qx = Quaternion::RotationMatrix(Matrix4::RotationX(0.72f));
			Quaternion qy = Quaternion::RotationMatrix(Matrix4::RotationY(0.84f));
			Quaternion qz = Quaternion::RotationMatrix(Matrix4::RotationZ(0.96f));
			Quaternion qa = Quaternion::RotationMatrix(Matrix4::RotationAxis({ 3.0f, -2.0f, 1.0f }, 0.44f));

			Assert::AreEqual(qx.w, 0.9358968f, 0.000001f);
			Assert::AreEqual(qx.x, 0.3522742f, 0.000001f);
			Assert::AreEqual(qx.y, 0.0f, 0.000001f);
			Assert::AreEqual(qx.z, 0.0f, 0.000001f);

			Assert::AreEqual(qy.w, 0.9130889f, 0.000001f);
			Assert::AreEqual(qy.x, 0.0f, 0.000001f);
			Assert::AreEqual(qy.y, 0.4077605f, 0.000001f);
			Assert::AreEqual(qy.z, 0.0f, 0.000001f);

			Assert::AreEqual(qz.w, 0.8869949f, 0.000001f);
			Assert::AreEqual(qz.x, 0.0f, 0.000001f);
			Assert::AreEqual(qz.y, 0.0f, 0.000001f);
			Assert::AreEqual(qz.z, 0.4617792f, 0.000001f);

			Assert::AreEqual(qa.w, 0.9758974f, 0.000001f);
			Assert::AreEqual(qa.x, 0.174973f, 0.000001f);
			Assert::AreEqual(qa.y, -0.1166486f, 0.000001f);
			Assert::AreEqual(qa.z, 0.0583243f, 0.000001f);
		}

		TEST_METHOD(TestRotationLook)
		{
			Quaternion q0 = Quaternion::RotationLook({ 1.0f, 1.0f, 1.0f });

			Assert::AreEqual(q0.w, 0.880476236f, 0.000001f);
			Assert::AreEqual(q0.x, -0.279848129f, 0.000001f);
			Assert::AreEqual(q0.y, 0.364705175f, 0.000001f);
			Assert::AreEqual(q0.z, 0.115916900f, 0.000001f);
		}

		TEST_METHOD(TestRotationFromTo)
		{
			Quaternion q0 = Quaternion::RotationFromTo({ 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });

			Assert::AreEqual(q0.w, 0.888073862f, 0.000001f);
			Assert::AreEqual(q0.x, -0.325057596f, 0.000001f);
			Assert::AreEqual(q0.y, 0.325057596f, 0.000001f);
			Assert::AreEqual(q0.z, 0.000000000f, 0.000001f);
		}

		TEST_METHOD(TestConvertToMatrix)
		{
			Vector3 axis{ -1.0f, 1.0f, 1.0f };
			float angle = 0.45f;
			Matrix4 m0 = Matrix4::RotationQuaternion(Quaternion::RotationAxis(axis, angle));
			Matrix4 m1 = Matrix4::RotationAxis(axis, angle);

			for (size_t i = 0; i < std::size(m0.v); ++i)
				Assert::AreEqual(m0.v[i], m1.v[i], 0.000001f);
		}

		TEST_METHOD(TestMagitude)
		{
			Quaternion q0(1.0f, 2.0f, 3.0f, 4.0f);
			float magnitude = Magnitude(q0);

			Assert::AreEqual(magnitude, 5.47722578f, 0.000001f);
		}

		TEST_METHOD(TestNormalize)
		{
			Quaternion q0(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q1 = Normalize(q0);

			Assert::AreEqual(q1.w, 0.1825742f, 0.000001f);
			Assert::AreEqual(q1.x, 0.3651484f, 0.000001f);
			Assert::AreEqual(q1.y, 0.5477226f, 0.000001f);
			Assert::AreEqual(q1.z, 0.7302967f, 0.000001f);
		}

		TEST_METHOD(TestSlerp)
		{
			Quaternion q0 = Quaternion::RotationAxis({ 1.0f, 2.0f, 3.0f }, 4.0f);
			Quaternion q1 = Quaternion::RotationAxis({ 1.0f, -1.0f, 2.0f }, -2.0f);
			Quaternion q2 = Slerp(q0, q1, 0.47f);

			Assert::AreEqual(q2.w, -0.523719609f, 0.000001f);
			Assert::AreEqual(q2.x, 0.320419908f, 0.000001f);
			Assert::AreEqual(q2.y, 0.104300126f, 0.000001f);
			Assert::AreEqual(q2.z, 0.782413125f, 0.000001f);
		}
	};
}

