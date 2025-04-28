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
	// 여기서 퐁 모델(Phong reflection model)으로 조명 효과 계산!
	// 참고 자료
	// https://en.wikipedia.org/wiki/Phong_reflection_model
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF
	// Render first hit
	const auto hit = FindClosestCollision(ray);

	if (hit.d >= 0.0f)
	{
		glm::vec3 color(hit.obj->amb);

		// Diffuse
		const vec3 dirToLight = glm::normalize(light.pos - hit.point);
		Ray shadowRay = { hit.point + dirToLight * 1e-4f, dirToLight };

		if (FindClosestCollision(shadowRay).d < 0.0f)
		{
			const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

			// Specular
			const vec3 reflectDir = 2.0f * dot(hit.normal, dirToLight) * hit.normal - dirToLight;
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

			color += hit.obj->amb + hit.obj->dif * diff + hit.obj->spec * specular;
		}

		return color;
	}

	return vec3(0.0f);
}

void Raytracer::Render(vector<glm::vec4>& pixels)
{
	// 뒷 배경
	std::fill(pixels.begin(), pixels.end(), vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const vec3 eyePos(0.0f, 0.0f, -1.5f);

#pragma omp parallel for
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			const vec3 pixelPosWorld = TransformScreenToWorld(vec2(i, j));
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - eyePos) };
			pixels[i + width * j] = vec4(glm::clamp(traceRay(pixelRay), 0.0f, 1.0f), 1.0f);
		}
}

Hit Raytracer::FindClosestCollision(Ray& ray)
{
	float closestD = 1000.0; // inf
	Hit closestHit = Hit{ -1.0, glm::dvec3(0.0), glm::dvec3(0.0) };

	for (int l = 0; l < objects.size(); l++)
	{
		auto hit = objects[l]->CheckRayCollision(ray);

		if (hit.d >= 0.0f)
		{
			if (hit.d < closestD)
			{
				closestD = hit.d;
				closestHit = hit;
				closestHit.obj = objects[l];
			}
		}
	}

	return closestHit;
}
