#pragma once
#include "Mechanism/Pawn.h"
#include <functional>

class Enemy : public Mechanism::Pawn
{

public:

	enum class EnemyType
	{
		None = 0,
		Loner,
		Rusher,
		Drone,
		BigStoneAsteroid,
		MediumStoneAsteroid,
		SmallStoneAsteroid,
		BigMetalAsteroid,
		MediumMetalAsteroid,
		SmallMetalAsteroid
	};

	Enemy(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);
	virtual ~Enemy();

	void SetHealth(int hp) 
	{ 
		health = hp; 
	}

	int GetHealth() const { return health; }

	void TakeDamage(int damage);

	void UpdateEnemy(float deltaTime);

	void OnCollisionBegin(Mechanism::Actor* other) override;

	float GetTimeAlive() const { return timeAlive; }
	float GetStartX() const { return startX; }
	float GetStartY() const { return startY; }

	bool IsOffScreen(float screenWidth) const;

	void SetMovementPattern(std::function<void(Enemy*, float)> pattern)
	{
		m_MovementPattern = pattern;
	}
	
	void SetEnemyShootCallback(const std::function<void(float, float, float, float)>& callback)
	{
		m_EnemyShootCallback = callback;
	}

	void SetDeathCallback(std::function<void(float, float, EnemyType)> callback)
	{
		m_DeathCallback = callback;
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

	void SetEnemyType(EnemyType type) 
	{
		m_EnemyType = type;
	}

	void SetScreenBounds(float top, float bottom)
	{
		m_ScreenTop = top;
		m_ScreenBottom = bottom;
	}

	float GetScreenTop() const { return m_ScreenTop; }
	float GetScreenBottom() const { return m_ScreenBottom; }
	float GetDirectionY() const { return m_DirectionY; }
	void SetDirectionY(float dir) { m_DirectionY = dir; }

	EnemyType GetEnemyType() const { return m_EnemyType; }

	static std::function<void(Enemy*, float)> LonerMovement();
	static std::function<void(Enemy*, float)> RusherMovement();
	static std::function<void(Enemy*, float)> DroneMovement();
	static std::function<void(Enemy*, float)> AsteroidMovement();


private:

	int health = 100;

	std::function<void(Enemy*, float)> m_MovementPattern;

	float timeAlive = 0.0f; //Time since spawned
	float startX = 0.0f; //Initial x spawn position
	float startY = 0.0f; //Initial y spawn position

	float m_ScreenTop = 0.0f;
	float m_ScreenBottom = 0.0f;
	float m_DirectionY = 1.0f;

	bool m_CanShoot = false;
	float m_ShootCooldown = 2.0f;
	float m_ShootCooldownTime = 0.0f;

	std::function<void(float, float, float, float)> m_EnemyShootCallback;
	std::function<void(float, float, EnemyType)> m_DeathCallback;

	float m_PlayerX = 0.0f;
	float m_PlayerY = 0.0f;

	EnemyType m_EnemyType = EnemyType::None;
};

