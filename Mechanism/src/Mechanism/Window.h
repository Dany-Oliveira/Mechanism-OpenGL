#pragma once
#include "Core.h"
#include "Renderer.h"
#include <string>
#include <memory>

namespace Mechanism
{

	struct WindowSettings 
	{
		std::string windowTitle;
		unsigned int windowWidth;
		unsigned int windowHeight;

		WindowSettings(const std::string& title = "Mechanism Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			:windowTitle(title), windowWidth(width), windowHeight(height) {}

	};

	struct WindowData;

	class MECHANISM_API Window
	{

	public:

		Window(const WindowSettings& windowSettings = WindowSettings());

		~Window();

		void OnUpdate();

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		bool CloseWindow() const;

		bool IsVSync() const;
		void SetVSync(bool enabled);

		void* GetNativeWindow() const
		{
			return m_Window;
		}

		Renderer& GetRenderer() 
		{
			return *m_Renderer;
		}

	private:

		bool m_CloseWindow = false;
		bool Init(const WindowSettings& windowSettings);
		void Shutdown();

		void* m_Window;
		std::unique_ptr<Renderer> m_Renderer;
		WindowData* m_Data;
	};

}