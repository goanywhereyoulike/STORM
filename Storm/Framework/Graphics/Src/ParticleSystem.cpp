#include "Precompiled.h"
#include "ParticleSystem.h"
#include "VertexTypes.h"
#include "MeshBuilder.h"
#include <RapidJson/document.h>
#include <RapidJson/filereadstream.h>
#include <RapidJson/filewritestream.h>
#include <RapidJson/prettywriter.h>

using namespace Storm;
using namespace Storm::Graphics;
namespace rj = rapidjson;

void Storm::Graphics::ParticleSystem::Initialize()
{
	const size_t poolSize = 5000;
	poolIndex = 0;

	auto tm = TextureManager::Get();
	particle.diffuseMapId = tm->LoadTexture("../../Assets/Images/fire.png");
	particle.Initialize();
	particlepool.resize(poolSize, particle);
	//mSimpleEffect.Initialize();
	mParticleTexturingEffect.Initialize();
}

void Storm::Graphics::ParticleSystem::Render()
{
	mParticleTexturingEffect.SetCamera(*mCamera);

	std::vector<int> particleIndices;
	particleIndices.reserve(particlepool.size());

	//mSimpleEffect.SetCamera(*mCamera);
	for (int i = 0; i < particlepool.size(); ++i)
	{
		auto& particle = particlepool[i];
		if (particle.IsActive)
		{
			particleIndices.push_back(i);
		}
	}

	const auto& viewPosition = mCamera->GetPosition();
	const auto& viewDirection = mCamera->GetDirection();

	std::sort(particleIndices.begin(), particleIndices.end(), [&](int a, int b) {
		const float distA = Math::Dot(particlepool[a].transform.position - viewPosition, viewDirection);
		const float distB = Math::Dot(particlepool[b].transform.position - viewPosition, viewDirection);
		return distA > distB;
		});


	mParticleTexturingEffect.Begin();
	for (int i = 0; i < particleIndices.size(); ++i)
	{
		auto& particle = particlepool[particleIndices[i]];
		mParticleTexturingEffect.RenderParticle(particle);
	}
	mParticleTexturingEffect.End();
}

void Storm::Graphics::ParticleSystem::Terminate()
{
	//mSimpleEffect.Terminate();
	mParticleTexturingEffect.Terminate();
}


void Storm::Graphics::ParticleSystem::Emit(ParticleProps& particlepros)
{
	Particle& particle = particlepool[poolIndex];
	particle.IsActive = true;
	particle.transform.position = particlepros.position;
	particle.transform.position.x += Math::Random::UniformFloat(-particlepros.mEmitPosOffset.x, particlepros.mEmitPosOffset.x);
	particle.transform.position.y += Math::Random::UniformFloat(-particlepros.mEmitPosOffset.y, particlepros.mEmitPosOffset.y);
	particle.transform.position.z += Math::Random::UniformFloat(-particlepros.mEmitPosOffset.z, particlepros.mEmitPosOffset.z);
	//particle.transform.rotation = Math::Quaternion::RotationEuler(Math::Vector3{ 0, Math::Random::UniformFloat(0.0f,2.0f) * Math::Constants::HalfPi,0 });
	//	particle.useAdditiveBlend = true;
	particle.velocity.x = particlepros.velocity.x * particlepros.velocityvariation.x;
	particle.velocity.y = particlepros.velocity.y * particlepros.velocityvariation.y;
	particle.velocity.z = particlepros.velocity.z * particlepros.velocityvariation.z;
	//particle.velocity += Storm::Math::Random::OnUnitSphere();
	//particle.velocity.z += particlepros.velocityvariation.z * (generator() % 3 - 0.5f);

	particle.lifeTime = particlepros.lifetime;
	particle.lifeRemain = particlepros.lifetime;
	particle.color = particlepros.color;
	particle.startColor = particlepros.startColor;
	particle.endColor = particlepros.endColor;
	//particle.color.a = particle.color.a * life;

	particle.size = particlepros.size;
	particlepool.resize(particlepros.number, particle);
	poolIndex = (++poolIndex) % particlepool.size();
}

void Storm::Graphics::ParticleSystem::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void Storm::Graphics::ParticleSystem::UpdateParticles(float deltaTime)
{
	for (auto& particle : particlepool)
	{
		if (!particle.IsActive)
		{
			continue;
		}
		if (particle.lifeRemain <= 0.0f)
		{
			particle.IsActive = false;
			continue;
		}

		particle.lifeRemain -= deltaTime;
		particle.transform.position += particle.velocity * deltaTime;
		//particle.transform.rotation = Math::Quaternion::RotationLook(Math::Normalize(mCamera->GetDirection())) * Math::Quaternion::RotationAxis(Math::Vector3::YAxis, 135.0f);// +Math::Quaternion::RotationEuler(Math::Vector3{ 0,0, Math::Constants::HalfPi });
		//particle.meshBuffer.Update();

	}
}

const Storm::Graphics::Texture* Storm::Graphics::ParticleSystem::GetParticleTexture()
{
	auto tm = TextureManager::Get();
	if (particle.diffuseMapId != 0)
	{
		auto texture = tm->GetTexture(particle.diffuseMapId);

		return texture;
	}

	return nullptr;
}

