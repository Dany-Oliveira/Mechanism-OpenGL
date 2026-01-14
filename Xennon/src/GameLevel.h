#pragma once
#include <vector>
#include "Mechanism/Level.h"
#include "Mechanism/Actor.h"
#include "Mechanism/Window.h"
#include "Mechanism/Renderer.h"
#include "Spaceship.h"  
#include <memory>  
#include <optional>



    class GameLevel : public Mechanism::Level
    {

    public:
        GameLevel(Mechanism::Window& window);
        ~GameLevel();

        void Render() override;
        void UpdateGameLevel(float deltaTime);

		void SpawnLoner(float x, float y);
		void SpawnRusher(float x, float y);
		void SpawnPlayer(float x, float y);

        void AddBackground();
        void ClearAllActors();


    private:

		Mechanism::Renderer* m_Renderer;

		int m_WindowWidth;
		int m_WindowHeight;

        Mechanism::Actor* m_Background;
        Mechanism::Actor* m_Loner;
        Mechanism::Actor* m_Rusher;
        Spaceship* m_Player;

		std::vector<std::unique_ptr<Mechanism::Actor>> m_Actors;// All actors in the level
    };

