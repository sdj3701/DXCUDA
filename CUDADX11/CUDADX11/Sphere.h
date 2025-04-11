#pragma once
#include "Hit.h"
#include "Ray.h"

class Sphere
{
public:
    glm::vec3 center;
    float radius;
    glm::vec3 color; // 뒤에서 '재질(material)'로 확장

    Sphere(const glm::vec3& center, const float radius, const glm::vec3& color)
        : center(center), color(color), radius(radius)
    {
    }

    Hit IntersectRayCollision(Ray& ray)
    {
        Hit hit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) }; // d가 음수이면 충돌을 안한 것으로 가정

        /*
         * hit.d = ... // 광선의 시작점으로부터 충돌지점까지의 거리 (float)
         * hit.point = ... // 광선과 구가 충돌한 지점의 위치 (vec3)
         * hit.normal = .. // 충돌 지점에서 구의 단위 법선 벡터(unit normal vector)
         */

        const float b = 2.0f * glm::dot(ray.dir, ray.start - center);
        const float c = dot(ray.start - center, ray.start - center) - radius * radius;
        const float nabla = b * b / 4.0f - c;

        //hit.d = 
        if (nabla >= 0.0f) // 충돌 지점이 0개인 경우 (그릴 필요 X)
        {
            const float d1 = -b / 2.0f + sqrt(nabla);
            const float d2 = -b / 2.0f - sqrt(nabla);
            hit.d = glm::min(d1, d2);
            hit.point = ray.start + ray.dir - hit.d;
            hit.normal = glm::normalize(hit.point - this->center);
        }
        else if (nabla == 0.0f) // 충돌 지점이 1개인 경우 (꼭지점)
        {

        }
        else if (nabla > 0.0f) // 충돌 지점이 2개인 경우 (관동한 경우)
        {

        }
        // const float a = glm::dot(ray.dir, ray.dir); // dir이 unit vector라면 a는 1.0f라서 생략 가능

        return hit;
    }
};