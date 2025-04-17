#include "pch.h"
#include "Square.h"

Hit Square::CheckRayCollision(Ray& ray)
{
    auto hit1 = triangle1.CheckRayCollision(ray);
    auto hit2 = triangle2.CheckRayCollision(ray);

    if (hit1.d >= 0.0f && hit2.d >= 0.0f)
    {
        return hit1.d < hit2.d ? hit1 : hit2;
    }
    else if (hit1.d >= 0.0f)
    {
        return hit1;
    }
    else
    {
        return hit2;
    }
}
