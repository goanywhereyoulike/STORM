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

	//Storm::Graphics::Camera mCamera;
	Storm::GameWorld mGameWorld;
	Storm::GameObjectHandle mGameObjectHandle;

	std::vector<Storm::GameObjectHandle> mGameObjectHandles;

	//float mFPS = 0.0f;


};