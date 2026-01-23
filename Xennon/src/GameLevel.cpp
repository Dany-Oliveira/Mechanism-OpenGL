#include "GameLevel.h"
#include <iostream>
#include <algorithm>


    GameLevel::GameLevel(Mechanism::Window& window): Level(0.0f, 0.0f), 
        m_Renderer(&window.GetRenderer()), m_WindowWidth(window.GetWidth()), m_WindowHeight(window.GetHeight()),
		m_Background(nullptr), m_Player(nullptr)
    {
        printf("\nGameLevel created!\n");

        GetBox2DWorld().SetCollisionBeginCallback([this](Mechanism::Actor* actorA, Mechanism::Actor* actorB)
            {
                OnCollisionBegin(actorA, actorB);
			});

		AddBackground();

        SpawnEnemy("assets/LonerA.bmp", 100.0f, 100.0f, 4, 4, LonerMovement());
        
       
        m_Enemies.back()->SetCanShoot(true);
        m_Enemies.back()->SetShootInterval(2.0f);
        m_Enemies.back()->SetEnemyShootCallback([this](float bulletX, float bulletY, float targetX, float targetY)
            {
                SpawnEnemyProjectile(bulletX, bulletY, targetX, targetY);
            });
        //Enemies spawned after this line will not shoot

        SpawnEnemy("assets/rusher.bmp", 500.0f, 300.0f, 6, 4, RusherMovement());
        SpawnEnemy("assets/drone.bmp", m_WindowWidth / 2.0f, 0, 8, 2, DroneMovement());
          
		SpawnPlayer(m_WindowWidth / 2.0f, m_WindowHeight - 100.0f);// Spawn player near bottom center

        printf("Spawned %zu actors\n", m_Actors.size());
    }

    GameLevel::~GameLevel()
    {
        printf("GameLevel destroyed! Cleaning up %zu actors\n", m_Actors.size());
		m_Projectiles.clear();
        ClearAllActors();
    }


 
    void GameLevel::AddBackground ()
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto background = std::make_unique<Mechanism::Actor>(m_Renderer->GetNativeRenderer(), "assets/galaxy2.bmp", 0, 0, 1, 1, 0);

		m_Background = background.get();
        if(m_Background)
        {
            m_Background->ScaleActor(3.0f, 3.0f);
        }  
		m_Actors.push_back(std::move(background));

        printf("Background added\n\n");        
    }


    void GameLevel::SpawnEnemy(const char* texturePath, float xPos, float yPos, int cols, int rows,
        std::function<void(Enemy*, float)> movementPattern)
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto enemy = std::make_unique<Enemy>(m_Renderer->GetNativeRenderer(), texturePath, xPos, yPos, cols, rows, 0);

		enemy->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, false); //Create physics body for enemy
		enemy->SetCollisionTag(Mechanism::Actor::CollisionTag::Enemy); //Set collision tag to indentify as enemy

        enemy->SetMovementPattern(movementPattern);

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

    void GameLevel::SpawnEnemyProjectile(float x, float y, float targetX, float targetY)
    {
        auto projectile = std::make_unique<EnemyProjectile>(m_Renderer->GetNativeRenderer(), "assets/EnWeap6.bmp", 
            x, y, targetX, targetY, 8, 1, 0);

        projectile->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, true);
        projectile->SetCollisionTag(Mechanism::Actor::CollisionTag::EnemyProjectile);

        m_EnemyProjectiles.push_back(std::move(projectile));

        printf("Enemy Projectile spawned at (%.0f, %.0f)\n\n", x, y);
    }

    std::function<void(Enemy*, float)> GameLevel::LonerMovement()
    {
        return [](Enemy* enemy, float direction)
            {
                if (enemy->HasPhysicsBody())
                {
                    int phase = ((int)(enemy->GetTimeAlive() / 10.0f)) % 2;
                    float direction = phase == 0 ? 1.0f : -1.0f; //1 right, -1 left
                    enemy->MoveInDirection(direction, 0.0f, 1.0f);
                }
            };
    }

    std::function<void(Enemy*, float)> GameLevel::RusherMovement()
    {
        return [](Enemy* enemy, float direction)
            {
                if (enemy->HasPhysicsBody())
                {
                    enemy->MoveInDirection(0.0f, 1.0f, 1.0f); //vertical only
                }
            };
    }

    std::function<void(Enemy*, float)> GameLevel::DroneMovement()
    {
        return [](Enemy* enemy, float deltaTime)
            {
                if(enemy->HasPhysicsBody())
                {
                    float waveSpeed = 3.0f; // How fast the wave oscillates
                    float waveAmplitude = 100.0f; // How wide the wave is
                    float moveSpeed = 1.5f; // How fast it moves down

                    // Calculate horizontal position using sine wave
                    float timeAlive = enemy->GetTimeAlive();
                    float waveOffset = sin(timeAlive * waveSpeed) * waveAmplitude;

                    float targetX = enemy->GetStartX() + waveOffset;

                    // Calculate direction to move horizontally
                    float currentX = enemy->GetX();
                    float directionX = (targetX > currentX) ? 1.0 : -1.0f;

                    // Move in wave pattern while going down
                    enemy->MoveInDirection(directionX, 1.0f, moveSpeed);
                }
            };
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
                if(m_Player)
                {
                    //Get the center of the player
                    float playerCenterX = m_Player->GetX() + (m_Player->GetFrameWidth() / 2.0f);
                    float playerCenterY = m_Player->GetY() + (m_Player->GetFrameHeight() / 2.0f);
                    enemy->SetPlayerPosition(playerCenterX, playerCenterY);
                }
                enemy->UpdateEnemy(deltaTime);
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

        //Update all enemy projectiles
        for (auto& enemyProjectile : m_EnemyProjectiles)
        {
            if (enemyProjectile)
            {
                enemyProjectile->UpdateEnemyProjectile(deltaTime);
                enemyProjectile->UpdateActor(deltaTime);
                enemyProjectile->ScaleActor(2.0f, 2.0f);
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

        //Remove off-screen enemy projectiles
        m_EnemyProjectiles.erase(
            std::remove_if(m_EnemyProjectiles.begin(), m_EnemyProjectiles.end(),
                [this](const std::unique_ptr<EnemyProjectile>& proj)
                {
                    return proj->IsDead() || proj->IsOffScreen(m_WindowHeight);
                }),
            m_EnemyProjectiles.end()
        );

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

        //Render all enemy projectiles
        for (const auto& enemyProjectile : m_EnemyProjectiles)
        {
            if (enemyProjectile)
            {
                enemyProjectile->Render(m_Renderer->GetNativeRenderer());
            }
        }
    }

   


 

 


