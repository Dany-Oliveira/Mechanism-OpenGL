```mermaid
classDiagram
    %% ===== CORE ENGINE CLASSES =====
    
    class Application {
        <<abstract>>
        #Window m_Window
        -bool m_Running
        +Run() void
        +OnRender()* void
        +OnUpdate(deltaTime)* void
        +GetWindow() Window&
    }
    
    class Window {
        -SDL_Window* m_Window
        -void* m_GLContext
        -SpriteRenderer m_SpriteRenderer
        -WindowData m_Data
        +OnUpdate() void
        +SwapBuffers() void
        +GetWidth() uint
        +GetHeight() uint
        +GetSpriteRenderer() SpriteRenderer&
    }
    
    class SpriteRenderer {
        -uint m_VAO
        -uint m_VBO
        -uint m_ShaderProgram
        -uint m_ScreenWidth
        -uint m_ScreenHeight
        +Initialize(width, height) void
        +DrawSprite(texture, x, y, ...) void
        +DrawColoredRectangle(x, y, ...) void
    }
    
    class Texture {
        -uint m_TextureID
        -string m_Filepath
        -int m_Width
        -int m_Height
        +Bind(slot) void
        +Unbind() void
        +GetWidth() int
        +GetHeight() int
        +isValid() bool
    }
    
    class HealthBar {
        -int m_CurrentHealth
        -int m_MaxHealth
        -int m_X
        -int m_Y
        -Color m_CurrentColor
        +Render(renderer) void
        +SetCurrentHealth(health) void
        +GetCurrentHealth() int
    }
    
    class Input {
        <<static>>
        +IsKeyBeingPressed(keyCode)$ bool
        +IsKeyPressed(keyCode)$ bool
        +IsKeyReleased(keyCode)$ bool
        +Update()$ void
        +OnKeyDown(keyCode)$ void
        +OnKeyUp(keyCode)$ void
    }
    
    %% ===== PHYSICS CLASSES =====
    
    class Box2DWorld {
        -void* worldId_
        -function~callback~ m_CollisionBeginCallback
        +Step(timeStep, subSteps) void
        +GetWorldId() void*
        +ProcessContactEvents() void
        +SetCollisionBeginCallback(callback) void
    }
    
    class Box2DBody {
        -void* bodyId_
        +CreateDynamic(worldId, x, y, isBullet) void
        +CreateStatic(worldId, x, y) void
        +AddBoxShape(halfW, halfH, density, friction) void
        +SetLinearVelocity(vx, vy) void
        +GetPosition(x, y) void
        +SetUserData(data) void
        +GetUserData() void*
        +IsValid() bool
    }
    
    %% ===== LEVEL SYSTEM =====
    
    class Level {
        <<abstract>>
        #Box2DWorld m_Box2DWorld
        -float m_Accumulator
        -float m_TimeStep
        +Update(deltaTime) void
        +Render()* void
        +GetBox2DWorld() Box2DWorld&
    }
    
    %% ===== ACTOR HIERARCHY =====
    
    class Actor {
        <<base>>
        #Texture m_Texture
        #Box2DBody m_Box2DBody
        #float m_X
        #float m_Y
        #float m_ScaleX
        #float m_ScaleY
        #int m_CurrentFrame
        #bool m_AnimationEnabled
        #bool isDead
        #CollisionTag m_CollisionTag
        +Render(renderer) void
        +UpdateActor(deltaTime) void
        +SetPosition(x, y) void
        +CreatePhysicsBody(worldId, isDynamic, isBullet) void
        +SyncPhysicsToVisual() void
        +SetAnimationEnabled(enabled) void
        +SetPlayOnce(once) void
        +OnCollisionBegin(other)* void
        +GetCollisionTag() CollisionTag
        +IsDead() bool
    }
    
    class Pawn {
        #float speed
        +MoveInDirection(dirX, dirY, speed) void
        +Stop() void
        +UpdatePawn(deltaTime)* void
    }
    
    class Spaceship {
        -int m_MaxHealth
        -int m_CurrentHealth
        -int m_ProjectileDamage
        -float m_ShootCooldown
        -float m_ShootCooldownTime
        -function~shootCallback~ m_ShootCallback
        +PlayerUpdate(deltaTime) void
        +Shoot() void
        +TakeDamage(damage) void
        +ApplyShieldPowerUp() void
        +ApplyWeaponPowerUp() void
        +OnCollisionBegin(other) void
    }
    
    class Enemy {
        -int health
        -float timeAlive
        -float startX
        -float startY
        -bool m_CanShoot
        -EnemyType m_EnemyType
        -function~movement~ m_MovementPattern
        -function~shootCallback~ m_EnemyShootCallback
        -function~deathCallback~ m_DeathCallback
        +UpdateEnemy(deltaTime) void
        +TakeDamage(damage) void
        +OnCollisionBegin(other) void
        +SetMovementPattern(pattern) void
        +LonerMovement()$ function
        +RusherMovement()$ function
        +DroneMovement()$ function
        +AsteroidMovement()$ function
    }
    
    class Projectile {
        -float velocityY
        -int m_Damage
        -function~explosionCallback~ m_ExplosionCallback
        +UpdateProjectile(deltaTime) void
        +IsOffScreen(screenHeight) bool
        +OnCollisionBegin(other) void
        +GetDamage() int
        +SetExplosionCallback(callback) void
    }
    
    class EnemyProjectile {
        -float velocityX
        -float velocityY
        -float m_Speed
        +UpdateEnemyProjectile(deltaTime) void
        +IsOffScreen(screenHeight) bool
        +OnCollisionBegin(other) void
    }
    
    class PowerUps {
        -float velocityY
        -PowerUpType m_PowerUpType
        -function~effectCallback~ m_EffectCallback
        +UpdatePowerUp(deltaTime) void
        +IsOffScreen(screenHeight) bool
        +OnCollisionBegin(other) void
        +SetEffectCallback(callback) void
    }
    
    %% ===== GAME SPECIFIC CLASSES =====
    
    class GameLevel {
        -vector~Actor~ m_Actors
        -vector~Enemy~ m_Enemies
        -vector~Projectile~ m_Projectiles
        -vector~EnemyProjectile~ m_EnemyProjectiles
        -vector~PowerUps~ m_PowerUps
        -vector~Actor~ m_Effects
        -Spaceship* m_Player
        -Actor* m_Background
        -HealthBar m_HealthBar
        -float m_EnemySpawnTimer
        -float m_PowerUpSpawnTimer
        +UpdateGameLevel(deltaTime) void
        +Render() void
        +SpawnPlayer(x, y) void
        +SpawnEnemy(...) void
        +SpawnProjectile(x, y, damage) void
        +SpawnEnemyProjectile(...) void
        +SpawnPowerUp(...) void
        +SpawnExplosion(x, y) void
    }
    
    class Xennon {
        -GameLevel m_CurrentLevel
        +OnUpdate(deltaTime) void
        +OnRender() void
    }
    
    %% ===== ENUMS =====
    
    class CollisionTag {
        <<enumeration>>
        None
        Player
        Enemy
        Projectile
        Background
        EnemyProjectile
        PowerUp
    }
    
    class EnemyType {
        <<enumeration>>
        Loner
        Rusher
        Drone
        BigStoneAsteroid
        MediumStoneAsteroid
        SmallStoneAsteroid
        BigMetalAsteroid
        MediumMetalAsteroid
        SmallMetalAsteroid
    }
    
    class PowerUpType {
        <<enumeration>>
        None
        Weapon
        Shield
    }
    
    %% ===== INHERITANCE RELATIONSHIPS =====
    
    Application <|-- Xennon
    Level <|-- GameLevel
    Actor <|-- Pawn
    Actor <|-- Projectile
    Actor <|-- EnemyProjectile
    Actor <|-- PowerUps
    Pawn <|-- Spaceship
    Pawn <|-- Enemy
    
    %% ===== COMPOSITION RELATIONSHIPS =====
    
    Application *-- Window : contains
    Window *-- SpriteRenderer : contains
    Actor *-- Texture : contains
    Actor *-- Box2DBody : contains
    Level *-- Box2DWorld : contains
    GameLevel *-- HealthBar : contains
    
    %% ===== AGGREGATION RELATIONSHIPS =====
    
    GameLevel o-- Spaceship : manages
    GameLevel o-- Enemy : manages
    GameLevel o-- Projectile : manages
    GameLevel o-- EnemyProjectile : manages
    GameLevel o-- PowerUps : manages
    GameLevel o-- Actor : manages effects
    Xennon o-- GameLevel : has
    
    %% ===== ASSOCIATIONS =====
    
    SpriteRenderer ..> Texture : uses
    Actor ..> SpriteRenderer : uses
    Actor ..> CollisionTag : uses
    Enemy ..> EnemyType : uses
    PowerUps ..> PowerUpType : uses
    Box2DWorld ..> Actor : collision callbacks
    GameLevel ..> Box2DWorld : uses



- All physics bodies are synchronized with visual positions every frame
- Actors can exist without physics bodies (e.g., background, UI elements)
- Enemy movement patterns are static functions returning lambdas
- Damage system is dynamic based on power-ups collected
