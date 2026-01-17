#pragma once
#include "Core.h"
#include <memory>
#include <functional>

namespace Mechanism
{
	class Actor;

	class MECHANISM_API Box2DWorld
	{
		public:

			explicit Box2DWorld(float gravityX = 0.0f, float gravityY = -10.0f);
			~Box2DWorld();

			// Prevent copying
			Box2DWorld(const Box2DWorld&) = delete;
			Box2DWorld& operator=(const Box2DWorld&) = delete;

			// Allow moving
			Box2DWorld(Box2DWorld&& other) noexcept;
			Box2DWorld& operator=(Box2DWorld&& other) noexcept;

			void Step(float timeStep, int subStepCount = 4);

			void* GetWorldId() const;

			void SetCollisionBeginCallback(std::function<void(Actor*, Actor*)> callback)
			{
				m_CollisionBeginCallback = callback;
			}

			void ProcessContactEvents();

		private:

			void* worldId_;

			std::function<void(Actor*, Actor*)> m_CollisionBeginCallback;
	};

}



