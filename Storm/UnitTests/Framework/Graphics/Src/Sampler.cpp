#include "Precompiled.h"
#include "Sampler.h"

#include "GraphicsSystem.h"

using namespace Storm::Graphics;

namespace
{
	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case Sampler::Filter::Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case Sampler::Filter::Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case Sampler::Filter::Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;
		}
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AddressMode mode)
	{
		switch (mode)
		{
		case Sampler::AddressMode::Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		case Sampler::AddressMode::Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case Sampler::AddressMode::Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case Sampler::AddressMode::Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		}
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}
}

Sampler::~Sampler()
{
	ASSERT(mSampler == nullptr, "Sampler -- Sampler not released!");
}

void Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	auto d3dFilter = GetFilter(filter);
	auto d3dAddressMode = GetAddressMode(addressMode);

	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = d3dFilter;
	sampDesc.AddressU = d3dAddressMode;
	sampDesc.AddressV = d3dAddressMode;
	sampDesc.AddressW = d3dAddressMode;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 1.0f;
	sampDesc.BorderColor[2] = 1.0f;
	sampDesc.BorderColor[3] = 1.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto device = GraphicsSystem::Get()->GetDevice();
	auto hr = device->CreateSamplerState(&sampDesc, &mSampler);
	ASSERT(SUCCEEDED(hr), "Sampler -- Failed to create sampler state.");
}

void Sampler::Terminate()
{
	SafeRelease(mSampler);
}

void Sampler::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetSamplers(slot, 1, &mSampler);
}

void Sampler::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetSamplers(slot, 1, &mSampler);
}

