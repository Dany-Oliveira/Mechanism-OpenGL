#pragma once
#include <vector>
#include "Mechanism/Level.h"
#include "Mechanism/Actor.h"
#include "Mechanism/Window.h"
#include "Mechanism/Renderer.h"
#include "Spaceship.h"  
#include "Projectile.h"
#include <memory>  
#include <optional>



    class GameLevel : public Mechanism::Level
    {

    public:
        GameLevel(Mechanism::Window& window);
        virtual ~GameLevel();

        void Render() override;
        void UpdateGameLevel(float deltaTime);

		void SpawnLoner(float x, float y);
		void SpawnRusher(float x, float y);
		void SpawnPlayer(float x, float y);
		void SpawnProjectile(float x, float y);

        void AddBackground();
        void ClearAllActors();


    private:

		int m_WindowWidth;
		int m_WindowHeight;

        std::vector<std::unique_ptr<Mechanism::Actor>> m_Actors;// All actors in the level
        std::vector<std::unique_ptr<Projectile>> m_Projectiles;// All projectiles in the level

        Mechanism::Renderer* m_Renderer;
        Mechanism::Actor* m_Background;
        Mechanism::Actor* m_Loner;
        Mechanism::Actor* m_Rusher;
        Spaceship* m_Player;

		
    };

