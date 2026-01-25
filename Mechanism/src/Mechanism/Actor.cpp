#include "Actor.h"
#include "SpriteRenderer.h"
#include <SDL3/SDL.h>
#include <iostream>


namespace Mechanism
{
    Actor::Actor(void* renderer, const char* texturePath, float x, float y, int gridColumns, int gridRows, int frameIndex)
        : m_X(x), m_Y(y), m_ScaleX(1.0f), m_ScaleY(1.0f), m_GridColumns(gridColumns), m_GridRows(gridRows)
        , m_CurrentFrame(frameIndex), m_TextureWidth(0), m_TextureHeight(0), m_FrameWidth(0), m_FrameHeight(0), m_AnimationTimer(0)
        , m_FrameDuration(0.1), m_TotalFrames(0)
    {

        if (!renderer)
        {
            std::cerr << "Error: Null renderer" << std::endl;
            return;
        }
        
        m_Texture = std::make_unique<Texture>(renderer, texturePath);

        if(!m_Texture->isValid())
        {
            std::cerr << "Error: Failed to load texture: " << texturePath << std::endl;
            return;
		}

		// Get texture dimensions
		m_TextureWidth = m_Texture->GetWidth();
		m_TextureHeight = m_Texture->GetHeight();

		// Calculate frame dimensions
		m_FrameWidth = m_TextureWidth / m_GridColumns;
		m_FrameHeight = m_TextureHeight / m_GridRows;

		// Total frames
        m_TotalFrames = m_GridColumns * m_GridRows;

        std::cout << "\nActor loaded: " << texturePath << std::endl;
        std::cout << "  Texture: " << m_TextureWidth << "x" << m_TextureHeight << std::endl;
        std::cout << "  Grid: " << m_GridColumns << " columns x " << m_GridRows << " rows" << std::endl;
        std::cout << "  Frame size: " << m_FrameWidth << "x" << m_FrameHeight << std::endl;
        std::cout << "  Total frames: " << m_TotalFrames << std::endl;

    }

    Actor::~Actor()
	{
		
    }


    void Actor::UpdateActor(float deltaTime)
    {
		// Sync physics position to visual position
		SyncPhysicsToVisual();

        if (m_TotalFrames <= 1)
            return;

        // Count time
        m_AnimationTimer += deltaTime;

        // Switch frame when enough time has passed
        if (m_AnimationTimer >= m_FrameDuration)
        {
            m_AnimationTimer = 0.0f;

            ++m_CurrentFrame;

            if (m_CurrentFrame >= m_TotalFrames)
                m_CurrentFrame = 0;  // loop back to start
        }
    }

    void Actor::ScaleActor(float scaleX, float scaleY)
    {
		m_ScaleX = scaleX;
		m_ScaleY = scaleY;
    }

    void Actor::Render(SpriteRenderer* renderer)
    {
        if (!m_Texture || !m_Texture->isValid() || !renderer)
            return;

        // Calculate which row and column this frame is in
        int col = m_CurrentFrame % m_GridColumns;
        int row = m_CurrentFrame / m_GridColumns;

        // Calculate the final dimensions with scale
        float renderWidth = m_FrameWidth * m_ScaleX;
        float renderHeight = m_FrameHeight * m_ScaleY;

        // Draw the sprite using OpenGL
        renderer->DrawSprite(
            m_Texture.get(), 
            m_X, m_Y, 
            renderWidth, renderHeight,
            col * m_FrameWidth, row * m_FrameHeight,
            m_FrameWidth, m_FrameHeight,
            m_TextureWidth, m_TextureHeight
        );
    }

    void Actor::SetPosition(float x, float y)
    {
        m_X = x;
        m_Y = y;
    }

    void Actor::SetFrameIndex(int index)
    {
        if (index >= 0 && index < m_TotalFrames)
        {
            m_CurrentFrame = index;
        }
    }

    void Actor::CreatePhysicsBody(void* worldId, bool isDynamic, bool isBullet)
    {
		// Convert position from pixels to meters
		float pixelsPerMeter = 100.0f; 
		float physicsX = m_X / pixelsPerMeter;
		float physicsY = m_Y / pixelsPerMeter;

        if(isDynamic)
        {
            m_Box2DBody.CreateDynamic(worldId, physicsX, physicsY, isBullet);
        }
        else
        {
            m_Box2DBody.CreateStatic(worldId, physicsX, physicsY);
		}

		// Calculate half-width and half-height in meters, considering scale
		float halfWidth = (m_FrameWidth * m_ScaleX * 0.5f) / pixelsPerMeter;
		float halfHeight = (m_FrameHeight * m_ScaleY * 0.5f) / pixelsPerMeter;

		// Add box shape
		m_Box2DBody.AddBoxShape(halfWidth, halfHeight, 1.0f, 0.0f);

		// store pointer to this Actor in the Box2DBody user data
		m_Box2DBody.SetUserData(this);
    }

    void Actor::SyncPhysicsToVisual()
    {
        if (m_Box2DBody.IsValid())
        {   
			float physicsx, physicsy;
			m_Box2DBody.GetPosition(physicsx, physicsy);

			// Convert from meters to pixels
			float pixelsPerMeter = 100.0f;
            m_X = physicsx * pixelsPerMeter;
			m_Y = physicsy * pixelsPerMeter;
        }

    }
}