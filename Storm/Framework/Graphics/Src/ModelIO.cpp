#include "Precompiled.h"
#include "ModelIO.h"
#include "Model.h"
#include "AnimationIO.h"
using namespace Storm;
using namespace Storm::Graphics;

//-scale 0.1 ../../Assets/Models/Jumping.fbx ../../Assets/Models/Jumping.model

//-scale 0.1 ../../Assets/Models/Fighter/Fighter.fbx ../../Assets/Models/Fighter/Fighter.model

void ModelIO::SaveModel(std::filesystem::path filePath, const Model& model)
{
	if (model.meshData.empty())
	{
		return;
	}

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %u\n", meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{

		const auto& meshData = model.meshData[i];
		fprintf_s(file, "MaterialIndex: %u\n", meshData.materialIndex);

		const auto& mesh = meshData.mesh;
		const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
		fprintf_s(file, "VertexCount: %u\n", vertexCount);
		for (auto& vertex : mesh.vertices)
		{
			fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
				vertex.position.x,
				vertex.position.y,
				vertex.position.z,
				vertex.normal.x,
				vertex.normal.y,
				vertex.normal.z,
				vertex.tangent.x,
				vertex.tangent.y,
				vertex.tangent.z,
				vertex.textCoord.x,
				vertex.textCoord.y,
				vertex.boneIndices[0], vertex.boneIndices[1], vertex.boneIndices[2], vertex.boneIndices[3],
				vertex.boneWeight[0], vertex.boneWeight[1], vertex.boneWeight[2], vertex.boneWeight[3]);
		}

		const uint32_t indicesCount = static_cast<uint32_t>(mesh.indices.size());
		fprintf_s(file, "IndexCount: %u\n", indicesCount);
		for (uint32_t j = 0; j < indicesCount; j += 3)
		{
			fprintf_s(file, "%u %u %u\n",
				mesh.indices[j], mesh.indices[j + 1], mesh.indices[j + 2]);
		}

	}
	fclose(file);
}

void ModelIO::LoadModel(std::filesystem::path filePath, Model& model)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t meshCount = 0;
	fscanf_s(file, "MeshCount: %u\n", &meshCount);

	model.meshData.resize(meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		auto& meshData = model.meshData[i];
		fscanf_s(file, "MaterialIndex: %u\n", &meshData.materialIndex);

		auto& mesh = meshData.mesh;
		uint32_t vertexCount = 0;
		fscanf_s(file, "VertexCount: %u\n", &vertexCount);
		model.meshData[i].mesh.vertices.resize(vertexCount);
		for (uint32_t j = 0; j < vertexCount; ++j)
		{
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
				&mesh.vertices[j].position.x,
				&mesh.vertices[j].position.y,
				&mesh.vertices[j].position.z,
				&mesh.vertices[j].normal.x,
				&mesh.vertices[j].normal.y,
				&mesh.vertices[j].normal.z,
				&mesh.vertices[j].tangent.x,
				&mesh.vertices[j].tangent.y,
				&mesh.vertices[j].tangent.z,
				&mesh.vertices[j].textCoord.x,
				&mesh.vertices[j].textCoord.y,
				&mesh.vertices[j].boneIndices[0], &mesh.vertices[j].boneIndices[1], &mesh.vertices[j].boneIndices[2], &mesh.vertices[j].boneIndices[3],
				&mesh.vertices[j].boneWeight[0], &mesh.vertices[j].boneWeight[1], &mesh.vertices[j].boneWeight[2], &mesh.vertices[j].boneWeight[3]);
		}

		uint32_t indicesCount = 0;
		fscanf_s(file, "IndexCount: %u\n", &indicesCount);
		model.meshData[i].mesh.indices.resize(indicesCount);
		for (uint32_t x = 0; x < indicesCount; x += 3)
		{
			fscanf_s(file, "%u %u %u\n",
				&mesh.indices[x], &mesh.indices[x + 1], &mesh.indices[x + 2]);
		}



	}
	fclose(file);
}

