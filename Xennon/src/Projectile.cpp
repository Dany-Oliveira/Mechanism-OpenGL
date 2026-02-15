#include "Projectile.h"
#include <iostream>


Projectile::Projectile(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex, int damage)
	: Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), velocityY(-400.0f), m_Damage(damage)
{
		
}

Projectile::~Projectile()
{
		
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
		if (m_ExplosionCallback)
		{
			m_ExplosionCallback(GetX(), GetY());
		}
		SetIsDead(true);
	}
}

