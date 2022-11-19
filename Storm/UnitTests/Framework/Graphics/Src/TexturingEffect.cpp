#include "Precompiled.h"
#include "TexturingEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Storm;
using namespace Storm::Graphics;

void TexturingEffect::Initialize()
{
	mTexturingVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/DoTexturing.fx");
	mTexturingPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
	mTransformBuffer.Initialize();
	
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendstate.Initialize(Storm::Graphics::BlendState::Mode::Additive);
}

void TexturingEffect::Terminate()
{
	mBlendstate.Terminate();
	mSampler.Terminate();
	mTransformBuffer.Terminate();
	mTexturingPixelShader.Terminate();
	mTexturingVertexShader.Terminate();


}

void TexturingEffect::Begin()
{
	mTexturingVertexShader.Bind();
	mTexturingPixelShader.Bind();

	mTransformBuffer.BindVS(0);


	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void TexturingEffect::End()
{
	Texture::UnbindPS(0);
}

void TexturingEffect::Render(const RenderObject& renderObject)
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

void TexturingEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