void ModelIO::SaveMaterial(std::filesystem::path filePath, const Model& model)
{
	if (model.materialData.empty())
	{
		return;
	}
	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MaterialCount: %u\n", materialCount);
	for (uint32_t i = 0; i < materialCount; ++i)
	{
		const auto& materialData = model.materialData[i];
		const auto& materialInfo = materialData.material;
		fprintf_s(file, "MaterialAmbient: %f %f %f %f\n", materialInfo.ambient.r, materialInfo.ambient.g, materialInfo.ambient.b, materialInfo.ambient.a);
		fprintf_s(file, "MaterialDiffuse: %f %f %f %f\n", materialInfo.diffuse.r, materialInfo.diffuse.g, materialInfo.diffuse.b, materialInfo.diffuse.a);
		fprintf_s(file, "MaterialSpecular: %f %f %f %f\n", materialInfo.specular.r, materialInfo.specular.g, materialInfo.specular.b, materialInfo.specular.a);
		fprintf_s(file, "MaterialPower: %f\n", materialData.material.power);

		fprintf_s(file, "%s\n", materialData.diffuseMapName.empty() ? "<none>" : materialData.diffuseMapName.c_str());
		fprintf_s(file, "%s\n", materialData.displacementMapName.empty() ? "<none>" : materialData.displacementMapName.c_str());
		fprintf_s(file, "%s\n", materialData.normalMapName.empty() ? "<none>" : materialData.normalMapName.c_str());
		fprintf_s(file, "%s\n", materialData.specularMapName.empty() ? "<none>" : materialData.specularMapName.c_str());
	}
	fclose(file);

}

void ModelIO::LoadMaterial(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("material");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t materialCount = 0;
	fscanf_s(file, "MaterialCount: %u\n", &materialCount);

	auto TryReadTextureName = [&](auto& destName)
	{
		char textureName[1024]{};
		if (fscanf_s(file, "%s\n", textureName, (uint32_t)sizeof(textureName)) && strcmp(textureName, "<none>") != 0)
		{
			destName = filePath.replace_filename(textureName).u8string().c_str();
		}

	};

	model.materialData.resize(materialCount);
	for (uint32_t i = 0; i < materialCount; ++i)
	{
		auto& materialData = model.materialData[i];
		auto& materialInfo = materialData.material;
		fscanf_s(file, "MaterialAmbient: %f %f %f %f\n", &materialInfo.ambient.r, &materialInfo.ambient.g, &materialInfo.ambient.b, &materialInfo.ambient.a);
		fscanf_s(file, "MaterialDiffuse: %f %f %f %f\n", &materialInfo.diffuse.r, &materialInfo.diffuse.g, &materialInfo.diffuse.b, &materialInfo.diffuse.a);
		fscanf_s(file, "MaterialSpecular: %f %f %f %f\n", &materialInfo.specular.r, &materialInfo.specular.g, &materialInfo.specular.b, &materialInfo.specular.a);
		fscanf_s(file, "MaterialPower: %f\n", &materialData.material.power);

		//auto ReadTextureName = [&](const char* fmt, std::string& name)
		//{
		//	const uint32_t bufferSize = 256;
		//	char buffer[bufferSize];

		//	fscanf_s(file, fmt, buffer, bufferSize);
		//	if (strcmp(buffer, "<none>") != 0)
		//		name = buffer;
		//};

		TryReadTextureName(materialData.diffuseMapName);
		TryReadTextureName(materialData.displacementMapName);
		TryReadTextureName(materialData.normalMapName);
		TryReadTextureName(materialData.specularMapName);
		//ReadTextureName("%s\n", materialData.diffuseMapName);
		//ReadTextureName("%s\n", materialData.displacementMapName);
		//ReadTextureName("%s\n", materialData.normalMapName);
		//ReadTextureName("%s\n", materialData.specularMapName);
	}
	fclose(file);

}

