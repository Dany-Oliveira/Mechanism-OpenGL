#include "Projectile.h"
#include <iostream>


Projectile::Projectile(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex)
	: Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), velocityY(-400.0f)
{
		std::cout << "Projectile created\n";
}

Projectile::~Projectile()
{
		std::cout << "Projectile destroyed\n";
}

void Projectile::UpdateProjectile(float deltaTime)
{
	if(HasPhysicsBody())
	{
		// Move the projectile upwards at a constant speed
		GetPhysicsBody().SetLinearVelocity(0.0f, velocityY / 100.0f); // Divided by 100 to convert to Box2D units meters/second
	}
}

bool Projectile::IsOffScreen(float screenHeight) const
{
	return GetY() < -50.0f || GetY() > screenHeight + 50.0f;
}

void Projectile::OnCollisionBegin(Mechanism::Actor* other)
{
	if(other && other->GetCollisionTag()==Mechanism::Actor::CollisionTag::Enemy)
	{
		printf("Projectile collided with an enemy!\n");
		SetIsDead(true);
	}
}

