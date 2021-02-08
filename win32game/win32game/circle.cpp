#include "circle.h"

#include <Windows.h>

void Circle::draw(void* a_hdc, Vec2<float> center, float radius)
{
	HDC hdc = (HDC)a_hdc;
	Ellipse(hdc, (int)(center.x-radius), (int)(center.y-radius), 
	             (int)(center.x+radius), (int)(center.y+radius));
}

void Circle::draw(void* a_hdc)
{
	Circle::draw(a_hdc, center, radius);
}

bool Circle::overlaps(Circle & c)
{
	Vec2<float> delta = c.center - center;
	return delta.magnitude() < radius+c.radius;
}
