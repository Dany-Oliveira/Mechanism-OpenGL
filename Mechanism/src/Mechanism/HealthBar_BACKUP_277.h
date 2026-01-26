#pragma once
#include "Core.h"

<<<<<<< HEAD
=======
// Forward declaration
namespace Mechanism
{
    class SpriteRenderer;
}

>>>>>>> GameEngine-With-OpenGL
namespace Mechanism
{
    class MECHANISM_API HealthBar
    {
    public:
        struct Color
        {
            unsigned char r, g, b, a;
        };

        HealthBar(int maxHealth = 10, int x = 10, int y = 10, int rectWidth = 30, int rectHeight = 20, int spacing = 5);
        ~HealthBar();

<<<<<<< HEAD
        void Render(void* renderer);

        int GetCurrentHealth() const { return m_CurrentHealth; }
        int GetMaxHealth() const { return m_MaxHealth; }

        void SetCurrentHealth(int health);
        void SetMaxHealth(int maxHealth);

=======
        void Render(SpriteRenderer* spriteRenderer);

        int GetCurrentHealth() const { return m_CurrentHealth; }
        int GetMaxHealth() const { return m_MaxHealth; }
        void SetCurrentHealth(int health);
        void SetMaxHealth(int maxHealth);
>>>>>>> GameEngine-With-OpenGL
        void SetPosition(int x, int y);

    private:
        void UpdateColor();
<<<<<<< HEAD

        int m_CurrentHealth;
        int m_MaxHealth;

=======
        void Initialize(SpriteRenderer* spriteRenderer);

        int m_CurrentHealth;
        int m_MaxHealth;
>>>>>>> GameEngine-With-OpenGL
        int m_X;
        int m_Y;
        int m_RectWidth;
        int m_RectHeight;
        int m_Spacing;

        Color m_CurrentColor;
        Color m_HighHealthColor;
        Color m_MediumHealthColor;
        Color m_LowHealthColor;
<<<<<<< HEAD
=======

        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_WhiteTexture;
        unsigned int m_ShaderProgram;
        bool m_Initialized;
        int m_ScreenWidth;
        int m_ScreenHeight;
>>>>>>> GameEngine-With-OpenGL
    };
}