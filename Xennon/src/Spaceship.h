 #pragma once
#include "Mechanism/Pawn.h"
#include <functional>

class Spaceship : public Mechanism::Pawn
{
public:

	Spaceship(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);

	~Spaceship();

	void PlayerUpdate(float deltaTime);

	void Shoot();

	void SetShootCallback(const std::function<void(float, float)>& callback)
	{
		m_ShootCallback = callback;
	}

private:

	float m_ShootCooldown; 
	float m_ShootCooldownTime;
	std::function<void(float, float)> m_ShootCallback;

};

