#include "Enemy.h"
#include <iostream>

Enemy::Enemy(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex) 
	:Pawn(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), health(100), m_ShootCooldown(2.0f), m_ShootCooldownTime(0.0f)
{
	// Initialize spawn position
	startX = x;
	startY = y;
}

Enemy::~Enemy()
{
	
}

void Enemy::TakeDamage(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		if(m_DeathCallback)
		{
			m_DeathCallback(GetX(), GetY(), m_EnemyType);
		}

		SetIsDead(true);
	}
}

void Enemy::OnCollisionBegin(Mechanism::Actor* other)
{
	if(other && other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Projectile)
	{
		TakeDamage(50); // Example damage value
	}
}


void Enemy::UpdateEnemy(float deltaTime)
{
	timeAlive += deltaTime;

	if(m_MovementPattern)
	{
		m_MovementPattern(this, deltaTime);
	}

	if(m_CanShoot && m_EnemyShootCallback)
	{
		m_ShootCooldownTime += deltaTime;

		if(m_ShootCooldownTime >= m_ShootCooldown)
		{
			float bulletX = GetX() + (GetFrameWidth() / 2.0f);
			float bulletY = GetY() + GetFrameHeight();

			m_EnemyShootCallback(bulletX, bulletY, m_PlayerX, m_PlayerY);

			m_ShootCooldownTime = 0.0f;
		}
	}
}





