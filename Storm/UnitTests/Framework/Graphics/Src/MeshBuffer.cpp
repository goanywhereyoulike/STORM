#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"

using namespace Storm;
using namespace Storm::Graphics;

MeshBuffer::~MeshBuffer()
{
	ASSERT(mVertexBuffer == nullptr, "Terminate must be called before destruction");
}
MeshBuffer::MeshBuffer(MeshBuffer&&rhs) noexcept
	:mVertexBuffer(rhs.mVertexBuffer)
	,mIndexBuffer(rhs.mIndexBuffer)
	,mTopology(rhs.mTopology)
	,mVertexSize(rhs.mVertexSize)
	,mVertexCount(rhs.mVertexCount)
	,mIndexCount(rhs.mIndexCount)
{
	rhs.mVertexBuffer = nullptr;
	rhs.mIndexBuffer = nullptr;

}
MeshBuffer& MeshBuffer::operator=(MeshBuffer&&rhs) noexcept
{
	if (this != &rhs)
	{					
		mVertexBuffer = rhs.mVertexBuffer;
		mIndexBuffer = rhs.mIndexBuffer;
		mTopology = rhs.mTopology;
		mVertexSize = rhs.mVertexSize;
		mVertexCount = rhs.mVertexCount;
		mIndexCount = rhs.mIndexCount;
		rhs.mVertexBuffer = nullptr;
		rhs.mIndexBuffer = nullptr;

	}
	return *this;
}
void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, bool dynamic)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount, dynamic);
}

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, bool dynamic)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount, dynamic);
	CreateIndexBuffer(indices, indexCount);
	
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Topology::Triangles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;


	}
}

void MeshBuffer::Update(const void* vertices, uint32_t numVertices)
{
	mVertexCount = numVertices;
	auto context = GraphicsSystem::Get()->GetContext();
	
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (hr != S_OK)
	{
		return;
	}
	memcpy(resource.pData, vertices, numVertices * mVertexSize);

	context->Unmap(mVertexBuffer, 0);


}

void MeshBuffer::Render() const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->IASetPrimitiveTopology(mTopology);
	UINT stride = mVertexSize;
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	if (mIndexBuffer != nullptr)
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		context->Draw(mVertexCount, 0);
	}
	
}

void MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = vertexSize * vertexCount;
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;
	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&bufferDesc, vertices ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
}

void MeshBuffer::CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount)
{
	mIndexCount = indexCount;
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(indexCount * sizeof(uint32_t));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = indices;
	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");
}
