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

		// Delete copy constructor and assignment
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		// Allow move constructor and assignment
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
		
		unsigned int GetTextureID() const { return m_TextureID; }

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		bool isValid() const { return m_TextureID != 0; }

		const std::string& GetFilepath() const { return m_Filepath; }

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

	private:

		void LoadTexture(const std::string& filepath);

		unsigned int m_TextureID = 0;
		std::string m_Filepath;
		int m_Width = 0;
		int m_Height = 0;

	};

}