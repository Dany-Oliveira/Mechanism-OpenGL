#include "Application.h"
#include "Input.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>  

namespace Mechanism
{
	Application::Application()
	{
		m_Window = std::make_unique<Window>();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Uint64 lastTime = SDL_GetTicks();

		while (m_Running)
		{
			if (m_Window->CloseWindow())
			{
				m_Running = false;
				break;
			}

			// Calculate delta time
			Uint64 currentTime = SDL_GetTicks();
			float deltaTime = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;

			// Update input
			Input::Update();

			// Update window events
			m_Window->OnUpdate();

			OnUpdate(deltaTime);

			// Render - Clear with OpenGL
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OnRender();

			// Swap buffers
			m_Window->SwapBuffers();
		}
	}
}