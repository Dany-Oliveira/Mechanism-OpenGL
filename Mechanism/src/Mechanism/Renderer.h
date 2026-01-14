#pragma once
#include "Core.h"

namespace Mechanism
{

	class MECHANISM_API Renderer
	{

	public:
		Renderer(void* sdlRenderer);
		~Renderer();

		void Clear(unsigned char r = 0, unsigned char g = 0, unsigned char  b = 0);
		void Present();

		void* GetNativeRenderer() const { return m_SDLRenderer; }

	private:
		void* m_SDLRenderer;
		bool m_OwnsRenderer;

	};


}
