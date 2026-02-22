 #pragma once
#include "Mechanism/Pawn.h"
#include "Mechanism/HealthBar.h"

#include <functional>

class Spaceship : public Mechanism::Pawn
{
public:

	Spaceship(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);

	~Spaceship();

	void PlayerUpdate(float deltaTime);

	void Shoot();

	void SetShootCallback(const std::function<void(float, float, int)>& callback)
	{
		m_ShootCallback = callback;
	}

	void TakeDamage(int damage);
	void OnCollisionBegin(Mechanism::Actor* other) override;

	void ApplyShieldPowerUp();
	void ApplyWeaponPowerUp();

	void SetShootCooldownTime(float time) { m_ShootCooldownTime = time; }
	float GetShootCooldownTime() const { return m_ShootCooldownTime; }

	int GetCurrentHealth() const { return m_CurrentHealth; }
	int GetMaxHealth() const { return m_MaxHealth; }

private:

	int m_MaxHealth;
	int m_CurrentHealth;
	int m_ProjectileDamage;
	int m_WeaponPowerUpLevel;

	float m_ShootCooldown; 
	float m_ShootCooldownTime;
	std::function<void(float, float, int)> m_ShootCallback;

	int m_CurrentFrame = 3;

};