void ModelIO::SaveSkeleton(std::filesystem::path filePath, const Model& model)
{
	if (!model.skeleton)
		return;
	filePath.replace_extension("skeleton");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t bonesCount = static_cast<uint32_t>(model.skeleton->bones.size());
	fprintf_s(file, "BonesCount: %u\n", bonesCount);

	for (uint32_t i = 0; i < bonesCount; ++i)
	{
		const auto& boneData = model.skeleton->bones[i];
		fprintf_s(file, "BoneName: %s\n", boneData->name.empty() ? "<none>" : boneData->name.c_str());

		fprintf_s(file, "BoneIndex: %d\n", boneData->index);
		fprintf_s(file, "BoneParentIndex: %d\n", boneData->parentIndex);

		const uint32_t ChildIndicesCount = static_cast<uint32_t>(boneData->childIndices.size());
		fprintf_s(file, "ChildIndicesCount: %u\n", ChildIndicesCount);

		for (uint32_t i = 0; i < ChildIndicesCount; ++i)
		{
			fprintf_s(file, "%d\n", boneData->childIndices[i]);
		}

		fprintf_s(file, "%f %f %f %f\n", boneData->toParentTransform._11, boneData->toParentTransform._12, boneData->toParentTransform._13, boneData->toParentTransform._14);
		fprintf_s(file, "%f %f %f %f\n", boneData->toParentTransform._21, boneData->toParentTransform._22, boneData->toParentTransform._23, boneData->toParentTransform._24);
		fprintf_s(file, "%f %f %f %f\n", boneData->toParentTransform._31, boneData->toParentTransform._32, boneData->toParentTransform._33, boneData->toParentTransform._34);
		fprintf_s(file, "%f %f %f %f\n", boneData->toParentTransform._41, boneData->toParentTransform._42, boneData->toParentTransform._43, boneData->toParentTransform._44);

		fprintf_s(file, "%f %f %f %f\n", boneData->offsetTransform._11, boneData->offsetTransform._12, boneData->offsetTransform._13, boneData->offsetTransform._14);
		fprintf_s(file, "%f %f %f %f\n", boneData->offsetTransform._21, boneData->offsetTransform._22, boneData->offsetTransform._23, boneData->offsetTransform._24);
		fprintf_s(file, "%f %f %f %f\n", boneData->offsetTransform._31, boneData->offsetTransform._32, boneData->offsetTransform._33, boneData->offsetTransform._34);
		fprintf_s(file, "%f %f %f %f\n", boneData->offsetTransform._41, boneData->offsetTransform._42, boneData->offsetTransform._43, boneData->offsetTransform._44);

	}
	fclose(file);

}

void ModelIO::LoadSkeleton(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("skeleton");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t bonesCount = 0;
	fscanf_s(file, "BonesCount: %u\n", &bonesCount);

	model.skeleton = std::make_unique<Skeleton>();
	model.skeleton->bones.reserve(bonesCount);
	auto ReadBoneName = [&](const char* fmt, std::string& name)
	{
		const uint32_t bufferSize = 256;
		char buffer[bufferSize];

		fscanf_s(file, fmt, buffer, bufferSize);
		if (strcmp(buffer, "<none>") != 0)
			name = buffer;
	};
	for (uint32_t i = 0; i < bonesCount; ++i)
	{
		auto& boneData = model.skeleton->bones.emplace_back(std::make_unique<Bone>());

		ReadBoneName("BoneName: %s\n", boneData->name);

		fscanf_s(file, "BoneIndex: %d\n", &boneData->index);
		fscanf_s(file, "BoneParentIndex: %d\n", &boneData->parentIndex);

		uint32_t ChildIndicesCount = 0;
		fscanf_s(file, "ChildIndicesCount: %u\n", &ChildIndicesCount);
		boneData->childIndices.resize(ChildIndicesCount);

		for (uint32_t i = 0; i < ChildIndicesCount; ++i)
		{
			fscanf_s(file, "%d\n", &boneData->childIndices[i]);
		}

		fscanf_s(file, "%f %f %f %f\n", &boneData->toParentTransform._11, &boneData->toParentTransform._12, &boneData->toParentTransform._13, &boneData->toParentTransform._14);
		fscanf_s(file, "%f %f %f %f\n", &boneData->toParentTransform._21, &boneData->toParentTransform._22, &boneData->toParentTransform._23, &boneData->toParentTransform._24);
		fscanf_s(file, "%f %f %f %f\n", &boneData->toParentTransform._31, &boneData->toParentTransform._32, &boneData->toParentTransform._33, &boneData->toParentTransform._34);
		fscanf_s(file, "%f %f %f %f\n", &boneData->toParentTransform._41, &boneData->toParentTransform._42, &boneData->toParentTransform._43, &boneData->toParentTransform._44);

		fscanf_s(file, "%f %f %f %f\n", &boneData->offsetTransform._11, &boneData->offsetTransform._12, &boneData->offsetTransform._13, &boneData->offsetTransform._14);
		fscanf_s(file, "%f %f %f %f\n", &boneData->offsetTransform._21, &boneData->offsetTransform._22, &boneData->offsetTransform._23, &boneData->offsetTransform._24);
		fscanf_s(file, "%f %f %f %f\n", &boneData->offsetTransform._31, &boneData->offsetTransform._32, &boneData->offsetTransform._33, &boneData->offsetTransform._34);
		fscanf_s(file, "%f %f %f %f\n", &boneData->offsetTransform._41, &boneData->offsetTransform._42, &boneData->offsetTransform._43, &boneData->offsetTransform._44);


	}
	fclose(file);

	for (uint32_t b = 0; b < bonesCount; ++b)
	{
		auto& BonesData = model.skeleton->bones;
		Bone* bone = BonesData[b].get();
		if (bone->parentIndex == -1)
		{
			model.skeleton->root = bone;
		}
		else
		{
			bone->parent = BonesData[bone->parentIndex].get();
		}

		if (!bone->childIndices.empty())
		{
			const auto childNum = bone->childIndices.size();
			bone->children.resize(childNum);

			for (uint32_t i = 0; i < childNum; ++i)
			{
				bone->children[i] = BonesData[bone->childIndices[i]].get();
			}

		}

	}

}

