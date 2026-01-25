#include "Texture.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"
#include <iostream>

namespace Mechanism
{

	Texture::Texture(void* renderer, const std::string& filepath): m_Filepath(filepath), m_TextureID(0), m_Width(0), m_Height(0)
	{
		LoadTexture(filepath);
	}

	Texture::~Texture()
	{
		if(m_TextureID != 0)
		{
			glDeleteTextures(1, &m_TextureID);
			m_TextureID = 0;
		}
	}

	// Move constructor
	Texture::Texture(Texture&& other) noexcept
		: m_TextureID(other.m_TextureID)
		, m_Filepath(std::move(other.m_Filepath))
		, m_Width(other.m_Width)
		, m_Height(other.m_Height)
	{
		other.m_TextureID = 0;
		other.m_Width = 0;
		other.m_Height = 0;
	}

	// Move assignment
	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			// Delete current texture
			if (m_TextureID != 0)
			{
				glDeleteTextures(1, &m_TextureID);
			}

			// Move from other
			m_TextureID = other.m_TextureID;
			m_Filepath = std::move(other.m_Filepath);
			m_Width = other.m_Width;
			m_Height = other.m_Height;

			// Reset other
			other.m_TextureID = 0;
			other.m_Width = 0;
			other.m_Height = 0;
		}
		return *this;
	}

	void Texture::LoadTexture(const std::string& filepath)
	{
		// Load BMP using SDL
		SDL_Surface* surface = SDL_LoadBMP(filepath.c_str());

		if (!surface)
		{
			std::cerr << "Failed to load BMP: " << filepath << std::endl;
			std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
			return;
		}

		// Store dimensions
		m_Width = surface->w;
		m_Height = surface->h;

		// Determine format
		GLenum format = GL_BGR;
		const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(surface->format);

		if (formatDetails)
		{
			if (formatDetails->bytes_per_pixel == 4)
			{
				format = GL_BGRA;
			}
			else if (formatDetails->bytes_per_pixel == 3)
			{
				format = GL_BGR;
			}
		}
	

		// Generate OpenGL texture
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload texture data to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height,
			0, format, GL_UNSIGNED_BYTE, surface->pixels);

		// Clean up SDL surface
		SDL_DestroySurface(surface);

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		std::cout << "OpenGL Texture loaded: " << filepath << " (ID: " << m_TextureID << ")\n";
		std::cout << "  Size: " << m_Width << "x" << m_Height << std::endl;
	}

	void Texture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}