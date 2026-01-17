#pragma once
#include "Mechanism/Pawn.h"

class Enemy : public Mechanism::Pawn
{

public:
	Enemy(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);
	virtual ~Enemy();

	void SetHealth(int hp) { health = hp; }
	int GetHealth() const { return health; }	

	void TakeDamage(int damage);

	void OnCollisionBegin(Mechanism::Actor* other) override;

private:
	int health = 100;

};

