#pragma once

#include "vec2.h"

class GameObject
{
public:
	virtual void init(Vec2<float> position, float radius, int color) = 0;
	virtual void draw(void* hdc) = 0;
	virtual void update(float secondsPassed) = 0;
	virtual bool collidesWith(GameObject * e) = 0;

	virtual void setPosition(Vec2<float> position) = 0;
	virtual Vec2<float> * getPosition() = 0;

	virtual void setColor(int color) = 0;
	virtual int getColor() = 0;
};
