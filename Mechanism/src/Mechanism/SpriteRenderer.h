#pragma once
#include "Core.h"
#include <string>

namespace Mechanism
{
	class Texture;

	class MECHANISM_API SpriteRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		// Delete copy
		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;

		void Initialize(unsigned int screenWidth, unsigned int screenHeight);
		void UpdateProjection(unsigned int screenWidth, unsigned int screenHeight);

		void DrawSprite(const Texture* texture,
			float x, float y, float width, float height,
			int frameX, int frameY,
			int frameWidth, int frameHeight,
			int textureWidth, int textureHeight);

	private:
		void CreateShader();
		void CreateQuad();
		std::string LoadShaderSource(const std::string& filepath);

		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_ShaderProgram;
		unsigned int m_ScreenWidth;
		unsigned int m_ScreenHeight;
	};
}