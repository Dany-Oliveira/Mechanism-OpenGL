#pragma once

#include "Core.h"
#include <string>


namespace Mechanism
{

	class MECHANISM_API Texture
	{

	public:
		Texture(void* renderer, const std::string& filepath);
		~Texture();

		void* GetSDLTexture() const { return m_Texture;}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		
		bool isValid() const { return m_Texture != nullptr; }

		const std::string& GetFilepath() const { return m_Filepath; }

	private:

		void LoadTexture(void* renderer, const std::string& filepath);

		void* m_Texture = nullptr;
		std::string m_Filepath;
		int m_Width = 0;
		int m_Height = 0;

	};

}