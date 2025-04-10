#pragma once
#include "Types.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

struct Vertex
{
	Vec3 position;
	//Color color;
	Vec2 uv;
};

struct TransformData
{
	Vec3 offset;
	float dummy;
};

struct CircleData 
{
	glm::vec2 center;
	float radius;
	float padding;
	glm::vec4 color;
};