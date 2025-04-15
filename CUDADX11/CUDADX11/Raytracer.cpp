#include "pch.h"
#include "Raytracer.h"

glm::vec3 Raytracer::TransformScreenToWorld(glm::vec2 posScreen)
{
	const float xScale = 2.0f / (this->width - 1);
	const float yScale = 2.0f / (this->height - 1);
	const float aspect = float(this->width) / this->height;

	return glm::vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
}

glm::vec3 Raytracer::traceRay(Ray& ray)
{
	// �浹 ����
	const Hit hit = sphere->IntersectRayCollision(ray);

	if (hit.d < 0.0f)
	{
		return glm::vec3(0.0f);
	}
	else
	{
		// ���⼭ �� ��(Phong reflection model)���� ���� ȿ�� ���!
		// ���� �ڷ�
		// https://en.wikipedia.org/wiki/Phong_reflection_model
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF

		// Diffuse
		const glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
		const float diff = glm::max(glm::dot(hit.normal, dirToLight), 0.0f);

		// Specular
		const glm::vec3 reflectDir = 2.0f * dot(hit.normal, dirToLight) * hit.normal - dirToLight;// r = 2 (n dot l) n - l
		const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), sphere->alpha);

		return sphere->amb + sphere->diff * diff + sphere->spec * specular * sphere->ks;//sphere->color * hit.d; // ���̸� ���ؼ� ��ü�� �����
	}
}

void Raytracer::Render(vector<glm::vec4>& pixels)
{
	std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			// 2D�� 3D�� ���� ��ȯ
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));

			// ������ ���� ����
			// ��ũ���� ������ z����, ���밪 1.0�� ���� ����
			// Orthographic projection (������) vs perspective projection (��������)
			const auto rayDir = glm::vec3(0.0f, 0.0f, 1.0f);

			Ray pixelRay{ pixelPosWorld, rayDir };

			// index���� size_t�� ��� (index�� ������ ���� �����ϱ�)
			// traceRay()�� ��ȯ���� vec3 (RGB), A�� ���ʿ�
			pixels[size_t(i + width * j)] = glm::vec4(traceRay(pixelRay), 1.0f);
		}
}
