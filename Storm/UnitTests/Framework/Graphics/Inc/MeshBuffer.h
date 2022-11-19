#pragma once

namespace Storm::Graphics
{
	class MeshBuffer
	{
	public:
		enum class Topology { Points, Lines, Triangles };
		MeshBuffer() = default;
		~MeshBuffer();
		MeshBuffer(MeshBuffer&&) noexcept;
		MeshBuffer& operator=(MeshBuffer&&) noexcept;

		template <class MeshType>
		void Initialize(const MeshType& mesh, bool dynamic = false)
		{
			if (!mesh.indices.empty())
			{
				Initialize(
					mesh.vertices.data(),
					static_cast<uint32_t>(sizeof(MeshType::VertexType)),
					static_cast<uint32_t>(mesh.vertices.size()),
					mesh.indices.data(),
					static_cast<uint32_t>(mesh.indices.size()),dynamic);
			}
			else
			{
				Initialize(
					mesh.vertices.data(),
					static_cast<uint32_t>(sizeof(MeshType::VertexType)),
					static_cast<uint32_t>(mesh.vertices.size()),dynamic);
			}

		}


		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, bool dynamic = false);
		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, bool dynamic = false);
		void Terminate();

		void SetTopology(Topology topology);
		void Update(const void* vertices, uint32_t numVertices);
		void Render() const;
	private:
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;

		void CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, bool dynamic);
		void CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount);

		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		uint32_t mVertexSize = 0;
		uint32_t mVertexCount = 0;
		uint32_t mIndexCount = 0;
	};
}