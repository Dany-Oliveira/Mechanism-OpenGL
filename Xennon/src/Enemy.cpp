#include "Enemy.h"
#include <iostream>

Enemy::Enemy(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex) 
	:Pawn(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), health(100)
{
	
}

Enemy::~Enemy()
{
	std::cout << "Enemy destroyed.\n";
}

void Enemy::TakeDamage(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		SetIsDead(true);
	}
}

void Enemy::OnCollisionBegin(Mechanism::Actor* other)
{
	if(other && other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Projectile)
	{
		TakeDamage(100); // Example damage value
	}
}
