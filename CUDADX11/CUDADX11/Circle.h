#pragma once
class Circle
{
private:
	Vec2 center;
	float radius;
	Color color;

	// 상수 버퍼 데이터
	CircleData circleData;

public:
	Circle(const Vec2& center, const float radius, const Color& color)
		: center(center), radius(radius), color(color)
	{}

	bool IsInside(const Vec2& x)
	{
		return false;
	}

};

