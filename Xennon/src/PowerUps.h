#pragma once
#include "Mechanism/Actor.h"
#include <functional>

class PowerUps : public Mechanism::Actor
{
public:

	enum class PowerUpType
	{
		None = 0,
		Weapon,
		Shield
	};

	PowerUps(void* renderer, const char* texturePath, float x, float y, int gridColumns, 
		int gridRows, PowerUpType type, int frameIndex = 0);
	virtual ~PowerUps();

	void UpdatePowerUp(float deltaTime);	

	void OnCollisionBegin(Mechanism::Actor* other) override;

	bool IsOffScreen(float screenHeight) const;

	void SetEffectCallback(std::function<void(PowerUpType)> callback)
	{
		m_EffectCallback = callback;
	}

private:

	float velocityY;

	std::function<void(PowerUpType)> m_EffectCallback;

	PowerUpType m_PowerUpType = PowerUpType::None;
};

