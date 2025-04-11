#pragma once
#include "Hit.h"
#include "Ray.h"

class Sphere
{
public:
    glm::vec3 center;
    float radius;
    glm::vec3 color; // �ڿ��� '����(material)'�� Ȯ��

    Sphere(const glm::vec3& center, const float radius, const glm::vec3& color)
        : center(center), color(color), radius(radius)
    {
    }

    Hit IntersectRayCollision(Ray& ray)
    {
        Hit hit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) }; // d�� �����̸� �浹�� ���� ������ ����

        /*
         * hit.d = ... // ������ ���������κ��� �浹���������� �Ÿ� (float)
         * hit.point = ... // ������ ���� �浹�� ������ ��ġ (vec3)
         * hit.normal = .. // �浹 �������� ���� ���� ���� ����(unit normal vector)
         */

        const float b = 2.0f * glm::dot(ray.dir, ray.start - center);
        const float c = dot(ray.start - center, ray.start - center) - radius * radius;
        const float nabla = b * b / 4.0f - c;

        //hit.d = 
        if (nabla >= 0.0f) // �浹 ������ 0���� ��� (�׸� �ʿ� X)
        {
            const float d1 = -b / 2.0f + sqrt(nabla);
            const float d2 = -b / 2.0f - sqrt(nabla);
            hit.d = glm::min(d1, d2);
            hit.point = ray.start + ray.dir - hit.d;
            hit.normal = glm::normalize(hit.point - this->center);
        }
        else if (nabla == 0.0f) // �浹 ������ 1���� ��� (������)
        {

        }
        else if (nabla > 0.0f) // �浹 ������ 2���� ��� (������ ���)
        {

        }
        // const float a = glm::dot(ray.dir, ray.dir); // dir�� unit vector��� a�� 1.0f�� ���� ����

        return hit;
    }
};