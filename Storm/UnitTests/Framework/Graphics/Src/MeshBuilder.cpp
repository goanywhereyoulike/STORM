#include "Precompiled.h"
#include "MeshBuilder.h"
using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Math;

MeshPC MeshBuilder::CreateCubePC()
{
	MeshPC mesh;
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, 1.0f },Colors::Yellow });     //0
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, -1.0f },Colors::White });     //1
	mesh.vertices.push_back({ Vector3{  1.0f,  -1.0f, 1.0f },Colors::Brown });     //2
	mesh.vertices.push_back({ Vector3{  -1.0f,  1.0f, 1.0f },Colors::Green });     //3
	mesh.vertices.push_back({ Vector3{  1.0f,  -1.0f, -1.0f },Colors::Red });      //4
	mesh.vertices.push_back({ Vector3{  -1.0f,  1.0f, -1.0f },Colors::Blue });     //5
	mesh.vertices.push_back({ Vector3{  -1.0f,  -1.0f, 1.0f },Colors::Pink });     //6
	mesh.vertices.push_back({ Vector3{  -1.0f,  -1.0f, -1.0f },Colors::Purple });  //7

	mesh.indices.push_back(5);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);

	mesh.indices.push_back(1);
	mesh.indices.push_back(4);
	mesh.indices.push_back(5);

	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(4);

	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(1);

	mesh.indices.push_back(0);
	mesh.indices.push_back(6);
	mesh.indices.push_back(2);

	mesh.indices.push_back(3);
	mesh.indices.push_back(6);
	mesh.indices.push_back(0);

	mesh.indices.push_back(3);
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);

	mesh.indices.push_back(5);
	mesh.indices.push_back(7);
	mesh.indices.push_back(3);

	mesh.indices.push_back(3);
	mesh.indices.push_back(1);
	mesh.indices.push_back(5);

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(3);

	mesh.indices.push_back(7);
	mesh.indices.push_back(2);
	mesh.indices.push_back(6);

	mesh.indices.push_back(4);
	mesh.indices.push_back(2);
	mesh.indices.push_back(7);

	return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(uint32_t columns, uint32_t rows, float spacing)
{
	ASSERT(columns >= 2 && rows >= 2, "Invalid arguments for plane.");
	ASSERT(spacing > 0.0f, "Invalid arguments for plane.");
	MeshPX mesh;

	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;
	const float width = (columns - 1) * spacing;
	const float length = (rows - 1) * spacing;
	const float offsetX = width * -0.5f;
	const float offsetZ = length * -0.5f;
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float x = (c * spacing) + offsetX;
			const float y = 0.0f;
			const float z = (r * spacing) + offsetZ;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},{u,v} });
		}
	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;



}

MeshPX MeshBuilder::CreateCylinderPX(uint32_t slices, uint32_t rings, float radius, float height)
{
	ASSERT(slices >= 3 && rings >= 2, "Invalid arguments for cylinder.");
	ASSERT(radius > 0.0f && height > 0.0f, "Invalid arguments for cylinder.");

	MeshPX mesh;


	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6 + slices * 3 * 2;

	const float offset = radius * -0.5f;

	const float thetaStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);




	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);




	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float t = c * thetaStep;
			const float x = -sin(t) * radius + offset;
			const float y = r * height / rows;
			const float z = cos(t) * radius + offset;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},{u,v} });
		}

	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	for (uint32_t c = 0; c < slices - 1; ++c)
	{
		mesh.indices.push_back(0);
		mesh.indices.push_back(c + 1);
		mesh.indices.push_back(c + 2);


		mesh.indices.push_back(c + (rows - 1) * columns + 2);
		mesh.indices.push_back(c + (rows - 1) * columns + 1);
		mesh.indices.push_back((rows - 1) * columns);



	}
	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(uint32_t slices, uint32_t rings, float radius)
{
	ASSERT(slices >= 3 && rings >= 2, "Invalid arguments for cylinder.");
	ASSERT(radius > 0.0f, "Invalid arguments for cylinder.");

	MeshPX mesh;

	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;

	const float thetaStep = Math::Constants::TwoPi / slices;
	const float phiStep = Math::Constants::Pi / (rings - 1);
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float t = c * thetaStep;
			const float p = r * phiStep;
			const float heightPercent = static_cast<float>(r) / (rows - 1);
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius;
			const float y = -cos(p) * radius;
			const float z = cos(t) * discRadius;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},{u,v} });
		}

	}
	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPX MeshBuilder::CreateConePX(uint32_t slices, uint32_t rings, float radius, float height)
{
	ASSERT(slices >= 3 && rings >= 2, "Invalid arguments for cylinder.");
	ASSERT(radius > 0.0f && height > 0.0f, "Invalid arguments for cylinder.");

	MeshPX mesh;

	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6 + slices * 3 * 2;

	const float offset = radius * -0.5f;

	const float thetaStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float t = c * thetaStep;
			const float heightPercent = static_cast<float>(r) / (rows - 1);
			const float x = -sin(t) * radius * (1 - heightPercent) + offset;
			const float y = r * height / rows;
			const float z = cos(t) * radius * (1 - heightPercent) + offset;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},{u,v} });
		}
	}
	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	for (uint32_t c = 0; c < slices - 1; ++c)
	{
		mesh.indices.push_back(0);
		mesh.indices.push_back(c + 1);
		mesh.indices.push_back(c + 2);

	}
	return mesh;
}

