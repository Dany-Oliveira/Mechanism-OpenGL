#include "HealthBar.h"
#include <SDL3/SDL.h>
#include <algorithm>

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
        // Define colors for different health states
        m_HighHealthColor = { 0, 255, 0, 255 };      // Green
        m_MediumHealthColor = { 255, 255, 0, 255 };  // Yellow
        m_LowHealthColor = { 255, 0, 0, 255 };       // Red

        UpdateColor();
    }

    HealthBar::~HealthBar()
    {
    }

    void HealthBar::Render(void* renderer)
    {
        if (!renderer)
            return;

        SDL_Renderer* sdlRenderer = static_cast<SDL_Renderer*>(renderer);

        // Draw each health rectangle
        for (int i = 0; i < m_MaxHealth; ++i)
        {
            SDL_FRect rect;
            rect.x = static_cast<float>(m_X + (i * (m_RectWidth + m_Spacing)));
            rect.y = static_cast<float>(m_Y);
            rect.w = static_cast<float>(m_RectWidth);
            rect.h = static_cast<float>(m_RectHeight);

            // If this rectangle represents remaining health, draw it filled with color
            // Otherwise draw it as a dark outline/empty
            if (i < m_CurrentHealth)
            {
                // Draw filled rectangle with current health color
                SDL_SetRenderDrawColor(sdlRenderer, m_CurrentColor.r, m_CurrentColor.g, m_CurrentColor.b, m_CurrentColor.a);
                SDL_RenderFillRect(sdlRenderer, &rect);

                // Draw border
                SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
                SDL_RenderRect(sdlRenderer, &rect);
            }
            else
            {
                // Draw empty/lost health rectangle (dark gray)
                SDL_SetRenderDrawColor(sdlRenderer, 50, 50, 50, 255);
                SDL_RenderFillRect(sdlRenderer, &rect);

                // Draw border
                SDL_SetRenderDrawColor(sdlRenderer, 100, 100, 100, 255);
                SDL_RenderRect(sdlRenderer, &rect);
            }
        }
    }

    void HealthBar::SetCurrentHealth(int health)
    {
        // Clamp health between 0 and max health
        m_CurrentHealth = std::max(0, std::min(health, m_MaxHealth));
        UpdateColor();
    }

    void HealthBar::SetMaxHealth(int maxHealth)
    {
        m_MaxHealth = std::max(1, maxHealth); // Ensure max health is at least 1

        // Adjust current health if it exceeds new max
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
        // Calculate health percentage
        float healthPercent = static_cast<float>(m_CurrentHealth) / static_cast<float>(m_MaxHealth);

        // Update color based on health percentage
        if (healthPercent > 0.66f)
        {
            m_CurrentColor = m_HighHealthColor;  // Green
        }
        else if (healthPercent > 0.33f)
        {
            m_CurrentColor = m_MediumHealthColor; // Yellow
        }
        else
        {
            m_CurrentColor = m_LowHealthColor;    // Red
        }
    }
}