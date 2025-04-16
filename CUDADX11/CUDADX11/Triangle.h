#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
	vec3 v0, v1, v2;

public:
	Triangle()
		: v0(vec3(0.0f)), v1(vec3(0.0f)), v2(vec3(0.0f))
	{
	
	}

	Triangle(vec3 v0, vec3 v1, vec3 v2)
		:v0(v0), v1(v1), v2(v2)
	{

	}

	virtual Hit CheckRayCollision(Ray& ray);

	// 삼각형 그리기 참고
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	bool IntersectRayTriangle(const vec3& orig, const vec3& dir,
		const vec3& v0, const vec3& v1,
		const vec3& v2, vec3& point, vec3& faceNormal,
		float& t, float& u, float& v);
};

