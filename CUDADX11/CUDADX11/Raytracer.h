#pragma once
#include "Sphere.h"
#include "Light.h"

class Raytracer
{
public:
	uint32_t width, height;
	Light light;
	shared_ptr<Sphere> sphere;

	Raytracer(const uint32_t& width, const uint32_t& height)
		: width(width), height(height)
	{
		sphere = make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.5f), 0.5f);
		sphere->amb = glm::vec3(0.0f, 0.0f, 0.0f);
		sphere->diff = glm::vec3(0.0f, 0.0f, 1.0f);
		sphere->spec = glm::vec3(1.0f, 1.0f, 1.0f);
		sphere->alpha = 9.0f;
		sphere->ks = 0.8f;

		light = Light{ {0.0f, 0.0f, -1.0f} };
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 posScreen);

	// 광선이 물체에 닿으면 그 물체의 색 반환
	glm::vec3 traceRay(Ray& ray);

	void Render(std::vector<glm::vec4>& pixels);

};