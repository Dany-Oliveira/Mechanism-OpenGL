#include "EnemyProjectile.h"
#include <iostream>

EnemyProjectile::EnemyProjectile(void* renderer, const char* texturePath, float x, float y, float targetX, float targetY, int gridColumns,
	int gridRows, int frameIndex) : Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), m_Speed(250.0f)
{
	//Find direction to target
	float directionX = targetX - x;
	float directionY = targetY - y;

	//Calculate distance
	float distance = sqrt(directionX * directionX + directionY * directionY);

	//Normalize the direction
	if(distance > 0.0f)
	{
		directionX /= distance;
		directionY /= distance;
	}

	velocityX = directionX * m_Speed;
	velocityY = directionY * m_Speed;

	std::cout << "Enemy Projectile created\n";
}

EnemyProjectile::~EnemyProjectile()
{
	std::cout << "Enemy Projectile destroyed\n";	
}

void EnemyProjectile::UpdateEnemyProjectile(float deltaTime)
{
	if(HasPhysicsBody())
	{
		// Move the projectile dowards at a constant speed
		GetPhysicsBody().SetLinearVelocity(velocityX / 100.0f, velocityY / 100); // Divided by 100 to convert to Box2D units meters/second
	}
}

bool EnemyProjectile::IsOffScreen(float screenHeight) const
{
	return GetY() < -50.0f || GetY() > screenHeight + 50.0f;
}

void EnemyProjectile::OnCollisionBegin(Mechanism::Actor* other)
{
	if (other && other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Player)
	{
		printf("Enemy projectile collided with the player!\n");
		SetIsDead(true);
	}
}
