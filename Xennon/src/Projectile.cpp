#include "Projectile.h"
#include <iostream>


Projectile::Projectile(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex, int damage)
	: Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), velocityX(400.0f), m_Damage(damage)
{
		
}

Projectile::~Projectile()
{
		
}

void Projectile::UpdateProjectile(float deltaTime)
{
	if(HasPhysicsBody())
	{
		// Move the projectile to the right at a constant speed
		GetPhysicsBody().SetLinearVelocity(velocityX / 100.0f, 0.0f); // Divided by 100 to convert to Box2D units meters/second
	}
}

bool Projectile::IsOffScreen(float screenWidth) const
{
	return GetX() > screenWidth + 50.0f || GetX() < -50.0f;
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

