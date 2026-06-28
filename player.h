#pragma once
#include <raylib.h>

class Player {
private:
    Vector2 screenCenter;
    float baseSpeed;
    float speed;
    Vector2 velocity;
    float baseRadius;
    float radius;
    float angle;

    int health;
    int maxHealth;
    float stamina;
    float maxStamina;
    float hpRegenTimer;

    bool isJumping;
    float jumpTimer;
    float jumpDuration;
    float jumpCooldown;
    float cooldownTimer;

    bool isAttacking;
    float attackTimer;
    float attackDuration;
    int attackCounter;
    float attackCooldown;
    float attackCooldownTimer;

    int woodCount;
    int stoneCount;
    int goldCount;
    int axeTier;
    int pickaxeTier;
    int activeWeapon;

    float dropPickupTimer;

public:
    Player();

    void HandleMovement(float deltaTime);
    void Update(float deltaTime);
    void HandleInput();

    Vector2 GetScreenCenter() const { return screenCenter; }
    float GetRadius() const { return radius; }
    float GetAngle() const { return angle; }
    float GetSpeed() const { return speed; }
    Vector2& GetVelocity() { return velocity; }
    int GetHealth() const { return health; }
    int GetMaxHealth() const { return maxHealth; }
    float GetStamina() const { return stamina; }
    float GetMaxStamina() const { return maxStamina; }
    bool IsAttacking() const { return isAttacking; }
    bool IsJumping() const { return isJumping; }
    float GetJumpTimer() const { return jumpTimer; }
    float GetJumpDuration() const { return jumpDuration; }
    float GetAttackCooldownTimer() const { return attackCooldownTimer; }
    int GetAttackCounter() const { return attackCounter; }
    int GetActiveWeapon() const { return activeWeapon; }
    int GetAxeTier() const { return axeTier; }
    int GetPickaxeTier() const { return pickaxeTier; }
    int GetWoodCount() const { return woodCount; }
    int GetStoneCount() const { return stoneCount; }
    int GetGoldCount() const { return goldCount; }
    float GetDropPickupTimer() const { return dropPickupTimer; }

    void TakeDamage(int amount)
    {
        health -= amount;
        if (health < 0)
            health = 0;
    }

    void AddWood(int count) { woodCount += count; }
    void AddStone(int count) { stoneCount += count; }
    void AddGold(int count) { goldCount += count; }
    void ResetDropPickupTimer(float time) { dropPickupTimer = time; }

    void TriggerAttack();
};
