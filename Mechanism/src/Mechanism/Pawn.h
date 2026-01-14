#pragma once
#include "Core.h"
#include "Actor.h"

namespace Mechanism
{
	class MECHANISM_API Pawn : public Mechanism::Actor
	{

	public:

		Pawn(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex = 0);

		virtual ~Pawn();

		// Override this method to implement custom pawn behavior
		virtual void UpdatePawn(float deltaTime) {};

		// Move the pawn in a specified direction with a given speed
		void MoveInDirection(float directionX, float directionY, float speed);
		// Stop the pawn's movement
		void Stop();

		void SetSpeed(float sp) { speed = sp; }
		float GetSpeed() const { return speed; }

		bool IsDead() const { return isDead; }
		void SetIsDead(bool dead) { isDead = dead; }

	protected:

			float speed;
			bool isDead = false;

	};
}