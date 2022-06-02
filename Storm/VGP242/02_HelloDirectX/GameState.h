#pragma once

#include <Storm/Inc/Storm.h>

class GameState :public Storm::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;



};