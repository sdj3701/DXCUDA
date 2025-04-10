#pragma once

#include "Types.h"
#include "Values.h"
#include "Struct.h"
#include <iostream>
// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

// ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

// Win
#include <Windows.h>
#include <assert.h>
// 가속 멀티쓰레딩
#include <omp.h>

//DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectxTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectxTex.lib")
#endif // _DEBUG

#define CHECK(p) assert(SUCCEEDED(p))

