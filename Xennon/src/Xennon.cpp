#include <Mechanism.h>
#include "GameLevel.h"



class Xennon : public Mechanism::Application
{
	public:

		Xennon() : m_CurrentLevel(nullptr)
		{
			printf("Test App created\n");

			auto& window = GetWindow();
			printf("Window size: %d x %d\n", window.GetWidth(), window.GetHeight());

			m_CurrentLevel = std::make_unique<GameLevel>(window);
			
		}
		~Xennon()
		{
		}

		void OnUpdate(float deltaTime) override
		{
			if(m_CurrentLevel)
			{
				m_CurrentLevel->UpdateGameLevel(deltaTime);
			}
		}

		void OnRender() override
		{
			if (m_CurrentLevel)
			{
				m_CurrentLevel->Render();
			}
		}

	private:
		
		std::unique_ptr<GameLevel> m_CurrentLevel;
};

Mechanism::Application* Mechanism::CreateApplication()
{
	return new Xennon();
}