#include "HealthBar.h"
#include "SpriteRenderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

namespace Mechanism
{
    HealthBar::HealthBar(int maxHealth, int x, int y, int rectWidth, int rectHeight, int spacing)
        : m_MaxHealth(maxHealth)
        , m_CurrentHealth(maxHealth)
        , m_X(x)
        , m_Y(y)
        , m_RectWidth(rectWidth)
        , m_RectHeight(rectHeight)
        , m_Spacing(spacing)
        , m_VAO(0)
        , m_VBO(0)
        , m_WhiteTexture(0)
        , m_ShaderProgram(0)
        , m_Initialized(false)
        , m_ScreenWidth(1280)
        , m_ScreenHeight(720)
    {
        m_HighHealthColor = { 0, 255, 0, 255 };
        m_MediumHealthColor = { 255, 255, 0, 255 };
        m_LowHealthColor = { 255, 0, 0, 255 };
        UpdateColor();
    }

    HealthBar::~HealthBar()
    {
        if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
        if (m_WhiteTexture != 0) glDeleteTextures(1, &m_WhiteTexture);
    }

    void HealthBar::Initialize(SpriteRenderer* spriteRenderer)
    {
        if (!spriteRenderer)
            return;

        // Get shader program from SpriteRenderer
        m_ShaderProgram = spriteRenderer->GetShaderProgram();

        // Create a 1x1 white texture for solid color rendering
        glGenTextures(1, &m_WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);

        unsigned char whitePixel[] = { 255, 255, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Create quad VAO/VBO (same structure as SpriteRenderer)
        float vertices[] = {
            0.0f, 1.0f,    0.0f, 1.0f,
            1.0f, 0.0f,    1.0f, 0.0f,
            0.0f, 0.0f,    0.0f, 0.0f,

            0.0f, 1.0f,    0.0f, 1.0f,
            1.0f, 1.0f,    1.0f, 1.0f,
            1.0f, 0.0f,    1.0f, 0.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_Initialized = true;
        std::cout << "HealthBar initialized with OpenGL\n";
    }

    void HealthBar::Render(SpriteRenderer* spriteRenderer)
    {
        if (!spriteRenderer)
            return;

        if (!m_Initialized)
        {
            Initialize(spriteRenderer);
        }

        // Determine color based on health percentage
        float healthPercent = static_cast<float>(m_CurrentHealth) / static_cast<float>(m_MaxHealth);
        Color activeColor;

        if (healthPercent <= 0.33f)
        {
            activeColor = m_LowHealthColor;
        }
        else if (healthPercent <= 0.66f)
        {
            activeColor = m_MediumHealthColor;
        }
        else
        {
            activeColor = m_HighHealthColor;
        }

        Color emptyColor = { 50, 50, 50, 255 };

        // Save OpenGL state
        GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
        GLboolean depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use the sprite shader
        glUseProgram(m_ShaderProgram);

        // Bind white texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
        glUniform1i(glGetUniformLocation(m_ShaderProgram, "texture1"), 0);

        // Disable color key (we want solid colors)
        glUniform1i(glGetUniformLocation(m_ShaderProgram, "useColorKey"), false);

        // Bind our VAO
        glBindVertexArray(m_VAO);

        // Draw each health rectangle
        for (int i = 0; i < m_MaxHealth; ++i)
        {
            float x = static_cast<float>(m_X + (i * (m_RectWidth + m_Spacing)));
            float y = static_cast<float>(m_Y);
            float w = static_cast<float>(m_RectWidth);
            float h = static_cast<float>(m_RectHeight);

            // Choose color
            Color rectColor = (i < m_CurrentHealth) ? activeColor : emptyColor;

            // Create model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, y, 0.0f));
            model = glm::scale(model, glm::vec3(w, h, 1.0f));

            glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Enable color tinting and set the color
            glUniform1i(glGetUniformLocation(m_ShaderProgram, "useRectColor"), true);
            glUniform3f(glGetUniformLocation(m_ShaderProgram, "rectColor"),
                rectColor.r / 255.0f, rectColor.g / 255.0f, rectColor.b / 255.0f);

            // Draw the quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glBindVertexArray(0);

        // Restore state
        if (!blendWasEnabled) glDisable(GL_BLEND);
        if (depthTestWasEnabled) glEnable(GL_DEPTH_TEST);
    }

    void HealthBar::SetCurrentHealth(int health)
    {
        m_CurrentHealth = std::max(0, std::min(health, m_MaxHealth));
        UpdateColor();
    }

    void HealthBar::SetMaxHealth(int maxHealth)
    {
        m_MaxHealth = std::max(1, maxHealth);
        if (m_CurrentHealth > m_MaxHealth)
        {
            m_CurrentHealth = m_MaxHealth;
        }
        UpdateColor();
    }

    void HealthBar::SetPosition(int x, int y)
    {
        m_X = x;
        m_Y = y;
    }

    void HealthBar::UpdateColor()
    {
        float healthPercent = static_cast<float>(m_CurrentHealth) / static_cast<float>(m_MaxHealth);

        if (healthPercent > 0.66f)
        {
            m_CurrentColor = m_HighHealthColor;
        }
        else if (healthPercent > 0.33f)
        {
            m_CurrentColor = m_MediumHealthColor;
        }
        else
        {
            m_CurrentColor = m_LowHealthColor;
        }
    }
}