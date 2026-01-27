#include "Window.h"
#include "Input.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>  
#include <iostream>

namespace Mechanism
{
    static bool s_SDLInitialized = false;

    Window::Window(const WindowSettings& windowSettings)
        : m_Window(nullptr), m_GLContext(nullptr), m_Data{}
    {
        if (!Init(windowSettings))
        {
            std::cerr << "Window initialization failed\n";
            Cleanup();
        }
    }

    Window::~Window()
    {
        Cleanup();
    }

    // Move constructor
    Window::Window(Window&& other) noexcept
        : m_Window(other.m_Window)
        , m_GLContext(other.m_GLContext)
        , m_Data(std::move(other.m_Data))
    {
        other.m_Window = nullptr;
        other.m_GLContext = nullptr;
    }

    // Move assignment
    Window& Window::operator=(Window&& other) noexcept
    {
        if (this != &other)
        {
            Cleanup();

            m_Window = other.m_Window;
            m_GLContext = other.m_GLContext;
            m_Data = std::move(other.m_Data);

            other.m_Window = nullptr;
            other.m_GLContext = nullptr;
        }
        return *this;
    }

    bool Window::Init(const WindowSettings& windowSettings)
    {
        m_Data.title = windowSettings.windowTitle;
        m_Data.width = windowSettings.windowWidth;
        m_Data.height = windowSettings.windowHeight;
        m_Data.VSync = true;
        m_Data.shouldClose = false;

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
                return false;
            }
            s_SDLInitialized = true;
        }

        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create SDL window with OpenGL 
        m_Window = SDL_CreateWindow(
            m_Data.title.c_str(),
            m_Data.width,
            m_Data.height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

        if (!m_Window)
        {
            std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
            return false;
        }

        // Create OpenGL context
        m_GLContext = SDL_GL_CreateContext(m_Window);
        if (!m_GLContext)
        {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << "\n";
            return false;
        }

        // Make context current
        if (!SDL_GL_MakeCurrent(m_Window, static_cast<SDL_GLContext>(m_GLContext)))
        {
            std::cerr << "Failed to make context current: " << SDL_GetError() << "\n";
            return false;
        }

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD\n";
            return false;
        }

        // Set Vsync
        SDL_GL_SetSwapInterval(m_Data.VSync ? 1 : 0);

        // Set viewport
        glViewport(0, 0, m_Data.width, m_Data.height);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

        // Show the window
        SDL_ShowWindow(m_Window);
        m_SpriteRenderer.Initialize(m_Data.width, m_Data.height);
        return true;
    }

    void Window::Cleanup()
    {
        if (m_GLContext)
        {
            SDL_GL_DestroyContext(static_cast<SDL_GLContext>(m_GLContext));
            m_GLContext = nullptr;
        }

        if (m_Window)
        {
            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
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
                m_Data.shouldClose = true;
            }

            // Handle window resize event
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                m_Data.width = event.window.data1;
                m_Data.height = event.window.data2;
                glViewport(0, 0, m_Data.width, m_Data.height);
                m_SpriteRenderer.UpdateProjection(m_Data.width, m_Data.height);
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

    void Window::SwapBuffers()
    {
        if (m_Window)
        {
            SDL_GL_SwapWindow(m_Window);
        }
    }

    unsigned int Window::GetWidth() const
    {
        return m_Data.width;
    }

    unsigned int Window::GetHeight() const
    {
        return m_Data.height;
    }

    bool Window::CloseWindow() const
    {
        return m_Data.shouldClose;
    }

    bool Window::IsVSync() const
    {
        return m_Data.VSync;
    }

    void Window::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;
        SDL_GL_SetSwapInterval(enabled ? 1 : 0);
    }
}