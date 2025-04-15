#pragma once
#include "Sphere.h"
#include "Light.h"

class Raytracer
{
public:
	uint32_t width, height;
	Light light;
	std::vector<shared_ptr<Object>> objects;

	Raytracer(const uint32_t& width, const uint32_t& height)
		: width(width), height(height)
	{
		// ��ũ�����κ��� �Ÿ��� �ٸ� �� 3��
		auto sphere1 = make_shared<Sphere>(glm::vec3(0.5f, 0.0f, 0.5f), 0.4f, glm::vec3(0.5f, 0.5f, 0.5f));
		auto sphere2 = make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 1.0f), 0.4f, glm::vec3(0.5f, 0.5f, 0.5f));
		auto sphere3 = make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, 1.5f), 0.4f, glm::vec3(0.5f, 0.5f, 0.5f));

		sphere1->amb = glm::vec3(0.2f);
		sphere1->dif = glm::vec3(1.0f, 0.2f, 0.2f);
		sphere1->spec = glm::vec3(0.5f);
		sphere1->alpha = 10.0f;

		sphere2->amb = glm::vec3(0.2f);
		sphere2->dif = glm::vec3(0.2f, 1.0f, 0.2f);
		sphere2->spec = glm::vec3(0.5f);
		sphere2->alpha = 10.0f;

		sphere3->amb = glm::vec3(0.2f);
		sphere3->dif = glm::vec3(0.2f, 0.2f, 1.0f);
		sphere3->spec = glm::vec3(0.5f);
		sphere3->alpha = 10.0f;

		// �Ϻη� �������� �߰�
		objects.push_back(sphere3);
		objects.push_back(sphere2);
		objects.push_back(sphere1);

		light = Light{ {0.0f, 0.0f, -1.0f} };
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 posScreen);

	// ������ ��ü�� ������ �� ��ü�� �� ��ȯ
	glm::vec3 traceRay(Ray& ray);

	void Render(std::vector<glm::vec4>& pixels);
	Hit FindClosestCollision(Ray& ray);

};