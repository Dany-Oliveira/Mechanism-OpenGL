#pragma once

#include "Core.h"
#include "Box2DWorld.h"
#include <memory>

namespace Mechanism
{
	class MECHANISM_API Level
	{

	public:
		Level(float gravityX = 0.0f, float gravityY = 0.0f);
		virtual ~Level();

		virtual void Update(float deltaTime);
		virtual void Render() = 0;

		Box2DWorld& GetBox2DWorld() 
		{ 
			return *m_Box2DWorld;
		}

	protected:
		std::unique_ptr<Box2DWorld> m_Box2DWorld;

	private:

		float m_Accumulator = 0.0f;
		const float m_TimeStep = 1.0f / 60.0f;
		const int m_SubStepCount = 4;

	};
}