#pragma once

namespace Storm::Graphics
{
	class Texture
	{
	public:
		static void UnbindPS(uint32_t slot);
	public:
		enum class Format
		{
			RGBA_U8,
			RGBA_U32
		};
		Texture() = default;
		virtual ~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;


		virtual void Initialize(const std::filesystem::path& fileName);
		virtual void Initialize(uint32_t width,uint32_t height,Format format);
		virtual void Terminate();

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;

		void* GetRawData() const { return mShaderResourceView; }

	protected:
		//DXGI- DirectX Graphics Infrastructure
		DXGI_FORMAT GetDXGIFormat(Format format);

		ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	};
}

