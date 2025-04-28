#include "pch.h"
#include "Triangle.h"

Hit Triangle::CheckRayCollision(Ray& ray)
{
	Hit hit = Hit{ -1.0, vec3(0.0), vec3(0.0) };

	vec3 point, faceNormal;
	float t, u, v;
	if (IntersectRayTriangle(ray.start, ray.dir, this->v0, this->v1,
		this->v2, point, faceNormal, t, u, v))
	{
		hit.d = t;
		hit.point = point; // ray.start + ray.dir * t;
		hit.normal = faceNormal;

		// 텍스춰링(texturing)에서 사용
		// hit.uv = uv0 * u + uv1 * v + uv2 * (1.0f - u - v);
	}

	return hit;
}

// 광선 시작점, 방향
bool Triangle::IntersectRayTriangle(const vec3& orig, const vec3& dir, const vec3& v0, const vec3& v1, const vec3& v2, vec3& point, vec3& faceNormal, float& t, float& u, float& v)
{
	
	// 1. 삼각형이 놓여 있는 평면의 수직 벡터 계산
	// 주의 : 삼각형이 잘 만들어졌다는 가정
	faceNormal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

	// 삼각형의 뒷면은 그리고 싶지 않는 방법 Backface Culling
	if (glm::dot(-dir, faceNormal) < 0.0f)
		return false;

	// 평면과 광선이 수평에 매우 가깝다면 충돟하지 못하는 것으로 판단.
	if (glm::abs(dot(dir, faceNormal)) < 1e-2f) return false; // t 계산시 0으로 나누기 방지

	// 2. 광선과 평면의 충돌 위치 계산
	t = (glm::dot(v0, faceNormal) - glm::dot(orig, faceNormal)) / (glm::dot(dir, faceNormal));

	// 광선의 시작점 이전에 충돌한다면 렌더링할 필요 없음
	if (t < 0.0f) return false;

	point = orig + t * dir;

	// 3. 그 충돌 위치가 삼각형 안에 들어 왔는 지 확인
	// 작은 삼각형들 3개의 normal 계산
	const vec3 normal0 = glm::normalize(glm::cross(point - v2, v1 - v2));
	const vec3 normal1 = glm::normalize(glm::cross(point - v0, v2 - v0));
	const vec3 normal2 = glm::normalize(glm::cross(point - v1, v0 - v1));

	// 방향만 확인하면 되기 때문에 normalize() 생략 가능
	// 아래에서 cross product의 절대값으로 작은 삼각형들의 넓이 계산

	if (dot(normal0, faceNormal) < 0.0f) return false;
	if (dot(normal1, faceNormal) < 0.0f) return false;
	if (dot(normal2, faceNormal) < 0.0f) return false;

	return true;
}
