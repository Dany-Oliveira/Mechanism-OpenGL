#include "SpriteRenderer.h"
#include "Texture.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>     
#include <sstream> 
#include <filesystem>

namespace Mechanism
{

	Mechanism::SpriteRenderer::SpriteRenderer() : m_VAO(0), m_VBO(0), m_ShaderProgram(0), m_ScreenWidth(1280), m_ScreenHeight(720)
	{
	}

	Mechanism::SpriteRenderer::~SpriteRenderer()
	{
		if(m_VAO != 0)
		{
			glDeleteVertexArrays(1, &m_VAO);
		}
		if(m_VBO != 0)
		{
			glDeleteBuffers(1, &m_VBO);
		}
		if(m_ShaderProgram != 0)
		{
			glDeleteProgram(m_ShaderProgram);
		}
	}

	void Mechanism::SpriteRenderer::Initialize(unsigned int screenWidth, unsigned int screenHeight)
	{
		m_ScreenWidth = screenWidth;
		m_ScreenHeight = screenHeight;

		CreateShader();
		CreateQuad();
		UpdateProjection(screenWidth, screenHeight);

		std::cout << "Sprite Renderer initialized\n";
	}

	void Mechanism::SpriteRenderer::UpdateProjection(unsigned int screenWidth, unsigned int screenHeight)
	{
		m_ScreenWidth = screenWidth;
		m_ScreenHeight = screenHeight;

		// Create orthographic projection (0,0 at top-left, matches SDL coordinate system)
		glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);

		glUseProgram(m_ShaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void SpriteRenderer::DrawSprite(const Texture* texture,
		float x, float y, float width, float height,
		int frameX, int frameY, int frameWidth, int frameHeight,
		int textureWidth, int textureHeight)
	{
		if (!texture || !texture->isValid())
			return;

		glUseProgram(m_ShaderProgram);

		glUniform1i(glGetUniformLocation(m_ShaderProgram, "useRectColor"), false);

		// Create model matrix (position and scale)
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, 0.0f));
		model = glm::scale(model, glm::vec3(width, height, 1.0f));

		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Set color key for transparency (magenta)
		glUniform3f(glGetUniformLocation(m_ShaderProgram, "colorKey"), 1.0f, 0.0f, 1.0f);
		glUniform1i(glGetUniformLocation(m_ShaderProgram, "useColorKey"), true);

		// Bind texture
		texture->Bind(0);
		glUniform1i(glGetUniformLocation(m_ShaderProgram, "texture1"), 0);

		// Calculate texture coordinates for the specific frame
		float texLeft = (float)frameX / (float)textureWidth;
		float texRight = (float)(frameX + frameWidth) / (float)textureWidth;
		float texTop = (float)frameY / (float)textureHeight;
		float texBottom = (float)(frameY + frameHeight) / (float)textureHeight;

		// Update quad vertices with correct texture coordinates for this frame
		float vertices[] = {
			// Positions   // TexCoords
			0.0f, 1.0f,    texLeft,  texBottom,  // Top-left
			1.0f, 0.0f,    texRight, texTop,     // Bottom-right
			0.0f, 0.0f,    texLeft,  texTop,     // Bottom-left

			0.0f, 1.0f,    texLeft,  texBottom,  // Top-left
			1.0f, 1.0f,    texRight, texBottom,  // Top-right
			1.0f, 0.0f,    texRight, texTop      // Bottom-right
		};

		// Update VBO with new texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Draw the quad
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	std::string SpriteRenderer::LoadShaderSource(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			std::cerr << "Failed to open shader file: " << filepath << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void Mechanism::SpriteRenderer::CreateShader()
	{
		std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

		// Load vertex or fragment shader from files
		std::string vertexShaderFile = LoadShaderSource("../Mechanism/src/Mechanism/shaders/vertex_shader.glsl");
		std::string fragmentShaderFile = LoadShaderSource("../Mechanism/src/Mechanism/shaders/fragment_shader.glsl");

		if (vertexShaderFile.empty() || fragmentShaderFile.empty())
		{
			std::cerr << "Failed to load shader files\n";
			return;
		}

		const char* vertexShaderSource = vertexShaderFile.c_str();
		const char* fragmentShaderSource = fragmentShaderFile.c_str();

		//Compile vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		//Check for error
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cerr << "Vertex shader compilation failed\n" << infoLog << std::endl;
		}
		
		//Compile fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
		}


		//Link shaders
		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, vertexShader);
		glAttachShader(m_ShaderProgram, fragmentShader);
		glLinkProgram(m_ShaderProgram);

		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
			std::cerr << "Shader program linking failed\n" << infoLog << std::endl;
		}

		//Clean up
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::cout << "Shader loaded from files\n";
	}


	void Mechanism::SpriteRenderer::CreateQuad()
	{
		// Quad vertices (position + texture coordinates)
		float vertices[] = {
			// Positions   // TexCoords
			0.0f, 1.0f,    0.0f, 1.0f,  // Top-left
			1.0f, 0.0f,    1.0f, 0.0f,  // Bottom-right
			0.0f, 0.0f,    0.0f, 0.0f,  // Bottom-left

			0.0f, 1.0f,    0.0f, 1.0f,  // Top-left
			1.0f, 1.0f,    1.0f, 1.0f,  // Top-right
			1.0f, 0.0f,    1.0f, 0.0f   // Bottom-right
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture coordinate attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}