#include "Precompiled.h"
#include "ParticleTexturingEffect.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Storm;
using namespace Storm::Graphics;

void ParticleTexturingEffect::Initialize()
{
	mTexturingVertexShader.Initialize<VertexPCX>(L"../../Assets/Shaders/Particle.fx");
	mTexturingPixelShader.Initialize(L"../../Assets/Shaders/Particle.fx");
	mParticleBuffer.Initialize();
	particleMeshBuffer.Initialize(nullptr,sizeof(VertexPCX),6, true);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendstate.Initialize(BlendState::Mode::Additive);
	mDepthStencilState.Initialize(DepthStencilState::DepthTest::Enable, DepthStencilState::DepthWrite::Disable);
}

void ParticleTexturingEffect::Terminate()
{
	particleMeshBuffer.Terminate();
	mDepthStencilState.Terminate();
	mBlendstate.Terminate();
	mSampler.Terminate();
	mParticleBuffer.Terminate();
	mTexturingPixelShader.Terminate();
	mTexturingVertexShader.Terminate();


}

void ParticleTexturingEffect::Begin()
{
	mTexturingVertexShader.Bind();
	mTexturingPixelShader.Bind();

	mParticleBuffer.BindVS(0);
	mParticleBuffer.BindPS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mDepthStencilState.Set();
}

void ParticleTexturingEffect::End()
{
	DepthStencilState::ClearState();
}

void ParticleTexturingEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "TextureEffect -- no camera set!");
	Math::Matrix4 world = renderObject.transform.GetMatrix4();
	Math::Matrix4 view = mCamera->GetViewMatrix();
	Math::Matrix4 proj = mCamera->GetProjectionMatrix();
	Math::Matrix4 wvp = Math::Transpose(world * view * proj);
	mTransformBuffer.Update(wvp);

	if (renderObject.useAdditiveBlend)
		mBlendstate.Set();

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	renderObject.meshBuffer.Render();
	if (renderObject.useAdditiveBlend)
		mBlendstate.ClearState();

}

void ParticleTexturingEffect::RenderParticle(const Particle& particle)
{
	ASSERT(mCamera != nullptr, "TextureEffect -- no camera set!");
	float percent = particle.lifeRemain / particle.lifeTime;
	float size = particle.size * percent;
	Math::Matrix4 scaling = Math::Matrix4::Scaling(size);
	Math::Matrix4 world = scaling * particle.transform.GetMatrix4();
	Math::Matrix4 view = mCamera->GetViewMatrix();
	Math::Matrix4 proj = mCamera->GetProjectionMatrix();
	Math::Matrix4 wvp = Math::Transpose(world * view * proj);

	Math::Vector3 ParticleUp = Math::Normalize(mCamera->GetUpDirection());
	auto CameraDirection = Math::Normalize(mCamera->GetDirection());
	auto ParticleSide = Math::Normalize(Math::Cross(ParticleUp, CameraDirection));
	ParticleData particledata;
	Color color = Math::Lerp(particle.endColor, particle.startColor, percent);
	//Math::Matrix4 world = particle.transform.GetMatrix4();
	//Math::Matrix4 view = mCamera->GetViewMatrix();
	//Math::Matrix4 proj = mCamera->GetProjectionMatrix();
	//Math::Matrix4 wvp = Math::Transpose(world * view * proj);
	color.a = percent;
	particledata.wvp = wvp;
	particledata.color = color;
	mParticleBuffer.Update(particledata);

	std::vector<VertexPCX> vertices;
	const uint32_t vertexCount = 6;

	vertices.reserve(vertexCount);
	VertexPCX tl, tr, bl, br;

	Math::Vector2 side = { 1.0f,0.0f };
	Math::Vector2 up = { 0.0f,0.0f };

	tl.position = particle.transform.position - (ParticleSide * size) + (ParticleUp * size);
	tr.position = particle.transform.position + (ParticleSide * size) + (ParticleUp * size);
	bl.position = particle.transform.position - (ParticleSide * size) - (ParticleUp * size);
	br.position = particle.transform.position + (ParticleSide * size) - (ParticleUp * size);

	tl.color = color;
	tr.color = color;
	bl.color = color;
	br.color = color;
	tl.texCoord = Math::Vector2{ 0.0f, 0.0f };
	tr.texCoord = Math::Vector2{ 1.0f, 0.0f };
	bl.texCoord = Math::Vector2{ 0.0f, 1.0f };
	br.texCoord = Math::Vector2{ 1.0f, 1.0f };

	vertices.push_back(tl);
	vertices.push_back(tr);
	vertices.push_back(bl);
	vertices.push_back(tr);
	vertices.push_back(br);
	vertices.push_back(bl);

	particleMeshBuffer.Update(vertices.data(), static_cast<uint32_t>(vertices.size()));

	if (particle.useAdditiveBlend)
		mBlendstate.Set();

	auto tm = TextureManager::Get();
	tm->BindPS(particle.diffuseMapId, 0);
	particleMeshBuffer.Render();
	if (particle.useAdditiveBlend)
		mBlendstate.ClearState();

}


void ParticleTexturingEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

