#include "Enemy.h"
#include <iostream>
#include "Spaceship.h"
#include "Projectile.h"

Enemy::Enemy(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex) 
	:Pawn(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), health(100), m_ShootCooldown(2.0f), m_ShootCooldownTime(0.0f)
{
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
		Projectile* projectile = dynamic_cast<Projectile*>(other);

		if(projectile)
		{
			int damage = projectile->GetDamage();
			TakeDamage(damage);
			printf("Enemy took %d damage! Health: %d\n", damage, health);
		}
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

std::function<void(Enemy*, float)> Enemy::LonerMovement()
{
	return [](Enemy* enemy, float direction)
		{
			if (enemy->HasPhysicsBody())
			{
				int phase = ((int)(enemy->GetTimeAlive() / 10.0f)) % 2;
				float direction = phase == 0 ? 1.0f : -1.0f; //1 right, -1 left
				enemy->MoveInDirection(direction, 0.0f, 1.0f);
			}
		};
}

std::function<void(Enemy*, float)> Enemy::RusherMovement()
{
	return [](Enemy* enemy, float direction)
		{
			if (enemy->HasPhysicsBody())
			{
				enemy->MoveInDirection(0.0f, 1.0f, 1.0f); //vertical only
			}
		};
}

std::function<void(Enemy*, float)> Enemy::AsteroidMovement()
{
	return [](Enemy* enemy, float direction)
		{
			if (enemy->HasPhysicsBody())
			{
				enemy->MoveInDirection(0.0f, 1.0f, 0.5f); //vertical only
			}
		};
}

std::function<void(Enemy*, float)> Enemy::DroneMovement()
{
	return [](Enemy* enemy, float deltaTime)
		{
			if (enemy->HasPhysicsBody())
			{
				float waveSpeed = 3.0f; // How fast the wave oscillates
				float waveAmplitude = 100.0f; // How wide the wave is
				float moveSpeed = 1.5f; // How fast it moves down

				// Calculate horizontal position using sine wave
				float timeAlive = enemy->GetTimeAlive();
				float waveOffset = sin(timeAlive * waveSpeed) * waveAmplitude;

				float targetX = enemy->GetStartX() + waveOffset;

				// Calculate direction to move horizontally
				float currentX = enemy->GetX();
				float directionX = (targetX > currentX) ? 1.0 : -1.0f;

				// Move in wave pattern while going down
				enemy->MoveInDirection(directionX, 1.0f, moveSpeed);
			}
		};
}


