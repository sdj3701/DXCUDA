#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "DirectXMath.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>


using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using Vec2 = DirectX::XMFLOAT2;
using Vec3 = DirectX::XMFLOAT3;
using Vec4 = DirectX::XMFLOAT4;
using Color = DirectX::XMFLOAT4;

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
