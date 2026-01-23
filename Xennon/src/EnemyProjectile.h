#pragma once
#include "Mechanism/Actor.h"

class EnemyProjectile : public Mechanism::Actor
{

public:

	EnemyProjectile(void* renderer, const char* texturePath, float x, float y, float targetX, float targetY,
		int gridColumns, int gridRows, int frameIndex = 0);

	virtual ~EnemyProjectile();

	void UpdateEnemyProjectile(float deltaTime);

	bool IsOffScreen(float screenHeight) const;

	void OnCollisionBegin(Mechanism::Actor* other) override;
 
private:

	float velocityY;
	float velocityX;
	float m_Speed;

};

