#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
	vec3 v0, v1, v2;

public:
	// 생성자를 2개 사용한 이유
	// 기본 생성자 : 기본값 초기화 0,0,0으로 초기화
	Triangle()
		: v0(vec3(0.0f)), v1(vec3(0.0f)), v2(vec3(0.0f))
	{
	
	}

	// 매개변수 생성자 : 즉시 초기화 생성과 동시에 정확한 겂으로 삼각형을 설정할 수 있다.
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

