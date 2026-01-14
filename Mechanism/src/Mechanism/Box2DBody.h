#pragma once
#include "Core.h"


namespace Mechanism
{

	class MECHANISM_API Box2DBody
	{
	public:

		Box2DBody();
		~Box2DBody();

		//Prevent copying 
		Box2DBody(const Box2DBody&) = delete;
		Box2DBody& operator = (const Box2DBody&) = delete;

		//Allow moving
		Box2DBody(Box2DBody&& other) noexcept;
		Box2DBody& operator = (Box2DBody&& other) noexcept;

		//Create body
		void CreateDynamic(void* worldId, float x, float y, bool isBullet = false);
		void CreateStatic(void* worldId, float x, float y);

		//Add shape
		void AddBoxShape(float halfWidth, float halfHeight, float density = 1.0f, float friction = 0.0f);

		//Physics operations
		void SetLinearVelocity(float vx, float vy);
		void GetPosition(float& x, float& y) const;

		//Accessor
		bool IsValid() const;

	private:

		void* bodyId_;
	};

}