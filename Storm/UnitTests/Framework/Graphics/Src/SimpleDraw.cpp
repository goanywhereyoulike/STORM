#include "Precompiled.h"
#include "SimpleDraw.h"
#include "Transform.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace Storm;
using namespace Storm::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);
		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
			mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mLineVertexCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color)
{
	// Call sInstance->AddLine 8 times here ...
	Math::Vector3 extend2{ extend.x,extend.y,-extend.z };
	Math::Vector3 extend3{ extend.x,-extend.y,extend.z };
	Math::Vector3 extend4{ extend.x,-extend.y,-extend.z };
	Math::Vector3 extend5{ -extend.x,extend.y,extend.z };
	Math::Vector3 extend6{ -extend.x,extend.y,-extend.z };
	Math::Vector3 extend7{ -extend.x,-extend.y,extend.z };
	Math::Vector3 extend8{ -extend.x,-extend.y,-extend.z };
	sInstance->AddLine(center + extend, center + extend3, color);
	sInstance->AddLine(center + extend3, center + extend7, color);
	sInstance->AddLine(center + extend7, center + extend5, color);
	sInstance->AddLine(center + extend5, center + extend, color);
	sInstance->AddLine(center + extend8, center + extend6, color);
	sInstance->AddLine(center + extend6, center + extend2, color);
	sInstance->AddLine(center + extend2, center + extend4, color);
	sInstance->AddLine(center + extend4, center + extend8, color);
	sInstance->AddLine(center + extend, center + extend2, color);
	sInstance->AddLine(center + extend3, center + extend4, color);
	sInstance->AddLine(center + extend7, center + extend8, color);
	sInstance->AddLine(center + extend5, center + extend6, color);

}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, size_t rings, size_t slices)
{
	const float thetaStep = Math::Constants::TwoPi / slices;
	const float phiStep = Math::Constants::Pi / rings;
	std::vector<Math::Vector3> points;
	//std::vector<Math::Vector3> circlepoints;
	std::vector<Math::Vector3> vpoints;
	points.reserve(slices);
	vpoints.reserve(slices);
	for (size_t j = 0; j < rings; ++j)
	{
		for (uint32_t c = 0; c < slices; ++c)
		{

			const float t = c * thetaStep;
			const float p = j * phiStep;
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius + center.x;
			const float y = -cos(p) * radius + center.y;
			const float z = cos(t) * discRadius + center.z;


			const float x1 = (-sin(t) * radius) * cos(p) + center.x;
			const float y1 = cos(t) * radius + center.y;
			const float z1 = (-sin(t) * radius) * sin(p) + center.z;
			Math::Vector3 point{ x,y,z };
			Math::Vector3 vpoint{ x1,y1,z1 };
			points.push_back(point);
			vpoints.push_back(vpoint);
		}

		for (size_t i = 0; i < slices - 1; ++i)
		{
			sInstance->AddLine(points[i], points[i + 1], color);
			sInstance->AddLine(vpoints[i], vpoints[i + 1], color);
		}
		sInstance->AddLine(points[slices - 1], points[0], color);
		sInstance->AddLine(vpoints[slices - 1], vpoints[0], color);
		points.clear();
		vpoints.clear();
	}
}





void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
	const float offset = size * -0.5f;
	const float columns = size + 1;
	const float rows = size + 1;
	std::vector<Math::Vector3> points;
	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float x = c + offset;
			const float y = 0.0f;
			const float z = r + offset;

			Math::Vector3 point{ x,y,z };

			points.push_back(point);
		}
	}
	for (uint32_t r = 0; r < rows; ++r)
	{
		sInstance->AddLine(points[r * rows], points[r * rows + rows - 1], color);
	}
	for (uint32_t c = 0; c < columns; ++c)
	{

		sInstance->AddLine(points[c], points[c + columns * (columns - 1)], color);
	}


}

