#pragma once

// Engine headers
#include <Core/Inc/Core.h>
#include <Math/Inc/EngineMath.h>

// DirectX headers
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//External library
#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/ImGuiFileDialog.h>

// DirectX libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

// Utility function to release DirectX interfaces
template <class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