MeshPX MeshBuilder::CreateCirclePlanePX(uint32_t slices, float radius)
{
	ASSERT(slices >= 3, "Invalid arguments for cylinder.");
	ASSERT(radius > 0.0f, "Invalid arguments for cylinder.");

	MeshPX mesh;

	const size_t vertexCount = slices + 1;
	const size_t indexCount = slices * 3;

	const float offset = radius * -0.5f;

	const float thetaStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / slices;

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t c = 0; c < slices; ++c)
	{
		const float t = c * thetaStep;
		const float x = sin(t) * radius + offset;
		const float y = 0.0f;
		const float z = cos(t) * radius + offset;
		const float u = c * uStep;
		const float v = 1.0f;

		mesh.vertices.push_back({ {x,y,z},{u,v} });
	}

	for (uint32_t c = 0; c < slices - 1; ++c)
	{
		mesh.indices.push_back(0);
		mesh.indices.push_back(c + 0);
		mesh.indices.push_back(c + 1);
	}
	return mesh;
}

MeshPX MeshBuilder::CreateCubePX()
{
	MeshPX mesh;
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, 1.0f },   Vector2{1.0f,0.334f} });    //0
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, -1.0f },  Vector2{0.74f,0.334f} });   //1
	mesh.vertices.push_back({ Vector3{  1.0f,  -1.0f, 1.0f },  Vector2{1.0f,0.65f} });    //2
	mesh.vertices.push_back({ Vector3{  -1.0f,  1.0f, 1.0f },  Vector2{0.25f,0.334f} });   //3
	mesh.vertices.push_back({ Vector3{  1.0f,  -1.0f, -1.0f }, Vector2{0.74f,0.65f} });   //4
	mesh.vertices.push_back({ Vector3{  -1.0f,  1.0f, -1.0f }, Vector2{0.5f,0.334f} });    //5
	mesh.vertices.push_back({ Vector3{  -1.0f,  -1.0f, 1.0f }, Vector2{0.25f,0.665f} });   //6
	mesh.vertices.push_back({ Vector3{  -1.0f,  -1.0f, -1.0f },Vector2{0.5f,0.665f} });    //7
	mesh.vertices.push_back({ Vector3{  -1.0f,  1.0f, 1.0f },Vector2{0.5f,0.0f} });        //8
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, 1.0f },Vector2{0.74f,0.0f} });        //9
	mesh.vertices.push_back({ Vector3{  -1.0f,  -1.0f, 1.0f },Vector2{0.5f,1.0f} });       //10
	mesh.vertices.push_back({ Vector3{  1.0f,  -1.0f, 1.0f },Vector2{0.74f,1.0f} });       //11
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, 1.0f },Vector2{0.0f,0.334f} });       //12
	mesh.vertices.push_back({ Vector3{  1.0f,  -1.0f, 1.0f },Vector2{0.0f,0.665f} });      //13

	//front
	mesh.indices.push_back(5);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);

	mesh.indices.push_back(1);
	mesh.indices.push_back(4);
	mesh.indices.push_back(5);

	//right
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(4);

	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(1);

	//back
	mesh.indices.push_back(12);
	mesh.indices.push_back(6);
	mesh.indices.push_back(13);

	mesh.indices.push_back(3);
	mesh.indices.push_back(6);
	mesh.indices.push_back(12);

	//left
	mesh.indices.push_back(3);
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);

	mesh.indices.push_back(5);
	mesh.indices.push_back(7);
	mesh.indices.push_back(3);
	//up
	mesh.indices.push_back(8);
	mesh.indices.push_back(1);
	mesh.indices.push_back(5);

	mesh.indices.push_back(9);
	mesh.indices.push_back(1);
	mesh.indices.push_back(8);
	//down
	mesh.indices.push_back(7);
	mesh.indices.push_back(11);
	mesh.indices.push_back(10);

	mesh.indices.push_back(4);
	mesh.indices.push_back(11);
	mesh.indices.push_back(7);

	return mesh;
}

