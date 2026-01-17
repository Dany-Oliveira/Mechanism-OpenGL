#include "Pawn.h"
#include <iostream>
#include <cmath>

namespace Mechanism
{
	Pawn::Pawn(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex)
		: Actor(renderer, texturePath, x, y, gridColumns, gridRows, frameIndex)
	{
		
	}

	Pawn::~Pawn()
	{
		
	}

	void Pawn::MoveInDirection(float directionX, float directionY, float speed)
	{

		if(!HasPhysicsBody())
		{
			std::cerr << "Error: Cannot move pawn without a physics body.\n";
			return;
		}

		if (!GetPhysicsBody().IsValid())
		{
			std::cerr << "Error: Physics body is invalid in MoveInDirection().\n";
			return;
		}

		// Normalize the direction vector
		float length = sqrt(directionX * directionX + directionY * directionY);
		if (length > 0)
		{
			directionX /= length;
			directionY /= length;
		}

		//Apply velocity
		float velocityX = directionX * speed;
		float velocityY = directionY * speed;
		GetPhysicsBody().SetLinearVelocity(velocityX, velocityY);
	}

	void Pawn::Stop()
	{
		if(!HasPhysicsBody())
		{
			std::cerr << "Error: Cannot stop pawn without a physics body.\n";
			return;
		}

		if (!GetPhysicsBody().IsValid())
		{
			std::cerr << "Error: Physics body is invalid in Stop().\n";
			return;
		}

		GetPhysicsBody().SetLinearVelocity(0.0f, 0.0f);
	}
	

}