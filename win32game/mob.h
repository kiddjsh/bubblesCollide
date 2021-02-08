#pragma once

#include "entity.h"

/** [Mob]ile Entity */
class MOB : public Entity
{
	Vec2<float> velocity; // how much to move per second
	float maxSpeed;   // what to limit speed to
public:
	MOB(){}
	virtual Vec2<float> getVelocity(){ return velocity; }
	virtual void setVelocity(Vec2<float> v)
	{
		velocity = v;
		velocity.limitMagnitude(maxSpeed);
	}
	virtual void addVelocity(Vec2<float> v)
	{
		velocity += v;
		velocity.limitMagnitude(maxSpeed);
	}
	void update(float secondsPassed)
	{
		this->area.center += (velocity * secondsPassed);
	}
	void draw(void* hdc);
	void applyStoppingForce(float force);
	void setMaxSpeed(float maxSpeed){ this->maxSpeed = maxSpeed; }
	float getMaxSpeed(){ return maxSpeed; }
};
