#pragma once
#include "Triangle.h"

class Square : public Object
{
public:
    Triangle triangle1, triangle2;

    Square(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
        : triangle1(v0, v1, v2), triangle2(v0, v2, v3)
    {
    }

	virtual Hit CheckRayCollision(Ray& ray);
};

