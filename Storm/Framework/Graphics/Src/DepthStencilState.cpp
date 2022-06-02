#include "Precompiled.h"
#include "DepthStencilState.h"

#include "GraphicsSystem.h"

using namespace Storm::Graphics;

void DepthStencilState::ClearState()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(nullptr, 0);
}

DepthStencilState::~DepthStencilState()
{
	ASSERT(mDepthStencilState == nullptr, "DepthStencilState -- Depth stencil state not released!");
}

void DepthStencilState::Initialize(DepthTest depthTest, DepthWrite depthWrite)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = depthTest == DepthTest::Enable ? TRUE : FALSE;
	depthStencilDesc.DepthWriteMask = depthWrite == DepthWrite::Enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	ASSERT(SUCCEEDED(hr), "DepthStencilState -- Failed to create depth stencil state.");
}

void DepthStencilState::Terminate()
{
	SafeRelease(mDepthStencilState);
}

void DepthStencilState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(mDepthStencilState, 0);
}