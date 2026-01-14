#pragma once
#include "Mechanism/Actor.h"

class Projectile : public Mechanism::Actor
{
public:

	void BulletUpdate(float deltaTime);

private:

	float velocity;
};

