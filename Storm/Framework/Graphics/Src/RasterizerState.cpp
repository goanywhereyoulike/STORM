#include "Precompiled.h"
#include "RasterizerState.h"

#include "GraphicsSystem.h"

using namespace Storm::Graphics;

namespace
{
	inline D3D11_CULL_MODE GetCullMode(RasterizerState::CullMode cullMode)
	{
		switch (cullMode)
		{
		case RasterizerState::CullMode::Front:
			return D3D11_CULL_FRONT;
		case RasterizerState::CullMode::Back:
			return D3D11_CULL_BACK;
		case RasterizerState::CullMode::None:
			return D3D11_CULL_NONE;
		}
		return D3D11_CULL_NONE;
	};

	inline D3D11_FILL_MODE GetFillMode(RasterizerState::FillMode fillMode)
	{
		switch (fillMode)
		{
		case RasterizerState::FillMode::Solid:
			return D3D11_FILL_SOLID;
		case RasterizerState::FillMode::Wireframe:
			return D3D11_FILL_WIREFRAME;
		}
		return D3D11_FILL_WIREFRAME;
	};
}

void RasterizerState::Clear()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->RSSetState(nullptr);
}

RasterizerState::~RasterizerState()
{
	ASSERT(mRasterizerState == nullptr, "RasterizerState -- Rasterizer state not released!");
}

void RasterizerState::Initialize(CullMode cullMode, FillMode fillMode)
{
	D3D11_RASTERIZER_DESC desc{};
	desc.CullMode = GetCullMode(cullMode);
	desc.FillMode = GetFillMode(fillMode);
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateRasterizerState(&desc, &mRasterizerState);
	ASSERT(SUCCEEDED(hr), "RasterizerState -- Failed to create rasterizer state.");
}

void RasterizerState::Terminate()
{
	SafeRelease(mRasterizerState);
}

void RasterizerState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->RSSetState(mRasterizerState);
}
