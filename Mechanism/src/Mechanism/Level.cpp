#include "Level.h"

namespace Mechanism
{

	Level::Level(float gravityX, float gravityY)
	{
		m_Box2DWorld = std::make_unique<Box2DWorld>(gravityX, gravityY);
	}

	Level::~Level()
	{

	}

	void Level::Update(float deltaTime)
	{
		m_Accumulator += deltaTime;

		while (m_Accumulator >= m_TimeStep)
		{
			if(m_Box2DWorld)
			{
				m_Box2DWorld->Step(m_TimeStep, m_SubStepCount);
			}
			m_Accumulator -= m_TimeStep;
		}
	}

}