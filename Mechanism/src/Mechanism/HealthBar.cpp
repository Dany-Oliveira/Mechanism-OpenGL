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
    {
        m_HighHealthColor = { 0, 255, 0, 255 };
        m_MediumHealthColor = { 255, 255, 0, 255 };
        m_LowHealthColor = { 255, 0, 0, 255 };
        UpdateColor();
    }

    HealthBar::~HealthBar()
    {

    }

    void HealthBar::Render(SpriteRenderer* spriteRenderer)
    {
       if(!spriteRenderer)
       {
           return;
       }

	   Color emptyColor = { 50, 50, 50, 255 };

       for(int i = 0; i < m_MaxHealth; ++i)
       {
		   float x = m_X + (i * (m_RectWidth + m_Spacing));
           float y = m_Y;
           float width = static_cast<float>(m_RectWidth);
           float height = static_cast<float>(m_RectHeight);

		   Color rectColor = (i < m_CurrentHealth) ? m_CurrentColor : emptyColor;

           spriteRenderer->DrawColoredRectangle(x, y, m_RectWidth, m_RectHeight, 
               rectColor.r / 255.0f, rectColor.g / 255.0f, rectColor.b / 255.0f, 1.0f);
       }

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