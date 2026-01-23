#pragma once
#include "Mechanism/Pawn.h"
#include <functional>

class Enemy : public Mechanism::Pawn
{

public:

	Enemy(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);
	virtual ~Enemy();

	void SetHealth(int hp) { health = hp; }
	int GetHealth() const { return health; }

	void TakeDamage(int damage);

	void UpdateEnemy(float deltaTime);

	void OnCollisionBegin(Mechanism::Actor* other) override;

	void SetMovementPattern(std::function<void(Enemy*, float)> pattern)
	{
		m_MovementPattern = pattern;
	}

	float GetTimeAlive() const { return timeAlive; }
	float GetStartX() const { return startX; }
	float GetStartY() const { return startY; }
	
	void SetEnemyShootCallback(const std::function<void(float, float, float, float)>& callback)
	{
		m_EnemyShootCallback = callback;
	}

	void SetCanShoot(bool canShoot) 
	{
		m_CanShoot = canShoot;
	}

	void SetShootInterval(float cooldown)
	{
		m_ShootCooldown = cooldown;
	}

	void SetPlayerPosition(float x, float y)
	{
		m_PlayerX = x;
		m_PlayerY = y;
	}

private:

	int health = 100;

	std::function<void(Enemy*, float)> m_MovementPattern;

	float timeAlive = 0.0f; //Time since spawned
	float startX = 0.0f; //Initial x spawn position
	float startY = 0.0f; //Initial y spawn position

	bool m_CanShoot = false;
	float m_ShootCooldown = 2.0f;
	float m_ShootCooldownTime = 0.0f;

	std::function<void(float, float, float, float)> m_EnemyShootCallback;

	float m_PlayerX = 0.0f;
	float m_PlayerY = 0.0f;
};