void Storm::Graphics::ParticleSystem::SaveParticleProps(std::filesystem::path filePath, ParticleProps ParticleProp)
{
	FILE* file;
	fopen_s(&file, filePath.u8string().c_str(), "w+");
	char writeBuffer[65536];
	rj::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
	rj::PrettyWriter<rj::FileWriteStream> writer(os);
	writer.StartObject();
	writer.Key("Properties");
	writer.StartObject();

	writer.Key("Emitter_Properties");
	writer.StartObject();

	writer.Key("EmitPosOffset");
	writer.StartArray();
	writer.Double(ParticleProp.mEmitPosOffset.x); writer.Double(ParticleProp.mEmitPosOffset.y); writer.Double(ParticleProp.mEmitPosOffset.z);
	writer.EndArray();

	writer.Key("ParticleNumber");
	writer.Int(ParticleProp.number);

	writer.Key("EmitterPosition");
	writer.StartArray();
	writer.Double(ParticleProp.position.x); writer.Double(ParticleProp.position.y); writer.Double(ParticleProp.position.z);
	writer.EndArray();
	writer.EndObject();

	writer.Key("Particle_Properties");
	writer.StartObject();
	writer.Key("lifetime");
	writer.Double(ParticleProp.lifetime);

	writer.Key("size");
	writer.Double(ParticleProp.size);

	writer.Key("velocity");
	writer.StartArray();
	writer.Double(ParticleProp.velocity.x); writer.Double(ParticleProp.velocity.y); writer.Double(ParticleProp.velocity.z);
	writer.EndArray();

	writer.Key("velocityvariation");
	writer.StartArray();
	writer.Double(ParticleProp.velocityvariation.x); writer.Double(ParticleProp.velocityvariation.y); writer.Double(ParticleProp.velocityvariation.z);
	writer.EndArray();

	writer.Key("startColor");
	writer.StartArray();
	writer.Double(ParticleProp.startColor.a); writer.Double(ParticleProp.startColor.r); writer.Double(ParticleProp.startColor.g); writer.Double(ParticleProp.startColor.b);
	writer.EndArray();

	writer.Key("endColor");
	writer.StartArray();
	writer.Double(ParticleProp.endColor.a); writer.Double(ParticleProp.endColor.r); writer.Double(ParticleProp.endColor.g); writer.Double(ParticleProp.endColor.b);
	writer.EndArray();
	writer.EndObject();

	writer.EndObject();
	writer.EndObject();

	fclose(file);
}
void Storm::Graphics::ParticleSystem::LoadParticleProps(std::filesystem::path filePath, ParticleProps& ParticleProp)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, filePath.u8string().c_str(), "r");
	ASSERT(err == 0 || file != nullptr, "ParticleSystem -- Failed to load particle properties '%s'", filePath.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

	fclose(file);

	rj::Document document;
	document.ParseStream(readStream);

	auto components = document["Properties"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (strcmp(componentName, "Emitter_Properties") == 0)
		{
			if (component.value.HasMember("EmitPosOffset"))
			{
				const auto& offsets = component.value["EmitPosOffset"].GetArray();
				const float x = offsets[0].GetFloat();
				const float y = offsets[1].GetFloat();
				const float z = offsets[2].GetFloat();
				ParticleProp.mEmitPosOffset = Math::Vector3(x, y, z);
			}
			if (component.value.HasMember("ParticleNumber"))
			{
				const auto& number = component.value["ParticleNumber"].GetFloat();
				ParticleProp.number = number;
			}
			if (component.value.HasMember("EmitterPosition"))
			{
				const auto& position = component.value["EmitterPosition"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				ParticleProp.position = Math::Vector3(x, y, z);
			}
		}
		if (strcmp(componentName, "Particle_Properties") == 0)
		{
			if (component.value.HasMember("lifetime"))
			{
				const auto& lifetime = component.value["lifetime"].GetFloat();
				ParticleProp.lifetime = lifetime;
			}
			if (component.value.HasMember("size"))
			{
				const auto& size = component.value["size"].GetFloat();
				ParticleProp.size = size;
			}
			if (component.value.HasMember("velocity"))
			{
				const auto& velocity = component.value["velocity"].GetArray();
				const float x = velocity[0].GetFloat();
				const float y = velocity[1].GetFloat();
				const float z = velocity[2].GetFloat();
				ParticleProp.velocity = Math::Vector3(x, y, z);
			}
			if (component.value.HasMember("velocityvariation"))
			{
				const auto& velocityvariation = component.value["velocityvariation"].GetArray();
				const float x = velocityvariation[0].GetFloat();
				const float y = velocityvariation[1].GetFloat();
				const float z = velocityvariation[2].GetFloat();
				ParticleProp.velocityvariation = Math::Vector3(x, y, z);
			}
			if (component.value.HasMember("startColor"))
			{
				const auto& velocityvariation = component.value["startColor"].GetArray();
				const float a = velocityvariation[0].GetFloat();
				const float r = velocityvariation[1].GetFloat();
				const float g = velocityvariation[2].GetFloat();
				const float b = velocityvariation[2].GetFloat();
				ParticleProp.startColor = Math::Vector4(r, g, b, a);
			}
			if (component.value.HasMember("endColor"))
			{
				const auto& velocityvariation = component.value["endColor"].GetArray();
				const float a = velocityvariation[0].GetFloat();
				const float r = velocityvariation[1].GetFloat();
				const float g = velocityvariation[2].GetFloat();
				const float b = velocityvariation[2].GetFloat();
				ParticleProp.endColor = Math::Vector4(r, g, b, a);
			}
		}
	}
}

