#include "Box2DBody.h"
#include <box2d/box2d.h>
#include <iostream>

namespace Mechanism
{

	Box2DBody::Box2DBody() : bodyId_(nullptr)
	{

	}

	Box2DBody::~Box2DBody()
	{
		if (bodyId_)
		{
			// Destroy Box2D body
			b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
			if (b2Body_IsValid(*bodyId))
			{
				b2DestroyBody(*bodyId);
			}
			delete bodyId;
			bodyId_ = nullptr;
		}
	}

	// Move constructor
	Box2DBody::Box2DBody(Box2DBody&& other) noexcept : bodyId_(other.bodyId_)
	{
		other.bodyId_ = nullptr;
	}

	// Move assignment operator
	Box2DBody& Box2DBody::operator=(Box2DBody&& other) noexcept
	{
		if (this != &other)
		{
			if (bodyId_)
			{
				b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
				if (b2Body_IsValid(*bodyId))
				{
					b2DestroyBody(*bodyId);
				}
				delete bodyId;
			}
			bodyId_ = other.bodyId_;
			other.bodyId_ = nullptr;
		}
		return *this;
	}

	void Box2DBody::CreateDynamic(void* worldId, float x, float y, bool isBullet)
	{
		if (!worldId)
		{
			return;
		}

		// Define body
		b2WorldId* world = static_cast<b2WorldId*>(worldId);

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = b2Vec2{ x, y };
		bodyDef.isBullet = isBullet;

		if (isBullet)
		{
			bodyDef.enableSleep = false;
		}

		// Create dynamic body
		b2BodyId bodyId = b2CreateBody(*world, &bodyDef);
		bodyId_ = new b2BodyId(bodyId);
	}


	void Box2DBody::CreateStatic(void* worldId, float x, float y)
	{
		if (!worldId)
		{
			return;
		}

		// Define body
		b2WorldId* world = static_cast<b2WorldId*>(worldId);

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2{ x, y };

		// Create static body
		b2BodyId bodyId = b2CreateBody(*world, &bodyDef);
		bodyId_ = new b2BodyId(bodyId);
	}


	void Box2DBody::AddBoxShape(float halfWidth, float halfHeight, float density, float friction)
	{
		if (!IsValid())
		{
			std::cerr << "Error: Attempted to add shape to invalid Box2DBody." << std::endl;
			return;
		}

		// Define shape

		b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = density;
		shapeDef.material.friction = friction;
		shapeDef.enableContactEvents = true;

		// Create box shape
		b2Polygon box = b2MakeBox(halfWidth, halfHeight);
		b2CreatePolygonShape(*bodyId, &shapeDef, &box);
	}

	void Box2DBody::SetLinearVelocity(float vx, float vy)
	{
		if (IsValid())
		{
			b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
			b2Vec2 velocity = b2Vec2{ vx, vy };
			b2Body_SetLinearVelocity(*bodyId, velocity);
		}
	}

	void Box2DBody::GetPosition(float& x, float& y) const
	{

		if (IsValid())
		{
			b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
			b2Vec2 position = b2Body_GetPosition(*bodyId);
			x = position.x;
			y = position.y;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
		}
	}

	void Box2DBody::SetUserData(void* data)
	{
		if(IsValid())
		{
			b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
			b2Body_SetUserData(*bodyId, data);
		}
	}

	void* Box2DBody::GetUserData() const
	{
		if(IsValid())
		{
			b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
			return b2Body_GetUserData(*bodyId);
		}
		return nullptr;
	}

	bool Box2DBody::IsValid() const
	{
		if (!bodyId_)
		{
			return false;
		}

		b2BodyId* bodyId = static_cast<b2BodyId*>(bodyId_);
		return b2Body_IsValid(*bodyId);
	}

}