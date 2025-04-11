#pragma once

class Circle
{
public:
	glm::vec2 center;
	float radius;
	glm::vec4 _color;

	// ��� ���� ������
	CircleData circleData;

public:
	Circle(const glm::vec2& center, const float radius, const glm::vec4& color)
		: center(center), radius(radius), _color(color)
	{
		// ��� ���� ������ �ʱ�ȭ
		circleData.center = center;
		circleData.radius = radius;
		circleData.padding = 0.0f;
		circleData.color = color;
	}
	
	void DrawCircle(int width,int height , ComPtr<ID3D11ShaderResourceView>& _shaderResourceView, ComPtr<ID3D11DeviceContext>& _deviceContext, ComPtr<ID3D11Buffer>& constantBuffer);
	bool IsInside(const glm::vec2& x);
};