#include "Spaceship.h"
#include "Mechanism/Input.h"
#include <iostream>

Spaceship::Spaceship(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex) :
	Pawn(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex), 
	m_ShootCooldown(0.0f), m_ShootCooldownTime(0.2f), m_MaxHealth(10), m_CurrentHealth(10), m_ProjectileDamage(5), m_WeaponPowerUpLevel(0)
{
	SetSpeed(5.0f); // Set a default speed for the spaceship

	
}

Spaceship::~Spaceship()
{
	std::cout << "Spaceship destroyed\n";
}

void Spaceship::PlayerUpdate(float deltaTime)
{
	if(!HasPhysicsBody())
	{
		std::cerr << "Error: Cannot update spaceship without a physics body.\n";
		return;
	}

	//Update cooldown timer
	if(m_ShootCooldown > 0.0f)
	{
		m_ShootCooldown -= deltaTime;
	}

	float directionX = 0.0f;
	float directionY = 0.0f;

	
	// Input handling
	if(Mechanism::Input::IsKeyBeingPressed(Mechanism::Input::KEY_UP))
	{
		directionY = -1.0f;
	}
	if(Mechanism::Input::IsKeyBeingPressed(Mechanism::Input::KEY_DOWN))
	{
		directionY = 1.0f;
	}
	if(Mechanism::Input::IsKeyBeingPressed(Mechanism::Input::KEY_LEFT))
	{
		directionX = -1.0f;
	}
	if(Mechanism::Input::IsKeyBeingPressed(Mechanism::Input::KEY_RIGHT))
	{
		directionX = 1.0f;
	}

	// Movement
	if(directionX != 0.0f || directionY != 0.0f)
	{
		MoveInDirection(directionX, directionY, GetSpeed());
	}
	else
	{
		Stop();
	}

	// Shooting
	if(Mechanism::Input::IsKeyPressed(Mechanism::Input::KEY_SPACE) && m_ShootCooldown <= 0.0f)
	{
		Shoot();
		m_ShootCooldown = m_ShootCooldownTime;
	}

}

void Spaceship::Shoot()
{
	if(m_ShootCallback)
	{
		float bulletX = GetX() + GetFrameWidth(); 
		float bulletY = GetY() + (GetFrameWidth() / 2.0f); 
		int damage = m_ProjectileDamage;

		m_ShootCallback(bulletX, bulletY, damage);
	}
}

void Spaceship::OnCollisionBegin(Mechanism::Actor* other)
{
	if(other->GetCollisionTag() == Mechanism::Actor::CollisionTag::EnemyProjectile)
	{
		TakeDamage(1); // Take damage from enemy projectile
	}
	else if(other->GetCollisionTag() == Mechanism::Actor::CollisionTag::Enemy)
	{
		TakeDamage(2); // Take damage from enemy collisions
	}
}

void Spaceship::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;

	if(m_CurrentHealth < 0)
	{
		m_CurrentHealth = 0;
	}

	printf("Player took %d damage! Health: %d/%d\n", damage, m_CurrentHealth, m_MaxHealth);

	if (m_CurrentHealth <= 0)
	{
		printf("Player died!\n");
		SetIsDead(true);	
	}
}

void Spaceship::ApplyShieldPowerUp()
{
	m_CurrentHealth = m_MaxHealth;
	printf("Shield power-up! Health restored to %d\n", m_MaxHealth);
}

void Spaceship::ApplyWeaponPowerUp()
{
	if(m_WeaponPowerUpLevel >= 2)
	{
		printf("Weapon power-up already at max level!\n");
		return;
	}
	m_ProjectileDamage *= 2; 
	++m_WeaponPowerUpLevel;
}






