#pragma once

#include "vec2.h"

class Circle
{
public:
	Vec2<float> center;
	float radius;

	Circle():radius(0){}
	Circle(Vec2<float> center, float radius):center(center),radius(radius){}

	static void draw(void* hdc, Vec2<float> center, float radius);
	void draw(void* hdc);
	bool overlaps(Circle & c);
};
