#pragma once
#include "Mechanism/Pawn.h"

class Spaceship : public Mechanism::Pawn
{
public:

	Spaceship(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);

	~Spaceship();

	void PlayerUpdate(float deltaTime);

	void Shoot();

private:

	float boundaryX = 1280;
	float boundaryY = 720;

};

