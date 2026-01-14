#include "Renderer.h"
#include <SDL3/SDL.h>

namespace Mechanism
{
	Renderer::Renderer(void* sdlRenderer) : m_SDLRenderer(sdlRenderer), m_OwnsRenderer(false)
	{

	}

	Renderer::~Renderer()
	{
		if (m_SDLRenderer && m_OwnsRenderer)
		{
			SDL_DestroyRenderer(static_cast<SDL_Renderer*>(m_SDLRenderer));
			m_SDLRenderer = nullptr;
		}
	}

	void Renderer::Clear(unsigned char r, unsigned char g, unsigned char b)
	{
		auto* renderer = static_cast<SDL_Renderer*>(m_SDLRenderer);
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);
	}

	void Renderer::Present()
	{
		SDL_RenderPresent(static_cast<SDL_Renderer*>(m_SDLRenderer));
	}
}
