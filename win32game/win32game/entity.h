#pragma once
#include "circle.h"
#include "gameobject.h"

/** a basic concrete GameObject */
class Entity : public GameObject
{
protected:
	Circle area;
	int color;
public:
	Entity(){}
	Entity(Vec2<float> center, float radius, int color)
	{
		init(center, radius, color);
	}

	void init(Vec2<float> position, float radius, int color);
	void draw(void* hdc);
	void update(float secondsPassed){}
	bool collidesWith(GameObject * e);

	void setPosition(Vec2<float> position){area.center = position;}
	Vec2<float> * getPosition(){return &(this->area.center);}

	void setColor(int color){this->color = color;}
	int getColor(){return color;}
};
