// Window.cpp - Mechanism Engine
#include "Window.h"
#include "Input.h"
#include <SDL3/SDL.h>
#include <iostream>

namespace Mechanism 
{
    struct WindowData 
    {
        std::string title;
        unsigned int width, height;
        bool VSync;
    };

    static bool s_SDLInitialized = false;

    Window::Window(const WindowSettings& windowSettings) : m_Window(nullptr), m_Data(nullptr) 
    {
        if(!Init(windowSettings))
        {
            std::cerr << "Window initialization failed\n";
            m_Window = nullptr;
            m_Data = nullptr;
            
        }
    }

    Window::~Window() 
    {
        Shutdown();
    }

    bool Window::Init(const WindowSettings& windowSettings) 
    {
        m_Data = new WindowData();

        m_Data->title = windowSettings.windowTitle;
        m_Data->width = windowSettings.windowWidth;
        m_Data->height = windowSettings.windowHeight;
        m_Data->VSync = true;

        std::cout << "\nCreating window: " 
            << windowSettings.windowTitle
            << " (" << windowSettings.windowWidth << "x" 
            << windowSettings.windowHeight << ")\n";


        // Initialize SDL3
        if (!s_SDLInitialized) 
        {
            if (!SDL_Init(SDL_INIT_VIDEO)) 
            {
                std::cerr << "Failed to initialize SDL3: " << SDL_GetError() << "\n";

                delete m_Data;
                m_Data = nullptr;

                return false;
            }
            s_SDLInitialized = true;
        }

        // Create SDL window
        SDL_Window* window = SDL_CreateWindow(
            m_Data->title.c_str(), 
            m_Data->width,
            m_Data->height,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

        if (!window) 
        {
            std::cerr << "Failed to create window: " << SDL_GetError() << "\n";

            delete m_Data;
            m_Data = nullptr;

            return false;
        }

        m_Window = window;


        //Create SDL Renderer
        SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

        if(!renderer)
        {
            std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(window);
            m_Window = nullptr;
            delete m_Data;
            m_Data = nullptr;
            return false;
        }

        m_Renderer = std::make_unique<Mechanism::Renderer>(renderer);

        // Show the window
        SDL_ShowWindow(window);
        return true;
    }

    void Window::Shutdown() 
    {
        m_Renderer.reset();

        if (m_Window) 
        {
            SDL_DestroyWindow(static_cast<SDL_Window*>(m_Window));
            m_Window = nullptr;
        }

        if (m_Data) 
        {
            delete m_Data;
            m_Data = nullptr;
        }
    }

    void Window::OnUpdate() 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
			// Handle quit event
            if (event.type == SDL_EVENT_QUIT) 
            {
                m_CloseWindow = true;
            }

			// Handle window resize event
            else if (event.type == SDL_EVENT_WINDOW_RESIZED) 
            {
                if (m_Data) 
                {
                    m_Data->width = event.window.data1;
                    m_Data->height = event.window.data2;
                }
            }

			// Handle keyboard events
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
				Input::OnKeyDown(event.key.scancode);
            }
            else if (event.type == SDL_EVENT_KEY_UP)
            {
				Input::OnKeyUp(event.key.scancode);
            }

        }   
    }

    unsigned int Window::GetWidth() const
    {
        return m_Data ? m_Data->width: 0;
    }

    unsigned int Window::GetHeight() const
    {
        return m_Data ? m_Data->height: 0;
    }

    bool Window::CloseWindow() const
    {
        return m_CloseWindow;
    }

    bool Window::IsVSync() const 
    {
        return m_Data ? m_Data->VSync: false;
    }

    void Window::SetVSync(bool enabled) 
    {
        if(m_Data)
        {
            m_Data->VSync = enabled;
        }
    }

} 