#pragma once

#include "VertexTypes.h "
namespace Storm::Graphics
{

	template<class VertexT>
	struct MeshBase
	{
		using VertexType = VertexT;
		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;


	};

	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using MeshPCX = MeshBase<VertexPCX>;
	using Mesh = MeshBase<Vertex>;
}