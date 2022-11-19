#include "Precompiled.h"
#include "SimpleEffect.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "MeshTypes.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Storm;
using namespace Storm::Graphics;

void SimpleEffect::Initialize()
{
	mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/SimpleEffect.fx");
	mPixelShader.Initialize(L"../../Assets/Shaders/SimpleEffect.fx");
	mNormalBuffer.Initialize();

	MeshPCX particlemesh = MeshBuilder::CreateParticle2D(0.1f, Colors::White);
	particleMeshBuffer.Initialize(particlemesh, true);

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendstate.Initialize(Storm::Graphics::BlendState::Mode::Additive);
}
  
void SimpleEffect::Terminate()
{
	mBlendstate.Terminate();
	mSampler.Terminate();
	particleMeshBuffer.Terminate();
	mNormalBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void SimpleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mNormalBuffer.BindVS(0);
	mNormalBuffer.BindPS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void SimpleEffect::End()
{
}

void SimpleEffect::Render(Particle& particle)
{
	ASSERT(mCamera != nullptr, "TextureEffect -- no camera set!");

	float percent = particle.lifeRemain / particle.lifeTime;
	float size = particle.size * percent;
	//Color color = particle.color * percent;
	// float size = Lerp(particle.startSize, particle.endSize, percent);
	Color color = Math::Lerp(particle.endColor, particle.startColor, percent);

	Math::Matrix4 scaling = Math::Matrix4::Scaling(size);
	Math::Matrix4 world = scaling * particle.transform.GetMatrix4();
	Math::Matrix4 view = mCamera->GetViewMatrix();
	Math::Matrix4 proj = mCamera->GetProjectionMatrix();
	Math::Matrix4 wvp = Math::Transpose(world * view * proj);
	NormalData normalData;
	normalData.wvp = wvp;
	normalData.color = color;
	mNormalBuffer.Update(normalData);

	auto tm = TextureManager::Get();
	tm->BindPS(particle.diffuseMapId, 0);


	particleMeshBuffer.Render();
}

void SimpleEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