void SimpleDraw::AddCylinder(const Math::Vector3& center, float radius, float height, const Color& color)
{
	const float thetaStep = Math::Constants::TwoPi / 20;
	std::vector<Math::Vector3> bottompoints;
	std::vector<Math::Vector3> toppoints;
	for (uint32_t c = 0; c < 20; ++c)
	{
		const float t = c * thetaStep;
		const float x = -sin(t) * radius + center.x;
		const float y1 = center.y - height / 2;
		const float y2 = center.y + height / 2;
		const float z = cos(t) * radius + center.z;
		Math::Vector3 point1{ x,y1,z };
		Math::Vector3 point2{ x,y2,z };
		bottompoints.push_back(point1);
		toppoints.push_back(point2);
	}

	for (size_t i = 0; i < 20 - 1; ++i)
	{
		sInstance->AddLine(bottompoints[i], bottompoints[i + 1], color);
		sInstance->AddLine(toppoints[i], toppoints[i + 1], color);

	}
	sInstance->AddLine(bottompoints[20 - 1], bottompoints[0], color);
	sInstance->AddLine(toppoints[20 - 1], toppoints[0], color);


	for (size_t i = 0; i < 20; ++i)
	{
		sInstance->AddLine(bottompoints[i], toppoints[i], color);
	}

}

void SimpleDraw::AddCapsule(const Math::Vector3& center, float radius, float height, const Color& color)
{
	const float middleheight = height - 2 * radius;
	const float halfmiddleheight = middleheight / 2;
	const float thetaStep = Math::Constants::TwoPi / 20;
	const float phiStep = Math::Constants::Pi / 10;
	std::vector<Math::Vector3> points;
	std::vector<Math::Vector3> circlepoints;
	std::vector<Math::Vector3> vpoints;
	std::vector<Math::Vector3> MaxCirclebottompoints;
	std::vector<Math::Vector3> MaxCircletoppoints;
	points.reserve(20);
	vpoints.reserve(20 / 2);
	MaxCirclebottompoints.reserve(20);
	MaxCirclebottompoints.reserve(20);

	//Bottom half sphere
	for (size_t j = 0; j < 6; ++j)
	{
		for (uint32_t c = 0; c < 20; ++c)
		{

			const float t = c * thetaStep;
			const float p = j * phiStep;
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius + center.x;
			const float y = -cos(p) * radius - halfmiddleheight + center.y;
			const float z = cos(t) * discRadius + center.z;

			Math::Vector3 point{ x,y,z };

			points.push_back(point);
			if (j == 5)
			{
				MaxCirclebottompoints.push_back(point);
			}

		}

		for (size_t i = 0; i < 20 - 1; ++i)
		{
			sInstance->AddLine(points[i], points[i + 1], color);

		}
		sInstance->AddLine(points[20 - 1], points[0], color);

		points.clear();

	}

	for (size_t j = 0; j < 10; ++j)
	{
		for (size_t c = 20 / 4; c < 20 / 4 * 3 + 1; ++c)
		{
			const float p = j * phiStep;
			const float t = c * thetaStep;
			const float x1 = (-sin(t) * radius) * cos(p) + center.x;
			const float y1 = cos(t) * radius + center.y - halfmiddleheight;
			const float z1 = (-sin(t) * radius) * sin(p) + center.z;
			Math::Vector3 vpoint{ x1,y1,z1 };
			vpoints.push_back(vpoint);

		}

		for (size_t i = 0; i < vpoints.size() - 1; ++i)
		{
			sInstance->AddLine(vpoints[i], vpoints[i + 1], color);
		}
		vpoints.clear();
	}

	//Middle part
	for (size_t i = 0; i < MaxCirclebottompoints.size(); ++i)
	{
		Math::Vector3 tpoint = MaxCirclebottompoints[i];
		tpoint.y += middleheight;
		MaxCircletoppoints.push_back(tpoint);

	}

	for (size_t i = 0; i < MaxCirclebottompoints.size(); ++i)
	{
		sInstance->AddLine(MaxCirclebottompoints[i], MaxCircletoppoints[i], color);

	}
	MaxCirclebottompoints.clear();
	MaxCircletoppoints.clear();

	//Top half sphere
	for (size_t j = 5; j < 10; ++j)
	{
		for (uint32_t c = 0; c < 20; ++c)
		{

			const float t = c * thetaStep;
			const float p = j * phiStep;
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius + center.x;
			const float y = -cos(p) * radius + halfmiddleheight + center.y;
			const float z = cos(t) * discRadius + center.z;

			Math::Vector3 point{ x,y,z };

			points.push_back(point);


		}

		for (size_t i = 0; i < 20 - 1; ++i)
		{
			sInstance->AddLine(points[i], points[i + 1], color);

		}
		sInstance->AddLine(points[20 - 1], points[0], color);

		points.clear();

	}

	for (size_t j = 0; j < 10; ++j)
	{
		for (size_t c = 20 / 4 * 3; c < 20 / 4 * 5 + 1; ++c)
		{
			const float p = j * phiStep;
			const float t = c * thetaStep;
			const float x1 = (-sin(t) * radius) * cos(p) + center.x;
			const float y1 = cos(t) * radius + center.y + halfmiddleheight;
			const float z1 = (-sin(t) * radius) * sin(p) + center.z;
			Math::Vector3 vpoint{ x1,y1,z1 };
			vpoints.push_back(vpoint);

		}

		for (size_t i = 0; i < vpoints.size() - 1; ++i)
		{
			sInstance->AddLine(vpoints[i], vpoints[i + 1], color);
		}
		vpoints.clear();
	}


}

