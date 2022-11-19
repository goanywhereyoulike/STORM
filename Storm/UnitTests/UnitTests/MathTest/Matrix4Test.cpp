#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Storm::Math;

namespace MathTest
{
	TEST_CLASS(Matrix4Test)
	{
	public:

		TEST_METHOD(TestIdentityProduct)
		{
			Matrix4 m0 = Matrix4::Identity;
			Assert::AreEqual(m0._11, 1.0f);
			Assert::AreEqual(m0._12, 0.0f);
			Assert::AreEqual(m0._13, 0.0f);
			Assert::AreEqual(m0._14, 0.0f);
			Assert::AreEqual(m0._21, 0.0f);
			Assert::AreEqual(m0._22, 1.0f);
			Assert::AreEqual(m0._23, 0.0f);
			Assert::AreEqual(m0._24, 0.0f);
			Assert::AreEqual(m0._31, 0.0f);
			Assert::AreEqual(m0._32, 0.0f);
			Assert::AreEqual(m0._33, 1.0f);
			Assert::AreEqual(m0._34, 0.0f);
			Assert::AreEqual(m0._41, 0.0f);
			Assert::AreEqual(m0._42, 0.0f);
			Assert::AreEqual(m0._43, 0.0f);
			Assert::AreEqual(m0._44, 1.0f);

		}

		TEST_METHOD(TestTranslationProduct)
		{
			Matrix4 m0 = Matrix4::Translation(Vector3{ 1.0f,2.0f,3.0f });
			Assert::AreEqual(m0._11, 1.0f);
			Assert::AreEqual(m0._12, 0.0f);
			Assert::AreEqual(m0._13, 0.0f);
			Assert::AreEqual(m0._14, 0.0f);
			Assert::AreEqual(m0._21, 0.0f);
			Assert::AreEqual(m0._22, 1.0f);
			Assert::AreEqual(m0._23, 0.0f);
			Assert::AreEqual(m0._24, 0.0f);
			Assert::AreEqual(m0._31, 0.0f);
			Assert::AreEqual(m0._32, 0.0f);
			Assert::AreEqual(m0._33, 1.0f);
			Assert::AreEqual(m0._34, 0.0f);
			Assert::AreEqual(m0._41, 1.0f);
			Assert::AreEqual(m0._42, 2.0f);
			Assert::AreEqual(m0._43, 3.0f);
			Assert::AreEqual(m0._44, 1.0f);

		}

		TEST_METHOD(TestScalingProduct)
		{
			Matrix4 m0 = Matrix4::Scaling(3.0f);
			Assert::AreEqual(m0._11, 3.0f);
			Assert::AreEqual(m0._12, 0.0f);
			Assert::AreEqual(m0._13, 0.0f);
			Assert::AreEqual(m0._14, 0.0f);
			Assert::AreEqual(m0._21, 0.0f);
			Assert::AreEqual(m0._22, 3.0f);
			Assert::AreEqual(m0._23, 0.0f);
			Assert::AreEqual(m0._24, 0.0f);
			Assert::AreEqual(m0._31, 0.0f);
			Assert::AreEqual(m0._32, 0.0f);
			Assert::AreEqual(m0._33, 3.0f);
			Assert::AreEqual(m0._34, 0.0f);
			Assert::AreEqual(m0._41, 0.0f);
			Assert::AreEqual(m0._42, 0.0f);
			Assert::AreEqual(m0._43, 0.0f);
			Assert::AreEqual(m0._44, 1.0f);

		}

		TEST_METHOD(TestScalingProduct2)
		{
			Matrix4 m0 = Matrix4::Scaling(Vector3{ 1.0f,2.0f,3.0f });
			Assert::AreEqual(m0._11, 1.0f);
			Assert::AreEqual(m0._12, 0.0f);
			Assert::AreEqual(m0._13, 0.0f);
			Assert::AreEqual(m0._14, 0.0f);
			Assert::AreEqual(m0._21, 0.0f);
			Assert::AreEqual(m0._22, 2.0f);
			Assert::AreEqual(m0._23, 0.0f);
			Assert::AreEqual(m0._24, 0.0f);
			Assert::AreEqual(m0._31, 0.0f);
			Assert::AreEqual(m0._32, 0.0f);
			Assert::AreEqual(m0._33, 3.0f);
			Assert::AreEqual(m0._34, 0.0f);
			Assert::AreEqual(m0._41, 0.0f);
			Assert::AreEqual(m0._42, 0.0f);
			Assert::AreEqual(m0._43, 0.0f);
			Assert::AreEqual(m0._44, 1.0f);

		}

		TEST_METHOD(TestRotationXProduct)
		{
			Matrix4 m0 = Matrix4::RotationX(0.47f);
			Assert::AreEqual(m0._11, 1.0f);
			Assert::AreEqual(m0._12, 0.0f);
			Assert::AreEqual(m0._13, 0.0f);
			Assert::AreEqual(m0._14, 0.0f);
			Assert::AreEqual(m0._21, 0.0f);
			Assert::AreEqual(m0._22, cos(0.47f));
			Assert::AreEqual(m0._23, sin(0.47f));
			Assert::AreEqual(m0._24, 0.0f);
			Assert::AreEqual(m0._31, 0.0f);
			Assert::AreEqual(m0._32, -sin(0.47f));
			Assert::AreEqual(m0._33, cos(0.47f));
			Assert::AreEqual(m0._34, 0.0f);
			Assert::AreEqual(m0._41, 0.0f);
			Assert::AreEqual(m0._42, 0.0f);
			Assert::AreEqual(m0._43, 0.0f);
			Assert::AreEqual(m0._44, 1.0f);

		}
		TEST_METHOD(TestRotationYProduct)
		{
			Matrix4 m0 = Matrix4::RotationY(0.33f);
			Matrix4 m1 = { cos(0.33f), 0, -sin(0.33f), 0, 0, 1, 0, 0, sin(0.33f), 0, cos(0.33f), 0, 0, 0, 0, 1 };
			Assert::AreEqual(m0._11, m1._11);
			Assert::AreEqual(m0._12, m1._12);
			Assert::AreEqual(m0._13, m1._13);
			Assert::AreEqual(m0._14, m1._14);
			Assert::AreEqual(m0._21, m1._21);
			Assert::AreEqual(m0._22, m1._22);
			Assert::AreEqual(m0._23, m1._23);
			Assert::AreEqual(m0._24, m1._24);
			Assert::AreEqual(m0._33, m1._33);
			Assert::AreEqual(m0._34, m1._34);
			Assert::AreEqual(m0._41, m1._41);
			Assert::AreEqual(m0._42, m1._42);
			Assert::AreEqual(m0._43, m1._43);
			Assert::AreEqual(m0._44, m1._44);

		}

		TEST_METHOD(TestRotationZProduct)
		{
			Matrix4 m0 = Matrix4::RotationZ(0.26f);
			Matrix4 m1 = { cos(0.26f), sin(0.26f), 0, 0, -sin(0.26f), cos(0.26f), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
			Assert::AreEqual(m0._11, m1._11);
			Assert::AreEqual(m0._12, m1._12);
			Assert::AreEqual(m0._13, m1._13);
			Assert::AreEqual(m0._14, m1._14);
			Assert::AreEqual(m0._21, m1._21);
			Assert::AreEqual(m0._22, m1._22);
			Assert::AreEqual(m0._23, m1._23);
			Assert::AreEqual(m0._24, m1._24);
			Assert::AreEqual(m0._33, m1._33);
			Assert::AreEqual(m0._34, m1._34);
			Assert::AreEqual(m0._41, m1._41);
			Assert::AreEqual(m0._42, m1._42);
			Assert::AreEqual(m0._43, m1._43);
			Assert::AreEqual(m0._44, m1._44);

		}

		TEST_METHOD(TestRotationAxisProduct)
		{
			Matrix4 m0 = Matrix4::RotationAxis(Vector3::ZAxis, 0.26f);
			Matrix4 m1 = { cos(0.26f), sin(0.26f), 0, 0, -sin(0.26f), cos(0.26f), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
			Assert::AreEqual(m0._11, m1._11);
			Assert::AreEqual(m0._12, m1._12);
			Assert::AreEqual(m0._13, m1._13);
			Assert::AreEqual(m0._14, m1._14);
			Assert::AreEqual(m0._21, m1._21);
			Assert::AreEqual(m0._22, m1._22);
			Assert::AreEqual(m0._23, m1._23);
			Assert::AreEqual(m0._24, m1._24);
			Assert::AreEqual(m0._33, m1._33);
			Assert::AreEqual(m0._34, m1._34);
			Assert::AreEqual(m0._41, m1._41);
			Assert::AreEqual(m0._42, m1._42);
			Assert::AreEqual(m0._43, m1._43);
			Assert::AreEqual(m0._44, m1._44);

		}


		TEST_METHOD(TestTransposeProduct)
		{
			Matrix4 m0 = Matrix4::Identity;

			Matrix4 m1 = Transpose(m0);
			Assert::AreEqual(m0._11, m1._11);
			Assert::AreEqual(m0._12, m1._21);
			Assert::AreEqual(m0._13, m1._31);
			Assert::AreEqual(m0._14, m1._41);
			Assert::AreEqual(m0._21, m1._12);
			Assert::AreEqual(m0._22, m1._22);
			Assert::AreEqual(m0._23, m1._32);
			Assert::AreEqual(m0._24, m1._42);
			Assert::AreEqual(m0._33, m1._33);
			Assert::AreEqual(m0._34, m1._43);
			Assert::AreEqual(m0._41, m1._14);
			Assert::AreEqual(m0._42, m1._24);
			Assert::AreEqual(m0._43, m1._34);
			Assert::AreEqual(m0._44, m1._44);

		}

		TEST_METHOD(TestDeterminantProduct)
		{
			Matrix4 m0 = { 1, -1, 2, 1, 2, -3, 1, 3, 1, 2, 3, 1, 0, 1, 0, 3 };
			float r = Determinant(m0);
			Assert::AreEqual(r, 23.0f);
		}

		TEST_METHOD(TestCofactorProduct)
		{
			Matrix4 m0 = { 1, -1, 2, 1, 2, -3, 1, 3, 1, 2, 3, 1, 0, 1, 0, 3 };
			Matrix4 m1 = Cofactor(m0);
			Matrix4 m2 = { -41,-15,22,5,22,3,-9,-1,20,9,-4,-3,-15,-1,3,8 };
			Assert::AreEqual(m1._12, m2._12);
			Assert::AreEqual(m1._13, m2._13);
			Assert::AreEqual(m1._14, m2._14);
			Assert::AreEqual(m1._21, m2._21);
			Assert::AreEqual(m1._22, m2._22);
			Assert::AreEqual(m1._23, m2._23);
			Assert::AreEqual(m1._24, m2._24);
			Assert::AreEqual(m1._33, m2._33);
			Assert::AreEqual(m1._34, m2._34);
			Assert::AreEqual(m1._41, m2._41);
			Assert::AreEqual(m1._42, m2._42);
			Assert::AreEqual(m1._43, m2._43);
			Assert::AreEqual(m1._44, m2._44);

		}
		TEST_METHOD(TestAdjointProduct)
		{
			Matrix4 m0 = { 1, -1, 2, 1, 2, -3, 1, 3, 1, 2, 3, 1, 0, 1, 0, 3 };
			Matrix4 m1 = Adjoint(m0);
			Matrix4 m2 = { -41, 22, 20, -15, -15, 3, 9, -1, 22, -9, -4, 3, 5, -1, -3, 8 };
			Assert::AreEqual(m1._12, m2._12);
			Assert::AreEqual(m1._13, m2._13);
			Assert::AreEqual(m1._14, m2._14);
			Assert::AreEqual(m1._21, m2._21);
			Assert::AreEqual(m1._22, m2._22);
			Assert::AreEqual(m1._23, m2._23);
			Assert::AreEqual(m1._24, m2._24);
			Assert::AreEqual(m1._33, m2._33);
			Assert::AreEqual(m1._34, m2._34);
			Assert::AreEqual(m1._41, m2._41);
			Assert::AreEqual(m1._42, m2._42);
			Assert::AreEqual(m1._43, m2._43);
			Assert::AreEqual(m1._44, m2._44);

		}

		TEST_METHOD(TestInverseProduct)
		{
			Matrix4 m0 = { 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 1, 2, -5, 8, 1 };
			Matrix4 m1 = Inverse(m0);
			Matrix4 m2 = { 0, -1, 0, 0, 1, 0, 0, 0, 0.625f, 0.25f, -0.125f, 0.125f, 0, 0, 1, 0 };
			Assert::AreEqual(m1._12, m2._12);
			Assert::AreEqual(m1._13, m2._13);
			Assert::AreEqual(m1._14, m2._14);
			Assert::AreEqual(m1._21, m2._21);
			Assert::AreEqual(m1._22, m2._22);
			Assert::AreEqual(m1._23, m2._23);
			Assert::AreEqual(m1._24, m2._24);
			Assert::AreEqual(m1._33, m2._33);
			Assert::AreEqual(m1._34, m2._34);
			Assert::AreEqual(m1._41, m2._41);
			Assert::AreEqual(m1._42, m2._42);
			Assert::AreEqual(m1._43, m2._43);
			Assert::AreEqual(m1._44, m2._44);

		}

		TEST_METHOD(TestTransformCoordProduct)
		{
			Vector3 v0 = { 0, 1, 0 };
			Matrix4 m0 =  Matrix4::Translation({ 1, 0,1 });
			Vector3 v1 = TransformCoord(v0, m0);
			Vector3 v2 = { 1,1,1 };
			Assert::AreEqual(v1.x, v2.x);
			Assert::AreEqual(v1.y, v2.y);
			Assert::AreEqual(v1.z, v2.z);


		}
		TEST_METHOD(TestTransformNormalProduct)
		{
			Vector3 v0 = { 0, 1, 0 };
	
			Vector3 v1 = TransformNormal(v0,  Matrix4::Translation({ 1, 0,1 }));
			Vector3 v2 = { 0,1,0};
			Assert::AreEqual(v1.x, v2.x);
			Assert::AreEqual(v1.y, v2.y);
			Assert::AreEqual(v1.z, v2.z);


		}




	};
}