#pragma once

#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Triangle.h"
#include "Square.h"

class Raytracer
{
public:
	uint32_t width, height;
	Light light;
	shared_ptr<Sphere> sphere;
	vector<shared_ptr<Object>> objects;

	Raytracer(const uint32_t& width, const uint32_t& height)
		: width(width), height(height)
	{
		auto sphere1 = make_shared<Sphere>(vec3(0.0f, 0.0f, 0.6f), 0.4f);

		sphere1->amb = vec3(0.2f, 0.0f, 0.0f);
		sphere1->dif = vec3(1.0f, 0.1f, 0.1f);
		sphere1->spec = vec3(1.5f);
		sphere1->alpha = 50.0f;

		this->sphere = sphere1; // GUI 연결하기 위해 보관

		objects.push_back(sphere1);

		auto triangle1 = make_shared<Triangle>(vec3(-2.0f, -1.0f, 0.0f), vec3(-2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 4.0f));
		auto triangle2 = make_shared<Triangle>(vec3(-2.0f, -1.0f, 0.0f), vec3(2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 0.0f));
		triangle1->amb = vec3(0.2f);
		triangle1->dif = vec3(0.8f);
		triangle1->spec = vec3(1.0f);
		triangle1->alpha = 50.0f;
		triangle2->amb = vec3(0.2f);
		triangle2->dif = vec3(0.8f);
		triangle2->spec = vec3(1.0f);
		triangle2->alpha = 50.0f;
		objects.push_back(triangle1);
		objects.push_back(triangle2);

		light = Light{ {0.0f, 1.0f, 0.2f} }; // 화면 뒷쪽
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 posScreen);

	// 광선이 물체에 닿으면 그 물체의 색 반환
	glm::vec3 traceRay(Ray& ray);

	void Render(std::vector<glm::vec4>& pixels);
	Hit FindClosestCollision(Ray& ray);

};