MeshPX MeshBuilder::CreateSkyBoxPX()
{
	MeshPX mesh;
	mesh.vertices.push_back({ Vector3{  10.0f,  10.0f, 10.0f },   Vector2{1.0f,0.334f} });       //0
	mesh.vertices.push_back({ Vector3{  10.0f,  10.0f, -10.0f },  Vector2{0.745f,0.334f} });      //1
	mesh.vertices.push_back({ Vector3{  10.0f,  -10.0f, 10.0f },  Vector2{1.0f,0.665f} });       //2
	mesh.vertices.push_back({ Vector3{  -10.0f,  10.0f, 10.0f },  Vector2{0.25f,0.334f} });      //3
	mesh.vertices.push_back({ Vector3{  10.0f,  -10.0f, -10.0f }, Vector2{0.745f,0.665f} });      //4
	mesh.vertices.push_back({ Vector3{  -10.0f,  10.0f, -10.0f }, Vector2{0.5f,0.334f} });       //5
	mesh.vertices.push_back({ Vector3{  -10.0f,  -10.0f, 10.0f }, Vector2{0.25f,0.665f} });      //6
	mesh.vertices.push_back({ Vector3{  -10.0f,  -10.0f, -10.0f },Vector2{0.5f,0.665f} });       //7
	mesh.vertices.push_back({ Vector3{  -10.0f,  10.0f, 10.0f },Vector2{0.5f,0.0f} });         //8
	mesh.vertices.push_back({ Vector3{  10.0f,  10.0f, 10.0f },Vector2{0.75f,0.0f} });          //9
	mesh.vertices.push_back({ Vector3{  -10.0f,  -10.0f, 10.0f },Vector2{0.5f,1.0f} });         //10
	mesh.vertices.push_back({ Vector3{  10.0f,  -10.0f, 10.0f },Vector2{0.75f,1.0f} });         //11
	mesh.vertices.push_back({ Vector3{  10.0f,  10.0f, 10.0f },Vector2{0.0f,0.334f} });         //12
	mesh.vertices.push_back({ Vector3{  10.0f,  -10.0f, 10.0f },Vector2{0.0f,0.665f} });         //13

	//front
	mesh.indices.push_back(4);
	mesh.indices.push_back(5);
	mesh.indices.push_back(7);

	mesh.indices.push_back(1);
	mesh.indices.push_back(5);
	mesh.indices.push_back(4);

	//right
	mesh.indices.push_back(2);
	mesh.indices.push_back(1);
	mesh.indices.push_back(4);

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);

	//back
	mesh.indices.push_back(6);
	mesh.indices.push_back(12);
	mesh.indices.push_back(13);

	mesh.indices.push_back(6);
	mesh.indices.push_back(3);
	mesh.indices.push_back(12);

	//left
	mesh.indices.push_back(7);
	mesh.indices.push_back(3);
	mesh.indices.push_back(6);

	mesh.indices.push_back(7);
	mesh.indices.push_back(5);
	mesh.indices.push_back(3);
	//up
	mesh.indices.push_back(1);
	mesh.indices.push_back(8);
	mesh.indices.push_back(5);

	mesh.indices.push_back(1);
	mesh.indices.push_back(9);
	mesh.indices.push_back(8);
	//down
	mesh.indices.push_back(11);
	mesh.indices.push_back(7);
	mesh.indices.push_back(10);

	mesh.indices.push_back(11);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);

	return mesh;
}

