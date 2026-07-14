#pragma once
#include <raylib.h>
#include <vector>

class Player;

struct GrassBlade {
    Vector2 worldPos;
    float size;
};

struct Tree {
    Vector2 worldPos;
    float maxRadius;
    float radius;
    float targetRadius;
    int health;
    int maxHealth;
    Vector2 hitOffset = { 0.0f, 0.0f };
};

struct Rock {
    Vector2 worldPos;
    float maxRadius;
    float radius;
    float targetRadius;
    int health;
    int maxHealth;
    Vector2 hitOffset = { 0.0f, 0.0f };
};

struct Gold {
    Vector2 worldPos;
    float maxRadius;
    float radius;
    float targetRadius;
    int health;
    int maxHealth;
    Vector2 hitOffset = { 0.0f, 0.0f };
};

struct ResourceDrop {
    Vector2 worldPos;
    int type;
    float radius;
};

struct Mob {
    Vector2 worldPos;
    int type;
    int health;
    int maxHealth;
    float speed;
    float radius;

    float wanderTimer;
    Vector2 wanderTarget;
    Vector2 knockbackVelocity = { 0.0f, 0.0f };

    float angle;
};

class World {
private:
    void HandleCollisions(Player& p, Vector2& mapOffset, float oldOffsetX, float oldOffsetY);
    void HandleMobsCollisions(Player& p, const Vector2& mapOffset);
    void HandleAttackHitbox(Player& p, const Vector2& mapOffset);
    void HandleMeleeAnimation(Player& p, float deltaTime, float strikeFactor,
        float& leftHandOrbit, float& rightHandOrbit,
        float& leftHandAngleOffset, float& rightHandAngleOffset);
    void UpdateVisualEffects(float deltaTime);
    void UpdateWorldEntities(Player& p, const Vector2& mapOffset);

public:
    std::vector<GrassBlade> grassTiles;
    std::vector<Tree> trees;
    std::vector<Rock> rocks;
    std::vector<Gold> golds;
    std::vector<ResourceDrop> drops;
    std::vector<Mob> mobs;

    void Init();

    void Update(Player& p, Vector2& mapOffset, float deltaTime,
        Vector2& leftHandPos, Vector2& rightHandPos, float& handRadius);
};
