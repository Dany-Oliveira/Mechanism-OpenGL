#pragma once
#include "Core.h"
#include "Texture.h"
#include "Box2DBody.h"
#include <memory>

namespace Mechanism
{

    class SpriteRenderer;

    class MECHANISM_API Actor
    {

    public:

        enum class CollisionTag
        {
            None = 0,
            Player,
            Enemy,
            Projectile,
            Background,
            EnemyProjectile,
			PowerUp
        };

        Actor(void* renderer, const char* texturePath, float x, float y,
            int gridColumns, int gridRows, int frameIndex = 0);
        virtual ~Actor();

        void Render(SpriteRenderer* renderer);
        void SetPosition(float x, float y);
        void UpdateActor(float deltaTime);

		void ScaleActor(float scaleX, float scaleY);

        void SetRotation(float degrees) { m_Rotation = degrees; }
		float GetRotation() const { return m_Rotation; }
        
        void SetFrameIndex(int index);
        void SetAnimationSpeed(float fps){ m_FrameDuration = 1.0f / fps; }

        float GetX() const { return m_X; }
        float GetY() const { return m_Y; }

		int GetTextureWidth() const { return m_TextureWidth; }
		int GetTextureHeight() const { return m_TextureHeight; }
		int GetFrameWidth() const { return m_FrameWidth; }
		int GetFrameHeight() const { return m_FrameHeight; }

        bool IsValid() const { return m_Texture != nullptr; }

		void CreatePhysicsBody(void* worldId, bool isDynamic = true, bool isBullet = false);
        void SyncPhysicsToVisual();

		Box2DBody& GetPhysicsBody(){ return m_Box2DBody; }

        bool HasPhysicsBody() const { return m_Box2DBody.IsValid(); }

        void SetCollisionTag(CollisionTag tag){ m_CollisionTag = tag; }
        CollisionTag GetCollisionTag() const { return m_CollisionTag; }

        void SetAnimationEnabled(bool enabled) { m_AnimationEnabled = enabled; }
        bool IsAnimationEnabled() const { return m_AnimationEnabled; }

        void SetPlayOnce(bool playOnce) { m_PlayOnce = playOnce; }  
        int GetFrameIndex() const { return m_CurrentFrame; }

		virtual void OnCollisionBegin(Actor* other) {}

        void SetIsDead(bool dead) { isDead = dead; }
        bool IsDead() const { return isDead; }

    private:

		//Texture
		std::unique_ptr<Texture> m_Texture;

        bool isDead = false;

		// Position
        float m_X;
        float m_Y;

		// Scale factors
		float m_ScaleX = 1.0f;
		float m_ScaleY = 1.0f;

		float m_Rotation = 0.0f; // Rotation in degrees

        // Grid info
        int m_GridColumns;     // How many sprites across
        int m_GridRows;        // How many sprites down
        int m_CurrentFrame;    // Which frame to display

        // Calculated from texture and grid
        int m_TextureWidth;    // Full image width
        int m_TextureHeight;   // Full image height
        int m_FrameWidth;      // Width of one sprite 
        int m_FrameHeight;     // Height of one sprite 

		float m_AnimationTimer; //time since last frame change
        float m_FrameDuration; //FPS
        int m_TotalFrames;   
		bool m_AnimationEnabled = true;
		bool m_PlayOnce = false;

		Box2DBody m_Box2DBody;
		CollisionTag m_CollisionTag = CollisionTag::None;
    };
}
