#include "PowerUps.h"

PowerUps::PowerUps(void* renderer, const char* texturePath, float x, float y, 
	int gridColumns, int gridRows, PowerUpType type, int frameIndex)
	: Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), velocityY(100.0f), m_PowerUpType(type)
{

}

PowerUps::~PowerUps()
{
}

void PowerUps::UpdatePowerUp(float deltaTime)
{
	if(HasPhysicsBody())
	{
		GetPhysicsBody().SetLinearVelocity(0.0f, velocityY / 100.0f); // Divided by 100 to convert to Box2D units meters/second
	}
}

bool PowerUps::IsOffScreen(float screenHeight) const
{
	return GetY() < -50.0f || GetY() > screenHeight + 50.0f;
}

void PowerUps::OnCollisionBegin(Mechanism::Actor* other)
{
	if (other && other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Player)
	{
		if(m_EffectCallback)
		{
			m_EffectCallback(m_PowerUpType);
		}
		SetIsDead(true);
	}
}