void Storm::Graphics::SimpleDraw::AddOBB(const Math::OBB& obb, const Color& color)
{
	Math::Vector3 points[] =
	{
		{-1.0f,-1.0f,-1.0f},
		{-1.0f,+1.0f,-1.0f},
		{+1.0f,+1.0f,-1.0f},
		{+1.0f,-1.0f,-1.0f},
		{-1.0f,-1.0f,+1.0f},
		{-1.0f,+1.0f,+1.0f},
		{+1.0f,+1.0f,+1.0f},
		{+1.0f,-1.0f,+1.0f},
	};

	Math::Matrix4 transform = Transform{ obb.center,obb.rotation,obb.extend }.GetMatrix4();

	for (auto& point : points)
	{
		point = Math::TransformCoord(point, transform);
	}

	sInstance->AddLine(points[0], points[1], color);
	sInstance->AddLine(points[1], points[2], color);
	sInstance->AddLine(points[2], points[3], color);
	sInstance->AddLine(points[3], points[0], color);

	sInstance->AddLine(points[4], points[5], color);
	sInstance->AddLine(points[5], points[6], color);
	sInstance->AddLine(points[6], points[7], color);
	sInstance->AddLine(points[7], points[4], color);

	sInstance->AddLine(points[0], points[4], color);
	sInstance->AddLine(points[1], points[5], color);
	sInstance->AddLine(points[2], points[6], color);
	sInstance->AddLine(points[3], points[7], color);

}

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	Math::Vector3 Xaxis{ transform._11,transform._12,transform._13 };
	Math::Vector3 Yaxis{ transform._21,transform._22,transform._23 };
	Math::Vector3 Zaxis{ transform._31,transform._32,transform._33 };
	Math::Vector3 Origin{ 0.0f,0.0f,0.0f };
	sInstance->AddLine(Origin, Xaxis, Colors::Red);
	sInstance->AddLine(Origin, Yaxis, Colors::Green);
	sInstance->AddLine(Origin, Zaxis, Colors::Blue);

}

void SimpleDraw::AddCone(const Math::Vector3& center, float radius, float height, const Color& color)
{
	const float thetaStep = Math::Constants::TwoPi / 20;
	std::vector<Math::Vector3> points;
	for (uint32_t c = 0; c < 20; ++c)
	{
		const float t = c * thetaStep;
		const float x = -sin(t) * radius + center.x;
		const float y = center.y;
		const float z = cos(t) * radius + center.z;
		Math::Vector3 point{ x,y,z };
		points.push_back(point);
	}

	for (size_t i = 0; i < 20 - 1; ++i)
	{
		sInstance->AddLine(points[i], points[i + 1], color);


	}
	sInstance->AddLine(points[20 - 1], points[0], color);

	std::vector<Math::Vector3> circlepoints;
	circlepoints.push_back(Math::Vector3{ center.x + radius,center.y,center.z });
	circlepoints.push_back(Math::Vector3{ center.x - radius,center.y,center.z });
	circlepoints.push_back(Math::Vector3{ center.x ,center.y,center.z + radius });
	circlepoints.push_back(Math::Vector3{ center.x ,center.y,center.z - radius });

	Math::Vector3 Top{ center.x,center.y + height,center.z };
	for (size_t i = 0; i < circlepoints.size(); ++i)
	{
		sInstance->AddLine(circlepoints[i], Top, color);
	}

}