MeshPX Storm::Graphics::MeshBuilder::CreateSkyDome(uint32_t slices, uint32_t rings, float radius)
{
	ASSERT(slices >= 3 && rings >= 2, "Invalid arguments for cylinder.");
	ASSERT(radius > 0.0f, "Invalid arguments for cylinder.");

	MeshPX mesh;

	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6 + slices * 3 * 2;

	const float offset = radius * -0.5f;

	const float thetaStep = Math::Constants::TwoPi / slices;
	const float phiStep = Math::Constants::Pi / (rings - 1);
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);




	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);




	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float t = c * thetaStep;
			const float p = r * phiStep;
			const float heightPercent = static_cast<float>(r / (rows - 1));
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius + offset;
			const float y = -cos(p) * radius;
			const float z = cos(t) * discRadius + offset;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},{u,v} });
		}

	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
		}
	}

	return mesh;
}

Mesh MeshBuilder::CreatePlane(uint32_t columns, uint32_t rows, float spacing, float tiling)
{
	ASSERT(columns >= 2 && rows >= 2, "Invalid arguments for plane.");
	ASSERT(spacing > 0.0f, "Invalid arguments for plane.");
	Mesh mesh;

	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;
	const float width = (columns - 1) * spacing;
	const float length = (rows - 1) * spacing;
	const float offsetX = width * -0.5f;
	const float offsetZ = length * -0.5f;
	const float uStep = tiling / (columns - 1);
	const float vStep = tiling / (rows - 1);

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float x = (c * spacing) + offsetX;
			const float y = 0.0f;
			const float z = (r * spacing) + offsetZ;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back(
				{ {x,y,z},
				Math::Vector3::YAxis,
				Math::Vector3::XAxis,
				{u,v} }
			);
		}
	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;

}

Mesh MeshBuilder::CreateSphere(uint32_t slices, uint32_t rings, float radius)
{
	ASSERT(slices >= 3 && rings >= 2, "Invalid arguments for cylinder.");
	ASSERT(radius > 0.0f, "Invalid arguments for cylinder.");

	Mesh mesh;


	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;

	const float thetaStep = Math::Constants::TwoPi / slices;
	const float phiStep = Math::Constants::Pi / (rings - 1);
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);




	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);




	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float t = c * thetaStep;
			const float p = r * phiStep;
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius;
			const float y = -cos(p) * radius;
			const float z = cos(t) * discRadius;
			const Math::Vector3 normal = Math::Normalize(Vector3{ x,y,z });
			const Math::Vector3 tangent = Math::Normalize(Vector3{ -normal.z,0.0f,normal.x });
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},normal,tangent, {u,v} });

		}

	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPC MeshBuilder::CreateParticle(float radius, Color color)
{
	ASSERT(radius > 0.0f, "Invalid arguments for cylinder.");
	uint32_t slices = 128;
	uint32_t rings = 128;

	MeshPC mesh;


	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;


	const float thetaStep = Math::Constants::TwoPi / slices;
	const float phiStep = Math::Constants::Pi / (rings - 1);
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);




	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);




	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float t = c * thetaStep;
			const float p = r * phiStep;
			const float heightPercent = static_cast<float>(r) / (rows - 1);
			const float discRadius = radius * sin(p);
			const float x = -sin(t) * discRadius;
			const float y = -cos(p) * radius;
			const float z = cos(t) * discRadius;
			mesh.vertices.push_back({ {x,y,z},color });
		}

	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}
MeshPCX MeshBuilder::CreateParticle2D(float size, Color color)
{
	ASSERT(size >= 0, "Invalid arguments for plane.");
	MeshPCX mesh;

	const uint32_t vertexCount = 4;
	const uint32_t indexCount = 6;
	const float width = size;
	const float length = size;
	const float offsetX = width * -0.5f;
	const float offsetZ = length * -0.5f;
	const float uStep = 1.0f;
	const float vStep = 1.0f;

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t r = 0; r < 2; ++r)
	{
		for (uint32_t c = 0; c < 2; ++c)
		{
			const float x = (c * size) + offsetX;
			const float y = 0.0f;
			const float z = (r * size) + offsetZ;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);

			mesh.vertices.push_back({ {x,y,z},color,{u,v} });
		}
	}


	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);

	mesh.indices.push_back(0);
	mesh.indices.push_back(3);
	mesh.indices.push_back(1);

	return mesh;



}
