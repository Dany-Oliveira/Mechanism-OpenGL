#include "Spaceship.h"
#include "Mechanism/Input.h"
#include <iostream>

Spaceship::Spaceship(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex) :
	Pawn(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex)
{
	SetSpeed(5.0f); // Set a default speed for the spaceship
	std::cout << "Spaceship created\n";
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
	if(Mechanism::Input::IsKeyPressed(Mechanism::Input::KEY_SPACE))
	{
		Shoot();
	}

}

void Spaceship::Shoot()
{
	//To do: implement shooting logic
	std::cout << "Pew! Pew! Shooting!\n";
}




