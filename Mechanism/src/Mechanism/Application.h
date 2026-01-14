#pragma once
#include "Core.h"
#include "Window.h"
#include "Texture.h"
#include <memory>
#include <stdexcept>


namespace Mechanism
{
	class MECHANISM_API Application
	{

	public:

		Application();
		virtual ~Application();

		//Keep the application running
		void Run();

		virtual void OnRender() {}

		virtual void OnUpdate(float deltaTime) {}

		inline Window& GetWindow()
		{
			if(!m_Window)
			{
				throw std::runtime_error("Window not initialized");
			}
			return *m_Window;
		}


	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};


	//To be defined in CLIENT
	Application* CreateApplication();
}



