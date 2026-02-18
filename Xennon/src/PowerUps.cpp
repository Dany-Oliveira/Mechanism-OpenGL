#include "PowerUps.h"

PowerUps::PowerUps(void* renderer, const char* texturePath, float x, float y, 
	int gridColumns, int gridRows, PowerUpType type, int frameIndex)
	: Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), velocityX(-100.0f), m_PowerUpType(type)
{

}

PowerUps::~PowerUps()
{
}

void PowerUps::UpdatePowerUp(float deltaTime)
{
	if(HasPhysicsBody())
	{
		GetPhysicsBody().SetLinearVelocity(velocityX / 100.0f, 0.0f); // Divided by 100 to convert to Box2D units meters/second
	}
}

bool PowerUps::IsOffScreen(float screenHeight) const
{
	return GetY() < -50.0f || GetY() > screenHeight + 50.0f;
}

void PowerUps::OnCollisionBegin(Mechanism::Actor* other)
{
	if (other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Projectile)
	{
		return;  
	}

	if (other && other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Player)
	{
		if(m_EffectCallback)
		{
			m_EffectCallback(m_PowerUpType);
		}
		SetIsDead(true);
	}
}
