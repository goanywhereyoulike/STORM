#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Math;
using namespace Storm::Input;

void GameState::Initialize()
{

	// NDC - Normalized Device Coordinate
//
//          +-----------------+
//         /                 /|
//   1.0  +-----------------+ |
//	      |        ^        | |
//	      |        |        | |
//	      | <------+------> | |
//	      |        |        | | 1.0
//	      |        v        |/
//	-1.0  +-----------------+ 0.0
//      -1.0               1.0
//

	//-----------------------------------------------------------------------------
	mVertices[0].push_back({ Vector3{  0.0f,  0.0f, 0.0f },Colors::Red });
	mVertices[0].push_back({ Vector3{  0.25f, 0.4f, 0.0f } ,Colors::Red });
	mVertices[0].push_back({ Vector3{  0.5f, 0.0f, 0.0f },Colors::Red });

	mVertices[0].push_back({ Vector3{  0.5f,  0.0f, 0.0f },Colors::Red });
	mVertices[0].push_back({ Vector3{  0.4f, -0.4f, 0.0f } ,Colors::Red });
	mVertices[0].push_back({ Vector3{ 0.0f, 0.0f, 0.0f },Colors::Red });

	mVertices[0].push_back({ Vector3{  0.4f, -0.4f, 0.0f } ,Colors::Red });
	mVertices[0].push_back({ Vector3{  0.0f,  -0.9f, 0.0f },Colors::Red });
	mVertices[0].push_back({ Vector3{ 0.0f, 0.0f, 0.0f },Colors::Red });


	mVertices[0].push_back({ Vector3{  0.0f,  -0.9f, 0.0f },Colors::Red });
	mVertices[0].push_back({ Vector3{  -0.4f, -0.4f, 0.0f } ,Colors::Red });
	mVertices[0].push_back({ Vector3{ 0.0f, 0.0f, 0.0f },Colors::Red });

	mVertices[0].push_back({ Vector3{ -0.4f, -0.4f, 0.0f } ,Colors::Red });
	mVertices[0].push_back({ Vector3{  -0.5f,  0.0f, 0.0f },Colors::Red });
	mVertices[0].push_back({ Vector3{ 0.0f, 0.0f, 0.0f },Colors::Red });

	mVertices[0].push_back({ Vector3{  -0.5f, 0.0f, 0.0f },Colors::Red });
	mVertices[0].push_back({ Vector3{  -0.25f, 0.4f, 0.0f } ,Colors::Red });
	mVertices[0].push_back({ Vector3{  0.0f,  0.0f, 0.0f },Colors::Red });
	//-----------------------------------------------------------------------------
	mVertices[1].push_back({ Vector3{  0.0f, 0.5f, 0.0f },Colors::Yellow });
	mVertices[1].push_back({ Vector3{  0.3f, 0.0f, 0.0f } ,Colors::Yellow });
	mVertices[1].push_back({ Vector3{  -0.3f,  0.0f, 0.0f },Colors::Yellow });

	mVertices[1].push_back({ Vector3{  0.3f, 0.0f, 0.0f },Colors::Yellow });
	mVertices[1].push_back({ Vector3{  0.8f, -0.8f, 0.0f } ,Colors::Yellow });
	mVertices[1].push_back({ Vector3{  0.0f,  -0.8f, 0.0f },Colors::Yellow });

	mVertices[1].push_back({ Vector3{  -0.3f, 0.0f, 0.0f },Colors::Yellow });
	mVertices[1].push_back({ Vector3{  0.0f, -0.8f, 0.0f } ,Colors::Yellow });
	mVertices[1].push_back({ Vector3{  -0.8f,  -0.8f, 0.0f },Colors::Yellow });
	//-----------------------------------------------------------------------------

	mVertices[2].push_back({ Vector3{  -0.5f, 0.9f, 0.0f } ,Colors::White });
	mVertices[2].push_back({ Vector3{ -0.4f, 0.6f, 0.0f },Colors::White });
	mVertices[2].push_back({ Vector3{  -0.6f,  0.6f, 0.0f },Colors::White });

	mVertices[2].push_back({ Vector3{  -0.4f, 0.6f, 0.0f  } ,Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{ -0.3f,0.3f, 0.0f },Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{  -0.5f,  0.3f, 0.0f },Colors::SaddleBrown });

	mVertices[2].push_back({ Vector3{  -0.4f, 0.6f, 0.0f  } ,Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{  -0.5f,  0.3f, 0.0f },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ -0.6f,0.6f, 0.0f },Colors::SaddleBrown });

	mVertices[2].push_back({ Vector3{  -0.6f, 0.6f, 0.0f  } ,Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ -0.5f,0.3f, 0.0f },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{  -0.7f,  0.3f, 0.0f },Colors::SaddleBrown });


	mVertices[2].push_back({ Vector3{ -0.3f,0.3f, 0.0f },Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{ -0.1f,  0.0f, 0.0f },Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{  -0.4f, 0.0f, 0.0f  } ,Colors::SandyBrown });

	mVertices[2].push_back({ Vector3{ -0.3f,0.3f, 0.0f },Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{ -0.4f, 0.0f, 0.0f  },Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{  -0.5f,  0.3f, 0.0f  } ,Colors::SaddleBrown });

	mVertices[2].push_back({ Vector3{  -0.5f,  0.3f, 0.0f  } ,Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ -0.4f, 0.0f, 0.0f  },Colors::SandyBrown });
	mVertices[2].push_back({ Vector3{ -0.7f, 0.0f, 0.0f  },Colors::SaddleBrown });

	mVertices[2].push_back({ Vector3{  -0.5f,  0.3f, 0.0f  } ,Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ -0.7f, 0.0f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ -0.7f, 0.3f, 0.0f  },Colors::SaddleBrown });

	mVertices[2].push_back({ Vector3{ -0.7f, 0.3f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ -0.7f, 0.0f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{  -0.9f,  0.0f, 0.0f  } ,Colors::SaddleBrown });
	//---------------------------
	//---------------------------
	mVertices[2].push_back({ Vector3{ -1.0f, 0.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{ 0.0f, 0.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{  -1.0f,  -1.0f, 0.0f  } ,Colors::Green });

	mVertices[2].push_back({ Vector3{ 0.0f, 0.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{ 0.0f, -1.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{  -1.0f,  -1.0f, 0.0f  } ,Colors::Green });

	mVertices[2].push_back({ Vector3{ 0.0f, 0.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{ 1.0f,-1.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{  0.0f,  -1.0f, 0.0f  } ,Colors::Green });

	mVertices[2].push_back({ Vector3{ 0.0f, 0.0f, 0.0f  },Colors::Green });
	mVertices[2].push_back({ Vector3{ 1.0f, 0.0f, 0.0f  },Colors::GreenYellow });
	mVertices[2].push_back({ Vector3{  1.0f,  -1.0f, 0.0f  } ,Colors::Green });

	mVertices[2].push_back({ Vector3{ 0.5f, 0.4f, 0.0f  },Colors::DarkGreen });
	mVertices[2].push_back({ Vector3{ 0.6f, 0.3f, 0.0f  },Colors::DarkGreen });
	mVertices[2].push_back({ Vector3{  0.4f,  0.3f, 0.0f  } ,Colors::DarkGreen });

	mVertices[2].push_back({ Vector3{ 0.5f, 0.3f, 0.0f  },Colors::DarkGreen });
	mVertices[2].push_back({ Vector3{ 0.65f, 0.2f, 0.0f  },Colors::DarkGreen });
	mVertices[2].push_back({ Vector3{  0.35f,  0.2f, 0.0f  } ,Colors::DarkGreen });

	mVertices[2].push_back({ Vector3{ 0.48f, 0.2f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ 0.52f, 0.2f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{  0.52f,  0.0f, 0.0f  } ,Colors::SaddleBrown });

	mVertices[2].push_back({ Vector3{ 0.48f, 0.2f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{ 0.52f,  0.0f, 0.0f  },Colors::SaddleBrown });
	mVertices[2].push_back({ Vector3{  0.48f,  0.0f, 0.0f  } ,Colors::SaddleBrown });



	// -------------------------------------------
	// Create vertex buffer

	for (int i = 0; i < 3; ++i)
	{
		mVertexBuffer[i].Initialize(mVertices[i].data(), sizeof(Vertex), static_cast<uint32_t>(mVertices[i].size()));
	}

		// -------------------------------------------


		// -------------------------------------------
		// Compile and create vertex shader
	mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/DoSomething.fx");
		// -------------------------------------------

		// -------------------------------------------
		// Compile and create pixel shader
	mPixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");

	// -------------------------------------------


}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	for (int i = 0; i < 3; ++i)
	{
		mVertexBuffer[i].Terminate();
	}

}

void GameState::Update(float deltaTime)
{
	auto graphicsSystem = GraphicsSystem::Get();

	graphicsSystem->SetClearColor(Colors::Gray);

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ONE))
	{
		select = 0;
	}
	if (inputSystem->IsKeyPressed(KeyCode::TWO))
	{

		select = 1;

	}
	if (inputSystem->IsKeyPressed(KeyCode::THREE))
	{
		select = 2;

	}


}
void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mVertexBuffer[select].Render();

}
