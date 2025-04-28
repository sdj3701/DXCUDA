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

		// �ؽ��縵(texturing)���� ���
		// hit.uv = uv0 * u + uv1 * v + uv2 * (1.0f - u - v);
	}

	return hit;
}

// ���� ������, ����
bool Triangle::IntersectRayTriangle(const vec3& orig, const vec3& dir, const vec3& v0, const vec3& v1, const vec3& v2, vec3& point, vec3& faceNormal, float& t, float& u, float& v)
{
	
	// 1. �ﰢ���� ���� �ִ� ����� ���� ���� ���
	// ���� : �ﰢ���� �� ��������ٴ� ����
	faceNormal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

	// �ﰢ���� �޸��� �׸��� ���� �ʴ� ��� Backface Culling
	if (glm::dot(-dir, faceNormal) < 0.0f)
		return false;

	// ���� ������ ���� �ſ� �����ٸ� �扥���� ���ϴ� ������ �Ǵ�.
	if (glm::abs(dot(dir, faceNormal)) < 1e-2f) return false; // t ���� 0���� ������ ����

	// 2. ������ ����� �浹 ��ġ ���
	t = (glm::dot(v0, faceNormal) - glm::dot(orig, faceNormal)) / (glm::dot(dir, faceNormal));

	// ������ ������ ������ �浹�Ѵٸ� �������� �ʿ� ����
	if (t < 0.0f) return false;

	point = orig + t * dir;

	// 3. �� �浹 ��ġ�� �ﰢ�� �ȿ� ��� �Դ� �� Ȯ��
	// ���� �ﰢ���� 3���� normal ���
	const vec3 normal0 = glm::normalize(glm::cross(point - v2, v1 - v2));
	const vec3 normal1 = glm::normalize(glm::cross(point - v0, v2 - v0));
	const vec3 normal2 = glm::normalize(glm::cross(point - v1, v0 - v1));

	// ���⸸ Ȯ���ϸ� �Ǳ� ������ normalize() ���� ����
	// �Ʒ����� cross product�� ���밪���� ���� �ﰢ������ ���� ���

	if (dot(normal0, faceNormal) < 0.0f) return false;
	if (dot(normal1, faceNormal) < 0.0f) return false;
	if (dot(normal2, faceNormal) < 0.0f) return false;

	return true;
}
