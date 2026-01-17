#include "Box2DWorld.h"
#include "Actor.h"
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
	Box2DWorld::Box2DWorld(Box2DWorld&& other) noexcept : worldId_(other.worldId_), 
		m_CollisionBeginCallback(std::move(other.m_CollisionBeginCallback))
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
			m_CollisionBeginCallback = std::move(other.m_CollisionBeginCallback);
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

	void Box2DWorld::ProcessContactEvents()
	{
		// Ensure we have a valid world and callback
		if(!worldId_ || !m_CollisionBeginCallback)
		{
			return;
		}

		b2WorldId* worldId = static_cast<b2WorldId*>(worldId_);
		if(!b2World_IsValid(*worldId))
		{
			return;
		}

		//Ask Box2D for contact events in the last time step
		b2ContactEvents contactEvents = b2World_GetContactEvents(*worldId);

		// Process begin touch events
		for(int i = 0; i < contactEvents.beginCount; ++i)
		{
			const b2ContactBeginTouchEvent& beginEvent = contactEvents.beginEvents[i];

			//Get the two bodies that collided	
			b2BodyId bodyIdA = b2Shape_GetBody(beginEvent.shapeIdA);
			b2BodyId bodyIdB = b2Shape_GetBody(beginEvent.shapeIdB);

			//Get the Actor pointers from the user data
			void* userDataA = b2Body_GetUserData(bodyIdA);
			void* userDataB = b2Body_GetUserData(bodyIdB);

			//Convert to Actor pointers
			Actor* actorA = static_cast<Actor*>(userDataA);
			Actor* actorB = static_cast<Actor*>(userDataB);

			//Only call the callback if both actors are valid
			if(actorA && actorB)
			{
				m_CollisionBeginCallback(actorA, actorB);
			}
		}
	}


}