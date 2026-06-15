#pragma once
#include <cmath>
#include <vector>
#include "common.h"
#include "structures.h"
#include "player.h"
#include "movement.h"
#include "collisions.h"
#include "combat.h"
#include "inventory.h"

inline void UpdateGame(Player& p, Vector2& mapOffset,
        std::vector<Tree>& trees, std::vector<Rock>& rocks, std::vector<ResourceDrop>& drops,
        float deltaTime, Vector2& leftHandPos, Vector2& rightHandPos, float& handRadius){
    
    float oldOffsetX = mapOffset.x;
    float oldOffsetY = mapOffset.y;

    HandlePlayerMovement(p, mapOffset, deltaTime);    
    HandleCollisions(p, mapOffset, oldOffsetX, oldOffsetY, trees, rocks);

    if(p.attackCooldownTimer > 0.0f) p.attackCooldownTimer -= deltaTime;
    if(p.dropPickupTimer > 0.0f) p.dropPickupTimer -= deltaTime;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !p.isAttacking && p.attackCooldownTimer <= 0.0f){
        p.isAttacking = true;
        p.attackTimer = p.attackDuration;
        p.attackCooldownTimer = p.attackCooldown;
        p.dropPickupTimer = 0.25f;
        
        if (p.attackCounter == 0) p.attackCounter = 1; 
        else p.attackCounter = 0;

        HandleAttackHitbox(p, mapOffset, trees, rocks, drops);
    }

    HandleWeaponSelectionAndCraft(p);

    Vector2 mousePos = GetMousePosition();
    p.angle = atan2f(mousePos.y - p.screenCenter.y, mousePos.x - p.screenCenter.x);

    float leftHandOrbit = p.radius + 6.0f;
    float rightHandOrbit = p.radius + 6.0f;
    float leftHandAngleOffset = 0.7f;
    float rightHandAngleOffset = 0.7f;
    float strikeFactor = 0.0f;

    if (p.isAttacking) {
        p.attackTimer -= deltaTime;
        if (p.attackTimer < 0.0f) p.isAttacking = false;
        float progress = p.attackTimer / p.attackDuration;
        strikeFactor = sinf(progress * PI);
    }

    if(p.activeWeapon > 0){
        if (p.isAttacking) {
            rightHandOrbit += strikeFactor * 22.0f;
            rightHandAngleOffset -= strikeFactor * 0.2f;
            leftHandOrbit -= strikeFactor * 8.0f;
            leftHandAngleOffset += strikeFactor * 0.3f;
        }
    }
    else {
        if(p.isAttacking){
            HandleMeleeAnimation(p, deltaTime, strikeFactor,
                                 leftHandOrbit, rightHandOrbit, 
                                 leftHandAngleOffset, rightHandAngleOffset);
        }
    }

    leftHandPos = {
        p.screenCenter.x + leftHandOrbit * cosf(p.angle - leftHandAngleOffset),
        p.screenCenter.y + leftHandOrbit * sinf(p.angle - leftHandAngleOffset)
    };
    rightHandPos = {
        p.screenCenter.x + rightHandOrbit * cosf(p.angle + rightHandAngleOffset),
        p.screenCenter.y + rightHandOrbit * sinf(p.angle + rightHandAngleOffset)
    };
        
    handRadius = p.isJumping ? 10.0f : 8.0f;

    UpdateVisualEffects(trees, rocks);
    UpdateWorldEntities(p, mapOffset, trees, rocks, drops);
}
