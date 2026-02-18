#include "GameLevel.h"
#include <iostream>
#include <algorithm>



GameLevel::GameLevel(Mechanism::Window& window) :
    Level(0.0f, 0.0f), m_Window(window), m_SpriteRenderer(&window.GetSpriteRenderer()), 
    m_NativeWindow(window.GetNativeWindow()), m_WindowWidth(window.GetWidth()), m_WindowHeight(window.GetHeight()),
	m_Background(nullptr), m_Player(nullptr), 
    m_EnemySpawnTimer(0.0f), m_EnemySpawnInterval(3.0f), 
    m_PowerUpSpawnTimer(0.0f), m_PowerUpSpawnInterval(5.0f)
    {
        printf("\nGameLevel created!\n");

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        GetBox2DWorld().SetCollisionBeginCallback([this](Mechanism::Actor* actorA, Mechanism::Actor* actorB)
            {
                OnCollisionBegin(actorA, actorB);
			});

		AddBackground();
          
		SpawnPlayer(100.0f, m_WindowHeight / 2.0f);// Spawn player near bottom center

        
        //Mudar a posicao da barra
        m_HealthBar = std::make_unique<Mechanism::HealthBar>(10, 10, m_WindowHeight-50, 30, 20, 5);
        

        // message at top left
        DisplayText("Player One:", 20.0f, 20.0f, 0.7f, 0.5f);

        // message at top center
        float hiScoreWidth = 9 * (21 * 0.7f + 0.5f);
        float centerX = (m_WindowWidth - hiScoreWidth) / 2.0f;
        DisplayText("Hi score:", centerX, 20.0f, 1.0f, 1.0f);
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
                m_NativeWindow,
                "assets/font16x16.bmp",
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

    }


    void GameLevel::SpawnPowerUp(const char* texturePath, float x, float y, int cols, int rows, PowerUps::PowerUpType type)
    {
        auto powerup = std::make_unique<PowerUps>(m_NativeWindow, texturePath, x, y, cols, rows, type, 0);
		powerup->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, false);
		powerup->SetCollisionTag(Mechanism::Actor::CollisionTag::PowerUp);

		m_PowerUps.push_back(std::move(powerup));
    }

    void GameLevel::SpawnExplosion(float x, float y)
    {
		auto explosion = std::make_unique<Mechanism::Actor>(m_NativeWindow, "assets/explode16.bmp", x, y, 5, 2, 0);

		explosion->SetAnimationSpeed(20.0f);
		explosion->SetPlayOnce(true);
		explosion->ScaleActor(1.5f, 1.5f);  

		m_Effects.push_back(std::move(explosion));
    }
    

    void GameLevel::SpawnPlayer(float xPos, float yPos)
    {
        // x   y  col row 0=its the srite in the col0 row0, the first sprite
        auto player = std::make_unique<Spaceship>(m_NativeWindow, "assets/Ship1.bmp", xPos, yPos, 7, 1, 3);

		player->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, false); //Create physics body for player
		player->SetCollisionTag(Mechanism::Actor::CollisionTag::Player); //Set collision tag to indentify as player
		player->SetSpeed(5.0f); // Set player speed
		player->SetRotation(90.0f); 

		// Set up shooting callback
        player->SetShootCallback([this](float x, float y, int damage)
        {
            SpawnProjectile(x, y, damage);
		});

		m_Player = player.get();
		m_Actors.push_back(std::move(player));
  
		printf("\Player spawned at (%.0f, %.0f)\n\n", xPos, yPos);
    }


    void GameLevel::SpawnProjectile(float x, float y, int damage)
    {
		auto projectile = std::make_unique<Projectile>(m_NativeWindow, "assets/missile.bmp", x, y, 1, 3, 0, damage);

		projectile->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, true); 
		projectile->SetCollisionTag(Mechanism::Actor::CollisionTag::Projectile); 
        projectile->SetRotation(90.0f);
		projectile->ScaleActor(1.5f, 1.5f);
		projectile->SetAnimationEnabled(false); 

        projectile->SetExplosionCallback([this](float x, float y) {
            SpawnExplosion(x, y);  
            });

		m_Projectiles.push_back(std::move(projectile));
    }


    void GameLevel::SpawnEnemyProjectile(float x, float y, 
        float targetX, float targetY)
    {
        auto projectile = std::make_unique<EnemyProjectile>(m_NativeWindow, "assets/EnWeap6.bmp",
            x, y, targetX, targetY, 8, 1, 0);

        projectile->CreatePhysicsBody(GetBox2DWorld().GetWorldId(), true, true);
        projectile->SetCollisionTag(Mechanism::Actor::CollisionTag::EnemyProjectile);

        m_EnemyProjectiles.push_back(std::move(projectile));

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

        if(m_Player && m_HealthBar)
        {
			m_HealthBar->SetCurrentHealth(m_Player->GetCurrentHealth());
			m_HealthBar->SetMaxHealth(m_Player->GetMaxHealth());
        }

        m_PowerUpSpawnTimer += deltaTime;
        if(m_PowerUpSpawnTimer >= m_PowerUpSpawnInterval)
        {
            float spawnX = m_WindowWidth + 50.0f;
            float spawnY = (std::rand() % (m_WindowHeight - 100)) + 50.0f;
            int randomType = std::rand() % 2;
            switch(randomType)
            {
                case 0:
                    SpawnPowerUp("assets/PUShield.bmp", spawnX, spawnY, 4, 2, PowerUps::PowerUpType::Shield);

                    m_PowerUps.back()->SetEffectCallback([this](PowerUps::PowerUpType type)
                        {
                            if(type == PowerUps::PowerUpType::Shield)
                            {
								m_Player->ApplyShieldPowerUp();
                            }
						});

                    break;
                case 1:
                    SpawnPowerUp("assets/PUWeapon.bmp", spawnX, spawnY, 4, 2, PowerUps::PowerUpType::Weapon);
                     m_PowerUps.back()->SetEffectCallback([this](PowerUps::PowerUpType type)
                        {
                            if(type == PowerUps::PowerUpType::Weapon)
                            {
                                m_Player->ApplyWeaponPowerUp();
							}
                         });
                    break;
            }
            m_PowerUpSpawnTimer = 0.0f;
		}

        //Enemy Spawner
        m_EnemySpawnTimer += deltaTime;
        if(m_EnemySpawnTimer >= m_EnemySpawnInterval)
        {
            //Pick a random enemie type 
            int randomType = std::rand() % 7;

            float spawnX = m_WindowWidth + 50.0f;
            float spawnY = (std::rand() % (m_WindowHeight - 100)) + 50.0f;

            switch(randomType)
            {
                case 0: //Loner

                    /* variable order - texture location, location to spawn X and then Y, number of col and rows for the texture,
                    movement fucntion, Enemy Type(Enum class), Health */
                    SpawnEnemy("assets/LonerA.bmp", spawnX, spawnY, 4, 4, Enemy::LonerMovement(), Enemy::EnemyType::Loner, 15);

                    m_Enemies.back()->SetScreenBounds(50.0f, m_WindowHeight - 50.0f);
                    //this enemy starts shooting
                    m_Enemies.back()->SetCanShoot(true);
                    m_Enemies.back()->SetShootInterval(2.0f);
                    m_Enemies.back()->SetEnemyShootCallback([this](float bulletX, float bulletY, float targetX, float targetY)
                        {
                            SpawnEnemyProjectile(bulletX, bulletY, targetX, targetY);
                        });
                    break;

                case 1:  // Rusher
                    SpawnEnemy("assets/rusher.bmp", spawnX, spawnY, 4, 6, Enemy::RusherMovement(), Enemy::EnemyType::Rusher, 10);
                    break;

                case 2:  // Drone
                    SpawnEnemy("assets/drone.bmp", spawnX, spawnY, 8, 2, Enemy::DroneMovement(), Enemy::EnemyType::Drone, 10);
                    m_Enemies.back()->ScaleActor(1.5f, 1.5f);
                    break;

                case 3: //Stone Asteroid
                    SpawnEnemy("assets/SAster96.bmp", spawnX, spawnY, 5, 5, Enemy::AsteroidMovement(), Enemy::EnemyType::BigStoneAsteroid, 30);
                    m_Enemies.back()->ScaleActor(1.5f, 1.5f);
                    m_Enemies.back()->SetDeathCallback([this](float x, float y, Enemy::EnemyType type)
                        {
                            if(type == Enemy::EnemyType::BigStoneAsteroid)
                            {
                                SpawnEnemy("assets/SAster64.bmp", x - 50.0f, y, 8, 3, Enemy::AsteroidMovement(),
                                    Enemy::EnemyType::MediumStoneAsteroid, 20);
                                m_Enemies.back()->SetDeathCallback([this](float x, float y, Enemy::EnemyType type)
                                    {
                                        if(type == Enemy::EnemyType::MediumStoneAsteroid)
                                        {
                                            SpawnEnemy("assets/SAster32.bmp", x - 50.0f, y, 8, 2, Enemy::AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 10);
                                            SpawnEnemy("assets/SAster32.bmp", x + 50.0f, y, 8, 2, Enemy::AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 10);
                                        }
                                    });

                                SpawnEnemy("assets/SAster64.bmp", x + 50.0f, y, 8, 3, Enemy::AsteroidMovement(),
                                    Enemy::EnemyType::MediumStoneAsteroid, 20);
                                m_Enemies.back()->SetDeathCallback([this](float x, float y, Enemy::EnemyType type)
                                    {
                                        if (type == Enemy::EnemyType::MediumStoneAsteroid)
                                        {
                                            SpawnEnemy("assets/SAster32.bmp", x - 50.0f, y, 8, 2, Enemy::AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 10);
                                            SpawnEnemy("assets/SAster32.bmp", x + 50.0f, y, 8, 2, Enemy::AsteroidMovement(),
                                                Enemy::EnemyType::SmallStoneAsteroid, 10);
                                        }
                                    });
                            }
                        });
                    break;

                case 4: //Big Metal Asteroid
                    SpawnEnemy("assets/MAster96.bmp", spawnX, spawnY, 5, 5, Enemy::AsteroidMovement(), Enemy::EnemyType::BigMetalAsteroid, 9999);
                    m_Enemies.back()->ScaleActor(1.5f, 1.5f);
                    break;

                case 5: //Medium Metal Asteroid
                    SpawnEnemy("assets/MAster64.bmp", spawnX, spawnY, 8, 3, Enemy::AsteroidMovement(), Enemy::EnemyType::MediumMetalAsteroid, 9999);
                    break;

                case 6: //Small Metal Asteroid
                    SpawnEnemy("assets/MAster32.bmp", spawnX, spawnY, 8, 2, Enemy::AsteroidMovement(), Enemy::EnemyType::SmallMetalAsteroid, 9999);
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
		//Update all powerups
        for (auto& powerup : m_PowerUps)
        {
            if (powerup)
            {
                powerup->UpdatePowerUp(deltaTime);
                powerup->UpdateActor(deltaTime);
            }
		}

        // Update effects
        for (auto& effect : m_Effects)
        {
            if (effect) effect->UpdateActor(deltaTime);
        }

		//Remove dead enemies
        m_Enemies.erase(
            std::remove_if(m_Enemies.begin(), m_Enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy)
                {
                    return enemy->IsDead() || enemy->GetX() < -200.0f;
                }),
            m_Enemies.end()
		);

		//Remove off-screen projectiles
        m_Projectiles.erase(
            std::remove_if(m_Projectiles.begin(), m_Projectiles.end(),
                [this](const std::unique_ptr<Projectile>& proj)
                {
                    return proj->IsDead() || proj->IsOffScreen(m_WindowWidth); 
                }),
            m_Projectiles.end()
		);

        //Remove off-screen enemy projectiles
        m_EnemyProjectiles.erase(
            std::remove_if(m_EnemyProjectiles.begin(), m_EnemyProjectiles.end(),
                [this](const std::unique_ptr<EnemyProjectile>& proj)
                {
                    return proj->IsDead() || proj->IsOffScreen(m_WindowWidth);
                }),
            m_EnemyProjectiles.end()
        );

		//Remove off-screen or collected powerups
        m_PowerUps.erase(
            std::remove_if(m_PowerUps.begin(), m_PowerUps.end(),
                [this](const std::unique_ptr<PowerUps>& powerup)
                {
                    return powerup->IsDead() || powerup->IsOffScreen(m_WindowWidth);
                }),
            m_PowerUps.end()
		);

        // Remove effects mortos
        m_Effects.erase(
            std::remove_if(m_Effects.begin(), m_Effects.end(),
                [](const std::unique_ptr<Mechanism::Actor>& effect) {
                    return effect->IsDead();
                }),
            m_Effects.end()
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

        // Render all enemy projectiles
        for (const auto& enemyProjectile : m_EnemyProjectiles)
        {
            if (enemyProjectile)
            {
                enemyProjectile->Render(m_SpriteRenderer);
            }
        }

		//Render all powerups
        for (const auto& powerup : m_PowerUps)
        {
            if (powerup)
            {
                powerup->Render(m_SpriteRenderer);
            }
		}

        // Render all letters (text)
        for (auto* letter : m_Letters)
        {
            if (letter && letter->IsValid())
            {
                letter->Render(m_SpriteRenderer);
            }
        }

        // Render effects   
        for (const auto& effect : m_Effects)
        {
            if (effect) effect->Render(m_SpriteRenderer);
        }


        if (m_HealthBar)
        {
			int currentHeight = m_Window.GetHeight();
			m_HealthBar->SetPosition(10, currentHeight - 50);
            m_HealthBar->Render(m_SpriteRenderer);
        }
        else
        {
            printf("ERROR: m_HealthBar is null!\n");
        }
    }
