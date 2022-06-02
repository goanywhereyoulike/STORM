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
	struct TransformData
	{
		Storm::Math::Matrix4 wvp;


	};
	Storm::Graphics::Camera mCamera;



	float posZ = 5.0f;
	float angleY = 0.0f;
	//float posZ = 5.0f;
	float angleX = 0.0f;

};