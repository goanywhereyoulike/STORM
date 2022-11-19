#pragma once
#include <Storm/Inc/Storm.h>
class GameState :public Storm::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
private:

	struct TransformData0
	{
		Storm::Math::Matrix4 wvp;

	};

	struct TransformData
	{
		Storm::Math::Matrix4 world;
		Storm::Math::Matrix4 wvp;
		Storm::Math::Vector3 viewPosition;
		float padding;

	};
	struct SettingsData
	{
		int useSpecularMap = 1;
		int useDisplacementMap = 1;
		float displacementWeight = 0.3f;
		int useNormalMap = 1;


	};
	Storm::Graphics::Camera mCamera;
	Storm::Graphics::Mesh mSphereMesh;
	Storm::Graphics::MeshBuffer mSphereMeshBuffer;

	Storm::Graphics::MeshPX mCloudsSphereMesh;
	Storm::Graphics::MeshBuffer mCloudsSphereMeshBuffer;

	Storm::Graphics::VertexShader mVertexShader;
	Storm::Graphics::PixelShader mPixelShader;

	Storm::Graphics::VertexShader mTexturingVertexShader;
	Storm::Graphics::PixelShader mTexturingPixelShader;
	Storm::Graphics::TypedConstantBuffer<TransformData0> mConstantBuffer;

	using TransformBuffer = Storm::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::DirectionalLight>;
	using MaterialBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::Material>;
	using SettingsBuffer = Storm::Graphics::TypedConstantBuffer<SettingsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;

	SettingsData mSettingsData;
	Storm::Graphics::Texture mDiffuseMap;
	Storm::Graphics::Texture mSpecularMap;
	Storm::Graphics::Texture mDisplacementMap;
	Storm::Graphics::Texture mNormalMap;
	Storm::Graphics::Texture mCloudsMap;
	Storm::Graphics::Sampler mSampler;

	TransformData mTransform;
	TransformData0 mTransform0;
	Storm::Graphics::DirectionalLight mDirectionLight;
	Storm::Graphics::Material mMaterial;

	float mFPS = 0.0f;

	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mRotation = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mCloudsRotation = Storm::Math::Vector3::Zero;

	Storm::Graphics::BlendState mBlendstate;
};