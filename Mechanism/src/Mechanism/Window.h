#pragma once
#include "Core.h"
#include "SpriteRenderer.h"
#include <string>

struct SDL_Window;

namespace Mechanism
{
	struct WindowSettings
	{
		std::string windowTitle;
		unsigned int windowWidth;
		unsigned int windowHeight;

		WindowSettings(const std::string& title = "Mechanism Engine",
			unsigned int width = 1600,
			unsigned int height = 900)
			: windowTitle(title), windowWidth(width), windowHeight(height) {
		}
	};

	class MECHANISM_API Window
	{
	public:

		Window(const WindowSettings& windowSettings = WindowSettings());
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&& other) noexcept;
		Window& operator=(Window&& other) noexcept;

		void OnUpdate();

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		bool CloseWindow() const;

		bool IsVSync() const;
		void SetVSync(bool enabled);

		void* GetNativeWindow() const { return m_Window; }

		void SwapBuffers();

		SpriteRenderer& GetSpriteRenderer() { return m_SpriteRenderer; }

	private:

		struct WindowData
		{
			std::string title;
			unsigned int width = 0;
			unsigned int height = 0;
			bool VSync = true;
			bool shouldClose = false;
		};

		bool Init(const WindowSettings& windowSettings);
		void Cleanup();

		SDL_Window* m_Window;
		void* m_GLContext;
		WindowData m_Data;
		SpriteRenderer m_SpriteRenderer;
	};
}