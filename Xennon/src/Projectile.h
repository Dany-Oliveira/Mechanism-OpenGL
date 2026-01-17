#pragma once
#include "Mechanism/Actor.h"


class Projectile : public Mechanism::Actor
{
public:

	Projectile(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);
	virtual ~Projectile();
	
	void UpdateProjectile(float deltaTime);

	bool IsOffScreen(float screenHeight) const;

	void OnCollisionBegin(Mechanism::Actor* other) override;

private:

	float velocityY;
};

