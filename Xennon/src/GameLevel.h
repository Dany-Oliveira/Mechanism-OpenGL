#pragma once
#include <vector>
#include "Mechanism/Level.h"
#include "Mechanism/Actor.h"
#include "Mechanism/Window.h"
#include "Mechanism/Renderer.h"
#include "Mechanism/SpriteRenderer.h"
#include "Mechanism/HealthBar.h"
#include "Spaceship.h"  
#include "Projectile.h"
#include "Enemy.h"
#include "EnemyProjectile.h"
#include "PowerUps.h"
#include <memory>  
#include <optional>
#include <functional>



    class GameLevel : public Mechanism::Level
    {

    public:

        GameLevel(Mechanism::Window& window);
        virtual ~GameLevel();

        void Render() override;
        void UpdateGameLevel(float deltaTime);

		void SpawnEnemy(const char* texturePath, float x, float y, int cols, int rows,
            std::function<void(Enemy*, float)> movementDirection, Enemy::EnemyType type, int health);

		void SpawnPlayer(float x, float y);
		void SpawnProjectile(float x, float y, int damage);
        void SpawnEnemyProjectile(float x, float y, float targetX, float targetY);
		void SpawnPowerUp(const char* texturePath, float x, float y, int cols, int rows, PowerUps::PowerUpType type);
		void SpawnExplosion(float x, float y);


        void AddBackground();
        void ClearAllActors();

    private:

        void OnCollisionBegin(Mechanism::Actor* actorA, Mechanism::Actor* actorB);

        void* m_NativeWindow;
        Mechanism::Actor* m_Background;
        Spaceship* m_Player;
        Mechanism::SpriteRenderer* m_SpriteRenderer;


		int m_WindowWidth;
		int m_WindowHeight;

        float m_EnemySpawnTimer;
        float m_EnemySpawnInterval;
		float m_PowerUpSpawnTimer;
		float m_PowerUpSpawnInterval;

        std::vector<std::unique_ptr<Mechanism::Actor>> m_Actors;// All actors in the level
        std::vector<std::unique_ptr<Projectile>> m_Projectiles;// All projectiles in the level   
		std::vector<std::unique_ptr<Enemy>> m_Enemies;// All enemies in the level
        std::vector<std::unique_ptr<EnemyProjectile>> m_EnemyProjectiles;// All enemie projectiles
		std::vector<std::unique_ptr<PowerUps>> m_PowerUps;// All powerups in the level
		std::vector<std::unique_ptr<Mechanism::Actor>> m_Effects;// All effects in the level

        Mechanism::Window& m_Window;
        std::vector<Mechanism::Actor*> m_Letters;
        std::unique_ptr<Mechanism::HealthBar> m_HealthBar;

        void DisplayText(const std::string& text, float startX, float startY, float scale = 5.0f, float spacing = 5.0f);
        
    };

