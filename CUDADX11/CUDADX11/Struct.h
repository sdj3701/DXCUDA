#pragma once
#include "Types.h"

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
	Vec2 center;
	float radius;
	float padding;
	Vec4 color;
};