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
	// 충돌 판정
	const Hit hit = sphere->IntersectRayCollision(ray);

	if (hit.d < 0.0f)
	{
		return glm::vec3(0.0f);
	}
	else
	{
		// 여기서 퐁 모델(Phong reflection model)으로 조명 효과 계산!
		// 참고 자료
		// https://en.wikipedia.org/wiki/Phong_reflection_model
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF

		// Diffuse
		const glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
		const float diff = glm::max(glm::dot(hit.normal, dirToLight), 0.0f);

		// Specular
		const glm::vec3 reflectDir = 2.0f * dot(hit.normal, dirToLight) * hit.normal - dirToLight;// r = 2 (n dot l) n - l
		const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), sphere->alpha);

		return sphere->amb + sphere->diff * diff + sphere->spec * specular * sphere->ks;//sphere->color * hit.d; // 깊이를 곱해서 입체감 만들기
	}
}

void Raytracer::Render(vector<glm::vec4>& pixels)
{
	std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			// 2D를 3D로 투영 반환
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));

			// 광선의 방향 벡터
			// 스크린에 수직인 z방향, 절대값 1.0인 유닉 벡터
			// Orthographic projection (정투영) vs perspective projection (원근투영)
			const auto rayDir = glm::vec3(0.0f, 0.0f, 1.0f);

			Ray pixelRay{ pixelPosWorld, rayDir };

			// index에는 size_t형 사용 (index가 음수일 수는 없으니까)
			// traceRay()의 반환형은 vec3 (RGB), A는 불필요
			pixels[size_t(i + width * j)] = glm::vec4(traceRay(pixelRay), 1.0f);
		}
}
