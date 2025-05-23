#pragma once
#include "Hit.h"
#include "Ray.h"

class Object
{
public:
    // Material
    glm::vec3 amb = glm::vec3(0.0f);
    glm::vec3 dif = glm::vec3(0.0f);
    glm::vec3 spec = glm::vec3(0.0f);
    float alpha = 10.0f;

    Object(const vec3& color = { 1.0f, 1.0f, 1.0f })
        : amb(color), dif(color), spec(color)
    {
    }

    virtual Hit CheckRayCollision(Ray& ray) = 0;
};

