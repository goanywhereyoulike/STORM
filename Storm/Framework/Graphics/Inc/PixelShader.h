#pragma once
namespace Storm::Graphics
{
	class PixelShader
	{
	public:
		PixelShader() = default;
		~PixelShader();
		void Initialize(const std::filesystem::path& filepath, const char* entryPoint = "PS");
		void Terminate();


		void Bind() const;
	private:
		ID3D11PixelShader* mPixelShader = nullptr;

	};

}