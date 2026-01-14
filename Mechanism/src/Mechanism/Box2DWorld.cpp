#include "Box2DWorld.h"
#include <box2d/box2d.h>
#include <iostream>


namespace Mechanism
{

	Box2DWorld::Box2DWorld(float gravityX, float gravityY): worldId_(nullptr)
	{

		// Create Box2D world
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2{ gravityX, gravityY };	

		// Create the world
		b2WorldId worldId = b2CreateWorld(&worldDef);
		worldId_ = new b2WorldId(worldId);

		std::cout << "Box2DWorld created\n";
	}

	Box2DWorld::~Box2DWorld()
	{
		if(worldId_)
		{
			// Destroy Box2D world
			b2WorldId* worldId = static_cast<b2WorldId*>(worldId_);

			if(b2World_IsValid(*worldId))
			{
				b2DestroyWorld(*worldId);		
			}
			delete worldId;
			worldId_ = nullptr;
			std::cout << "Box2DWorld destroyed\n";
		}
	}

	// Move constructor
	Box2DWorld::Box2DWorld(Box2DWorld&& other) noexcept : worldId_(other.worldId_)
	{
		other.worldId_ = nullptr;
	}

	// Move assignment operator
	Box2DWorld& Box2DWorld::operator=(Box2DWorld&& other) noexcept
	{
		if (this != &other)
		{
			if(worldId_)
			{
				b2WorldId* worldId = static_cast<b2WorldId*>(worldId_);
				if(b2World_IsValid(*worldId))
				{
					b2DestroyWorld(*worldId);		
				}
				delete worldId;
			}
			worldId_ = other.worldId_;
			other.worldId_ = nullptr;
		}
		return *this;
	}

	// Step the Box2D world simulation
	void Box2DWorld::Step(float timeStep, int subStepCount)
	{
		if(worldId_)
		{
			b2WorldId* worldId = static_cast<b2WorldId*>(worldId_);
			if(b2World_IsValid(*worldId))
			{
				b2World_Step(*worldId, timeStep, subStepCount);
			}
		}
	}

	// Get the Box2D world ID
	void* Box2DWorld::GetWorldId() const
	{
		return worldId_;
	}

}