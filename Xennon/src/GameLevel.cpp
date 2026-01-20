#include "GameLevel.h"
#include <iostream>
#include <algorithm>


GameLevel::GameLevel(Mechanism::Window& window) :
    Level(0.0f, 0.0f),
    m_Window(window),
    m_Renderer(&window.GetRenderer()),
    m_WindowWidth(window.GetWidth()),
    m_WindowHeight(window.GetHeight()),
    m_Background(nullptr),
    m_Player(nullptr)
    {
        printf("\nGameLevel created!\n");

        GetBox2DWorld().SetCollisionBeginCallback(
            [this](Mechanism::Actor* actorA, Mechanism::Actor* actorB)
            {
                OnCollisionBegin(actorA, actorB);
			});

		AddBackground();

		SpawnEnemy("assets/LonerA.bmp", 300.0f, 100.0f, 4, 4);
		SpawnEnemy("assets/rusher.bmp", 500.0f, 300.0f, 6, 4);

		SpawnPlayer(m_WindowWidth / 2.0f, m_WindowHeight - 100.0f);// Spawn player near bottom center















        //do text stuff
        DisplayText("HELLO WORLD", 100.0f, 50.0f, 3.0f, 5.0f);









/*
        m_LetterA = new Mechanism::Actor(
            m_Renderer->GetNativeRenderer(),
            "assets/alphabet_tileset.bmp",
            m_WindowWidth / 2.0f,
            m_WindowHeight / 2.0f,
            8,     // numero de colunas
            12,    // numero de rows
            32     // Tem de se calcular o numero de cada letra assim: numero da row veses numero da linha mais o numero da coluna, pro a isto fica 32
        );
     
        //ficar maior
        m_LetterA->ScaleActor(5.0f, 5.0f);

        if (m_LetterA && m_LetterA->IsValid())
        {
            printf("Letter Actor created successfully!\n");
        }

       // m_Actors.push_back(std::move(letterA));

        //debug para ver se a letra spawna
        if (m_LetterA && m_LetterA->IsValid())
        {
            printf("Letter Actor created successfully!\n");
        }
        else
        {
            printf("ERROR: Letter Actor failed to load!\n");
        }
        printf("Spawned %zu actors\n", m_Actors.size()); */
    } 




