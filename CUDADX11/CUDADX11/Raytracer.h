#pragma once
#include "Sphere.h"

class Raytracer
{
public:
	uint32_t width, height;
	shared_ptr<Sphere> sphere;

	Raytracer(const uint32_t& width, const uint32_t& height)
		: width(width), height(height)
	{
		sphere = make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.5f), 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 posScreen);

	// 광선이 물체에 닿으면 그 물체의 색 반환
	glm::vec3 traceRay(Ray& ray);

	void Render(std::vector<glm::vec4>& pixels);

};