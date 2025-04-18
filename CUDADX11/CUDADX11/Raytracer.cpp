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
	// Render first hit
	const auto hit = FindClosestCollision(ray);

	if (hit.d >= 0.0f)
	{
		glm::vec3 color(hit.obj->amb);

		// 여기서 퐁 모델(Phong reflection model)으로 조명 효과 계산!
		// 참고 자료
		// https://en.wikipedia.org/wiki/Phong_reflection_model
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF
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

	const glm::vec3 eyePos(0.0f, 0.0f, -1.5f);

	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			// 2D를 3D로 투영 반환
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));

			// 광선의 방향 벡터
			// 스크린에 수직인 z방향, 절대값 1.0인 유닉 벡터
			// Orthographic projection (정투영) vs perspective projection (원근투영)
			//const auto rayDir = glm::vec3(0.0f, 0.0f, 1.0f);
			Ray pixelRay{ pixelPosWorld, normalize(pixelPosWorld - eyePos) };

			// index에는 size_t형 사용 (index가 음수일 수는 없으니까)
			// traceRay()의 반환형은 vec3 (RGB), A는 불필요
			pixels[i + width * j] = vec4(glm::clamp(traceRay(pixelRay), 0.0f, 1.0f), 1.0f);
		}
}

Hit Raytracer::FindClosestCollision(Ray& ray)
{
	float closestD = 1000.0;
	Hit closesthit = Hit{ -1.0, glm::dvec3(0.0), glm::dvec3(0.0) };

	for (int l = 0; l < objects.size(); l++)
	{
		auto hit = objects[l]->CheckRayCollision(ray);

		// 충돌했는가?
		if (hit.d >= 0.0f)
		{
			// 충돌한것중에 가까운것
			if (hit.d < closestD)
			{
				closestD = hit.d;
				// 가장 가까운 것을 저장
				closesthit = hit;
				closesthit.obj = objects[l];
			}
		}
	}

	return closesthit;
}
