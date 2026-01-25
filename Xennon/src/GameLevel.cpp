#include "GameLevel.h"
#include <iostream>
#include <algorithm>


    GameLevel::GameLevel(Mechanism::Window& window): Level(0.0f, 0.0f), m_SpriteRenderer(&window.GetSpriteRenderer()),
        m_NativeWindow(window.GetNativeWindow()), m_WindowWidth(window.GetWidth()), m_WindowHeight(window.GetHeight()),
		m_Background(nullptr), m_Player(nullptr), m_EnemySpawnTimer(0.0f), m_EnemySpawnInterval(3.0f)
    {
        printf("\nGameLevel created!\n");

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        GetBox2DWorld().SetCollisionBeginCallback([this](Mechanism::Actor* actorA, Mechanism::Actor* actorB)
            {
                OnCollisionBegin(actorA, actorB);
			});

		AddBackground();
          
		SpawnPlayer(m_WindowWidth / 2.0f, m_WindowHeight - 100.0f);// Spawn player near bottom center

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
        auto background = std::make_unique<Mechanism::Actor>(m_NativeWindow, "assets/galaxy2.bmp", 0, 0, 1, 1, 0);

		m_Background = background.get();
        if(m_Background)
        {
            m_Background->ScaleActor(3.0f, 3.0f);
        }  
		m_Actors.push_back(std::move(background));

        printf("Background added\n\n");        
    }


    void GameLevel::SpawnEnemy(const char* texturePath, float xPos, float yPos, int cols, int rows,
        std::function<void(Enemy*, float)> movementPattern, Enemy::EnemyType type, int health)
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto enemy = std::make_unique<Enemy>(m_NativeWindow, texturePath, xPos, yPos, cols, rows, 0);

		enemy->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, false); //Create physics body for enemy
		enemy->SetCollisionTag(Mechanism::Actor::CollisionTag::Enemy); //Set collision tag to indentify as enemy

        enemy->SetMovementPattern(movementPattern);
        enemy->SetEnemyType(type);
        enemy->SetHealth(health);

		m_Enemies.push_back(std::move(enemy));

		printf("Enemy spawned at (%.0f, %.0f)\n\n", xPos, yPos);
    }


    void GameLevel::SpawnPlayer(float xPos, float yPos)
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto player = std::make_unique<Spaceship>(m_NativeWindow, "assets/Ship1.bmp", xPos, yPos, 7, 1, 3);

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
		auto projectile = std::make_unique<Projectile>(m_NativeWindow, "assets/missile.bmp", x, y, 1, 1, 0);

		projectile->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, true); //Create physics body for projectile
		projectile->SetCollisionTag(Mechanism::Actor::CollisionTag::Projectile); //Set collision tag to indentify as projectile

		m_Projectiles.push_back(std::move(projectile));
		printf("Projectile spawned at (%.0f, %.0f)\n\n", x, y);
    }


    void GameLevel::SpawnEnemyProjectile(float x, float y, float targetX, float targetY)
    {
        auto projectile = std::make_unique<EnemyProjectile>(m_NativeWindow, "assets/EnWeap6.bmp",
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

    std::function<void(Enemy*, float)> GameLevel::AsteroidMovement()
    {
        return [](Enemy* enemy, float direction)
            {
                if (enemy->HasPhysicsBody())
                {
                    enemy->MoveInDirection(0.0f, 1.0f, 0.5f); //vertical only
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

        //Enemy Spawner
        m_EnemySpawnTimer += deltaTime;
        if(m_EnemySpawnTimer >= m_EnemySpawnInterval)
        {
            //Pick a random enemie type 
            int randomType = std::rand() % 7;

            float spawnX = (std::rand() % (m_WindowWidth - 100)) + 50.0f;
            float spawnY = 50.0f;

            switch(randomType)
            {
                case 0: //Loner

                    /* variable order - texture location, location to spawn X and then Y, number of col and rows for the texture,
                    movement fucntion, Enemy Type(Enum class), Health */
                    SpawnEnemy("assets/LonerA.bmp", spawnX, spawnY, 4, 4, LonerMovement(), Enemy::EnemyType::Loner, 100);

                    //this enemy starts shooting
                    m_Enemies.back()->SetCanShoot(true);
                    m_Enemies.back()->SetShootInterval(2.0f);
                    m_Enemies.back()->SetEnemyShootCallback([this](float bulletX, float bulletY, float targetX, float targetY)
                        {
                            SpawnEnemyProjectile(bulletX, bulletY, targetX, targetY);
                        });
                    break;

                case 1:  // Rusher
                    SpawnEnemy("assets/rusher.bmp", spawnX, spawnY, 6, 4, RusherMovement(), Enemy::EnemyType::Rusher, 50);
                    break;

                case 2:  // Drone
                    SpawnEnemy("assets/drone.bmp", spawnX, spawnY, 8, 2, DroneMovement(), Enemy::EnemyType::Drone, 50);
                    m_Enemies.back()->ScaleActor(1.5f, 1.5f);
                    break;

                case 3: //Stone Asteroid
                    SpawnEnemy("assets/SAster96.bmp", spawnX, spawnY, 5, 5, AsteroidMovement(), Enemy::EnemyType::BigStoneAsteroid, 300);
                    m_Enemies.back()->ScaleActor(1.5f, 1.5f);
                    m_Enemies.back()->SetDeathCallback([this](float x, float y, Enemy::EnemyType type)
                        {
                            if(type == Enemy::EnemyType::BigStoneAsteroid)
                            {
                                SpawnEnemy("assets/SAster64.bmp", x - 50.0f, y, 8, 3, AsteroidMovement(), 
                                    Enemy::EnemyType::MediumStoneAsteroid, 200);
                                m_Enemies.back()->SetDeathCallback([this](float x, float y, Enemy::EnemyType type)
                                    {
                                        if(type == Enemy::EnemyType::MediumStoneAsteroid)
                                        {
                                            SpawnEnemy("assets/SAster32.bmp", x - 50.0f, y, 8, 2, AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 100);
                                            SpawnEnemy("assets/SAster32.bmp", x + 50.0f, y, 8, 2, AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 100);
                                        }
                                    });

                                SpawnEnemy("assets/SAster64.bmp", x + 50.0f, y, 8, 3, AsteroidMovement(),
                                    Enemy::EnemyType::MediumStoneAsteroid, 200);
                                m_Enemies.back()->SetDeathCallback([this](float x, float y, Enemy::EnemyType type)
                                    {
                                        if (type == Enemy::EnemyType::MediumStoneAsteroid)
                                        {
                                            SpawnEnemy("assets/SAster32.bmp", x - 50.0f, y, 8, 2, AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 100);
                                            SpawnEnemy("assets/SAster32.bmp", x + 50.0f, y, 8, 2, AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 100);
                                        }
                                    });
                            }
                        });
                    break;

                case 4: //Big Metal Asteroid
                    SpawnEnemy("assets/MAster96.bmp", spawnX, spawnY, 5, 5, AsteroidMovement(), Enemy::EnemyType::BigMetalAsteroid, 9999);
                    m_Enemies.back()->ScaleActor(1.5f, 1.5f);
                    break;

                case 5: //Medium Metal Asteroid
                    SpawnEnemy("assets/MAster64.bmp", spawnX, spawnY, 8, 3, AsteroidMovement(), Enemy::EnemyType::MediumMetalAsteroid, 9999);
                    break;

                case 6: //Small Metal Asteroid
                    SpawnEnemy("assets/MAster32.bmp", spawnX, spawnY, 8, 2, AsteroidMovement(), Enemy::EnemyType::SmallMetalAsteroid, 9999);
                    break;
            }

            m_EnemySpawnTimer = 0.0f;
        }

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
                actor->Render(m_SpriteRenderer);
            }
        }

        // Render all enemies
        for (const auto& enemy : m_Enemies)
        {
            if (enemy)
            {
                enemy->Render(m_SpriteRenderer);
            }
        }

        // Render all projectiles
        for (const auto& projectile : m_Projectiles)
        {
            if (projectile)
            {
                projectile->Render(m_SpriteRenderer);
            }
        }

        //Render all enemy projectiles
        for (const auto& enemyProjectile : m_EnemyProjectiles)
        {
            if (enemyProjectile)
            {
                enemyProjectile->Render(m_SpriteRenderer);
            }
        }
    }

   


 

 