void ModelIO::SaveAnimationSet(std::filesystem::path filePath, const Model& model)
{
	if (model.animationSet.empty())
	{
		return;
	}
	filePath.replace_extension("animation");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;


	const uint32_t animationSetCount = static_cast<uint32_t>(model.animationSet.size());
	fprintf_s(file, "AnimationSetCount: %u\n", animationSetCount);
	for (uint32_t i = 0; i < animationSetCount; ++i)
	{
		const auto& animationData = model.animationSet[i];

		fprintf_s(file, "%s\n", animationData.name.empty() ? "<none>" : animationData.name.c_str());
		fprintf_s(file, "tickDuration: %f\n", animationData.tickDuration);
		fprintf_s(file, "ticksPerSecond: %f\n", animationData.ticksPerSecond);

		const uint32_t animationClipCount = static_cast<uint32_t>(animationData.boneAnimations.size());
		fprintf_s(file, "AnimationClipCount: %u\n", animationClipCount);
		if (!animationData.boneAnimations.empty())
		{
			for (uint32_t j = 0; j < animationClipCount; ++j)
			{
				const auto& animationclipData = animationData.boneAnimations[j];

				if (animationclipData)
				{
					fprintf_s(file, "[Animation]\n");
					AnimationIO::Write(file, *animationclipData);
				}
				else
				{
					fprintf_s(file, "[Empty]\n");

				}
			}
		}
	}

	fclose(file);
}

void ModelIO::LoadAnimationSet(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("animation");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;


	uint32_t animationSetCount = 0;
	fscanf_s(file, "AnimationSetCount: %u\n", &animationSetCount);
	model.animationSet.reserve(model.animationSet.size() + animationSetCount);
	auto ReadBoneName = [&](const char* fmt, std::string& name)
	{
		const uint32_t bufferSize = 256;
		char buffer[bufferSize];

		fscanf_s(file, fmt, buffer, bufferSize);
		if (strcmp(buffer, "<none>") != 0)
			name = buffer;
	};
	for (uint32_t i = 0; i < animationSetCount; ++i)
	{

		auto& animationData = model.animationSet.emplace_back();

		ReadBoneName("%s\n",animationData.name);

		fscanf_s(file, "tickDuration: %f\n", &animationData.tickDuration);
		fscanf_s(file, "ticksPerSecond: %f\n", &animationData.ticksPerSecond);

		uint32_t animationClipCount = 0;
		fscanf_s(file, "AnimationClipCount: %u\n", &animationClipCount);
		if (animationClipCount > 0)
		{
			animationData.boneAnimations.resize(animationClipCount);
			for (auto& animation : animationData.boneAnimations)
			{
				char AnimationName[1024]{};
				fscanf_s(file, "%s\n", AnimationName, (uint32_t)sizeof(AnimationName));
				if (strcmp(AnimationName, "[Animation]") == 0)
				{
					animation = std::make_unique<Animation>();
					AnimationIO::Read(file, *animation);
				}

			}
		}


	}

	fclose(file);

}
