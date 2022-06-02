#pragma once

namespace Storm::Graphics
{
	class DepthStencilState final
	{
	public:
		static void ClearState();

	public:
		enum class DepthTest
		{
			Enable,
			Disable
		};
		enum class DepthWrite
		{
			Enable,
			Disable
		};

		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize(DepthTest depthTest, DepthWrite depthWrite);
		void Terminate();

		void Set();

	private:
		ID3D11DepthStencilState* mDepthStencilState = nullptr;
	};
}