void GameLevel::DisplayText(const std::string& text, float startX, float startY, float scale, float spacing)
{
    float currentX = startX;

    for (char c : text)
    {
        int frameIndex = -1;

        if (c == ' ')
        {
            currentX += 21 * scale + spacing;
            continue;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            frameIndex = 33 + (c - 'A');  // Uppercase A starts at frame 33
        }
        else if (c >= 'a' && c <= 'z')
        {
            frameIndex = 65 + (c - 'a');  // Lowercase a starts at frame 65
        }
        else if (c >= '0' && c <= '9')
        {
            frameIndex = 16 + (c - '0');  // Digit 0 starts at frame 16
        }

        if (frameIndex >= 0)
        {
            Mechanism::Actor* letter = new Mechanism::Actor(
                m_Renderer->GetNativeRenderer(),
                "assets/alphabet_tileset.bmp",
                currentX,
                startY,
                8,
                12,
                frameIndex
            );
            letter->ScaleActor(scale, scale);
            m_Letters.push_back(letter);

            currentX += 21 * scale + spacing;
        }
    }
}
     







    GameLevel::~GameLevel()
    {
        printf("GameLevel destroyed! Cleaning up %zu actors\n", m_Actors.size());
		m_Projectiles.clear();
        ClearAllActors();
        for (auto* letter : m_Letters)
        {
            delete letter;
        }
        m_Letters.clear();
    }


 
    void GameLevel::AddBackground ()
    {
        auto background = std::make_unique<Mechanism::Actor>(m_Renderer->GetNativeRenderer(), "assets/galaxy2.bmp", 0, 0, 1, 1, 0);

		m_Background = background.get();
        if(m_Background)
        {
            m_Background->ScaleActor(3.0f, 3.0f);
        }  
		m_Actors.push_back(std::move(background));

        printf("Background added\n\n");        
    }


    void GameLevel::SpawnEnemy(const char* texturePath, float xPos, float yPos, int cols, int rows)
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto enemy = std::make_unique<Enemy>(m_Renderer->GetNativeRenderer(), texturePath, xPos, yPos, cols, rows, 0);

		enemy->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, false); //Create physics body for enemy
		enemy->SetCollisionTag(Mechanism::Actor::CollisionTag::Enemy); //Set collision tag to indentify as enemy

		m_Enemies.push_back(std::move(enemy));

		printf("Enemy spawned at (%.0f, %.0f)\n\n", xPos, yPos);
    }


    void GameLevel::SpawnPlayer(float xPos, float yPos)
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto player = std::make_unique<Spaceship>(m_Renderer->GetNativeRenderer(), "assets/Ship1.bmp", xPos, yPos, 7, 1, 3);

		player->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, false); //Create physics body for player
		player->SetCollisionTag(Mechanism::Actor::CollisionTag::Player); //Set collision tag to indentify as player
		player->SetSpeed(5.0f); // Set player speed

		// Set up shooting callback
        player->SetShootCallback([this](float x, float y)
        {
            SpawnProjectile(x, y);
		});

		m_Player = player.get();
		m_Actors.push_back(std::move(player));
       
		printf("\Player spawned at (%.0f, %.0f)\n\n", xPos, yPos);
    }

    void GameLevel::SpawnProjectile(float x, float y)
    {
		auto projectile = std::make_unique<Projectile>(m_Renderer->GetNativeRenderer(), "assets/missile.bmp", x, y, 1, 1, 0);

		projectile->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, true); //Create physics body for projectile
		projectile->SetCollisionTag(Mechanism::Actor::CollisionTag::Projectile); //Set collision tag to indentify as projectile

		m_Projectiles.push_back(std::move(projectile));
		printf("Projectile spawned at (%.0f, %.0f)\n\n", x, y);
    }









    void GameLevel::Render()
    {
        // Render all actors
        for (const auto& actor : m_Actors)
        {
            if (actor)
            {
                actor->Render(m_Renderer->GetNativeRenderer());
            }
        }

        // Render all enemies
        for (const auto& enemy : m_Enemies)
        {
            if (enemy)
            {
                enemy->Render(m_Renderer->GetNativeRenderer());
            }
        }

        // Render all projectiles
        for (const auto& projectile : m_Projectiles)
        {
            if (projectile)
            {
                projectile->Render(m_Renderer->GetNativeRenderer());
            }
        }

        for (auto* letter : m_Letters)
        {
            if (letter && letter->IsValid())
            {
                letter->Render(m_Renderer->GetNativeRenderer());
            }
        }
    }

    void GameLevel::UpdateGameLevel(float deltaTime)
    {
		Level::Update(deltaTime);

        // Update player-specific logic
        if (m_Player)
        {
            m_Player->PlayerUpdate(deltaTime);
		}

        // Update all actors
        for (const auto& actor : m_Actors)
        {
            if (actor)
            {
                actor->UpdateActor(deltaTime);
            }
        }
        
		// Update all enemies
        for (auto& enemy : m_Enemies)
        {
            if (enemy)
            {
                enemy->UpdateActor(deltaTime);
            }
		}

		//Update all projectiles
        for(auto& projectile : m_Projectiles)
        {
            if(projectile)
            {
                projectile->UpdateProjectile(deltaTime);
				projectile->UpdateActor(deltaTime);
            }
        }

		//Remove dead enemies
        m_Enemies.erase(
            std::remove_if(m_Enemies.begin(), m_Enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy)
                {
                    return enemy->IsDead();
                }),
            m_Enemies.end()
		);

		//Remove off-screen projectiles
        m_Projectiles.erase(
            std::remove_if(m_Projectiles.begin(), m_Projectiles.end(),
                [this](const std::unique_ptr<Projectile>& proj)
                {
                    return proj->IsDead() || proj->IsOffScreen(m_WindowHeight); 
                }),
            m_Projectiles.end()
		);

    }

    void GameLevel::OnCollisionBegin(Mechanism::Actor* actorA, Mechanism::Actor* actorB)
    {
        if (!actorA || !actorB)
        {
            return;
        }
        // Notify both actors of the collision
        actorA->OnCollisionBegin(actorB);
        actorB->OnCollisionBegin(actorA);
    }


    void GameLevel::ClearAllActors()
    {
        m_Actors.clear();
		m_Background = nullptr;
		m_Player = nullptr;
    }

 


