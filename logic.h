#pragma once
#include <cmath>
#include <vector>
#include "common.h"
#include "structures.h"
#include "player.h"

inline void UpdateGame(Player& p, Vector2& mapOffset,
        std::vector<Tree>& trees, std::vector<Rock>& rocks, std::vector<ResourceDrop>& drops,
        float deltaTime, Vector2& leftHandPos, Vector2& rightHandPos, float& handRadius){
    
    float oldOffsetX = mapOffset.x;
    if(IsKeyDown(KEY_A)) mapOffset.x -= p.speed;
    if(IsKeyDown(KEY_D)) mapOffset.x += p.speed;
    
    if(mapOffset.x < -MAP_HALF) mapOffset.x = -MAP_HALF;
    if(mapOffset.x > MAP_HALF) mapOffset.x = MAP_HALF;

    for(const auto& t : trees){
        float dx = t.worldPos.x - mapOffset.x;
        float dy = t.worldPos.y - mapOffset.y;
        if(sqrt(dx*dx + dy*dy) < p.radius + t.radius - 10.0f){
            mapOffset.x = oldOffsetX;
            break;
        }
    }
    for(const auto& r : rocks){
        float dx = r.worldPos.x - mapOffset.x;
        float dy = r.worldPos.y - mapOffset.y;
        if(sqrt(dx*dx + dy*dy) < p.radius + r.radius - 10.0f){
            mapOffset.x = oldOffsetX;
            break;
        }
    }

    float oldOffsetY = mapOffset.y;
    if(IsKeyDown(KEY_W)) mapOffset.y -= p.speed;
    if(IsKeyDown(KEY_S)) mapOffset.y += p.speed;

    if(mapOffset.y < -MAP_HALF) mapOffset.y = -MAP_HALF;
    if(mapOffset.y > MAP_HALF) mapOffset.y = MAP_HALF;

    for(const auto& t : trees){
        float dx = t.worldPos.x - mapOffset.x;
        float dy = t.worldPos.y - mapOffset.y;
        if(sqrt(dx*dx + dy*dy) < p.radius + t.radius){
            mapOffset.y = oldOffsetY;
            break;
        }
    }
    for(const auto& r : rocks){
        float dx = r.worldPos.x - mapOffset.x;
        float dy = r.worldPos.y - mapOffset.y;
        if(sqrt(dx*dx + dy*dy) < p.radius + r.radius){
            mapOffset.y = oldOffsetY;
            break;
        }
    }

    if(IsKeyDown(KEY_SPACE) && !p.isJumping && p.cooldownTimer <= 0.0f){
        p.isJumping = true;
        p.jumpTimer = p.jumpDuration;
        p.radius = p.baseRadius + 10.0f;
        p.speed = p.baseSpeed + 3.0f;
    }

    if(p.isJumping){
        p.jumpTimer -= deltaTime;
        if(p.jumpTimer <= 0.0f){
            p.isJumping = false;
            p.radius = p.baseRadius;
            p.speed = p.baseSpeed;
            p.cooldownTimer = p.jumpCooldown;
        }
    }
    else if(p.cooldownTimer > 0.0f) p.cooldownTimer -= deltaTime;

    if(p.attackCooldownTimer > 0.0f) p.attackCooldownTimer -= deltaTime;
    if(p.dropPickupTimer > 0.0f) p.dropPickupTimer -= deltaTime;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !p.isAttacking && p.attackCooldownTimer <= 0.0f){
        p.isAttacking = true;
        p.attackTimer = p.attackDuration;
        p.attackCooldownTimer = p.attackCooldown;
        p.attackCounter++;
	p.dropPickupTimer = 0.25f;

        int damage = 1;
        if(p.activeWeapon == 1) damage = p.axeTier * GetRandomValue(1, 3);
	
	Vector2 mousePos = GetMousePosition();
	float attackAngle = atan2f(mousePos.y - p.screenCenter.y, mousePos.x - p.screenCenter.x);

	Vector2 hitWorldPos = {
		mapOffset.x + cosf(attackAngle) * (p.radius + 35.0f),
		mapOffset.y + sinf(attackAngle) * (p.radius + 35.0f)
        };

        for(size_t i = 0; i < trees.size(); i++){
            float dx = trees[i].worldPos.x - hitWorldPos.x;
            float dy = trees[i].worldPos.y - hitWorldPos.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if(dist < trees[i].radius + 40.0f){
                trees[i].health -= damage;

		float screenTreeX = trees[i].worldPos.x - mapOffset.x + p.screenCenter.x;
		float screenTreeY = trees[i].worldPos.y - mapOffset.y + p.screenCenter.y;
		float pushAngle = atan2f(screenTreeY - p.screenCenter.y, screenTreeX - p.screenCenter.x);

		trees[i].hitOffset.x = cosf(pushAngle) * 12.0f;
		trees[i].hitOffset.y = sinf(pushAngle) * 12.0f;

                ResourceDrop drop;
                drop.type = 1;
                drop.radius = 15.0f;

		float randomSpread = (GetRandomValue(-45, 45) * PI) / 180.0f;
		float dropAngle = pushAngle + PI + randomSpread;

                drop.worldPos.x = trees[i].worldPos.x - cosf(pushAngle) * trees[i].radius + cosf(dropAngle) * 35.0f;
                drop.worldPos.y = trees[i].worldPos.y - sinf(pushAngle) * trees[i].radius + sinf(dropAngle) * 35.0f;

		//drop.worldPos.x += cosf(dropAngle) * 25.0f;
		//drop.worldPos.y += sinf(dropAngle) * 25.0f;

                drops.push_back(drop);
                break;
            }
        }

        int mineDamage = 0;
        if(p.activeWeapon == 2) mineDamage = p.pickaxeTier * GetRandomValue(1, 3);

        if(mineDamage > 0){
            for(size_t i = 0; i < rocks.size(); i++){
                float dx = rocks[i].worldPos.x - hitWorldPos.x;
                float dy = rocks[i].worldPos.y - hitWorldPos.y;
                float dist = sqrtf(dx*dx + dy*dy);

                if(dist < rocks[i].radius + 40.0f){
                    rocks[i].health -= mineDamage;

		    float screenRockX = rocks[i].worldPos.x - mapOffset.x + p.screenCenter.x;
		    float screenRockY = rocks[i].worldPos.y - mapOffset.y + p.screenCenter.y;
		    float pushAngle = atan2f(screenRockY - p.screenCenter.y, screenRockX - p.screenCenter.x);

		    rocks[i].hitOffset.x = cosf(pushAngle) * 12.0f;
		    rocks[i].hitOffset.y = sinf(pushAngle) * 12.0f;

                    ResourceDrop drop;
                    drop.type = 2;
                    drop.radius = 15.0f;

		    float randomSpread = (GetRandomValue(-45, 45) * PI) / 180.0f;
		    float dropAngle = pushAngle + PI + randomSpread;

                    drop.worldPos.x = rocks[i].worldPos.x - cosf(pushAngle) * rocks[i].radius + cosf(dropAngle) * 35.0f;
                    drop.worldPos.y = rocks[i].worldPos.y - sinf(pushAngle) * rocks[i].radius + sinf(dropAngle) * 35.0f;

		    //drop.worldPos.x += cosf(dropAngle) * 25.0f;
		    //drop.worldPos.y += sinf(dropAngle) * 25.0f;

                    drops.push_back(drop);
                    break;
                }
            }
        }
    }

    if(IsKeyPressed(KEY_ONE)){
        if(p.axeTier == 0){
            if(p.woodCount >= 10){
                p.woodCount -= 10;
                p.axeTier = 1;
                p.activeWeapon = 1;
            }
        }
        else if(p.axeTier == 1 && p.woodCount >= 10 && p.stoneCount >= 15){
            p.woodCount -= 10;
            p.stoneCount -= 15;
            p.axeTier = 2;
            p.activeWeapon = 1;
        }
        else if(p.axeTier > 0){
            p.activeWeapon = 1;
        }
    }
    
    if(IsKeyPressed(KEY_TWO)){
        if(p.pickaxeTier == 0){
            if(p.woodCount >= 10){
                p.woodCount -= 10;
                p.pickaxeTier = 1;
                p.activeWeapon = 2;
            }
        }
        else if(p.pickaxeTier == 1 && p.woodCount >= 15 && p.stoneCount >= 15){
            p.woodCount -= 15;
            p.stoneCount -= 15;
            p.pickaxeTier = 2;
            p.activeWeapon = 2;
        }
        else if(p.pickaxeTier > 0){
            p.activeWeapon = 2;
        }
    }

    if(IsKeyPressed(KEY_THREE)) p.activeWeapon = 0;

    Vector2 mousePos = GetMousePosition();
    p.angle = atan2(mousePos.y - p.screenCenter.y, mousePos.x - p.screenCenter.x);

    float leftHandOrbit = p.radius + 6.0f;
    float rightHandOrbit = p.radius + 6.0f;
    float leftHandAngleOffset = 0.7f;
    float rightHandAngleOffset = 0.7f;

    if(p.activeWeapon > 0){
	    //leftHandAngleOffset = 0.7f;
	    //leftHandOrbit = p.radius + 2.0f;

    		if(p.isAttacking){
        		p.attackTimer -= deltaTime;
        		if(p.attackTimer < 0.0f) p.isAttacking = false;

			float progress = p.attackTimer / p.attackDuration;
			float strikeFactor = sinf(progress * PI);

			rightHandOrbit += strikeFactor * 22.0f;
			rightHandAngleOffset -= strikeFactor * 0.2f;

			leftHandOrbit -= strikeFactor * 8.0f;
			leftHandAngleOffset += strikeFactor * 0.3f;
		}
    }
    else{
		if(p.isAttacking){
		    	p.attackTimer -= deltaTime;
			if(p.attackTimer < 0.0f) p.isAttacking = false;

		    	float progress = p.attackTimer / p.attackDuration;
            		if(p.attackCounter % 2 == 0){
                	rightHandOrbit += sinf(progress * PI) * 15.0f;
                	rightHandAngleOffset += cosf(progress * PI) * 15.0f;
            		}
            	    
			else{
                	leftHandOrbit += sinf(progress * PI) * 15.0f;
                	leftHandAngleOffset += cosf(progress * PI) * 15.0f;
			}
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
        
    handRadius = p.isJumping ? 12.0f : 8.0f;

    for(auto& t : trees) {
    	t.hitOffset.x *= 0.85f;
    	t.hitOffset.y *= 0.85f;

    	if (fabsf(t.hitOffset.x) < 0.05f) t.hitOffset.x = 0.0f;
    	if (fabsf(t.hitOffset.y) < 0.05f) t.hitOffset.y = 0.0f;
    }

    for(auto& r : rocks) {
    	r.hitOffset.x *= 0.85f;
    	r.hitOffset.y *= 0.85f;

    	if (fabsf(r.hitOffset.x) < 0.05f) r.hitOffset.x = 0.0f;
    	if (fabsf(r.hitOffset.y) < 0.05f) r.hitOffset.y = 0.0f;
    }

    for(auto it = trees.begin(); it != trees.end(); ){
        if(it->health <= 0) it = trees.erase(it);
        else ++it;
    }

    for(auto it = rocks.begin(); it != rocks.end(); ){
        if(it->health <= 0) it = rocks.erase(it);
        else ++it;
    }

    for(auto it = drops.begin(); it != drops.end(); ){
        float dx = it->worldPos.x - mapOffset.x;
        float dy = it->worldPos.y - mapOffset.y;
        float dist = sqrtf(dx*dx + dy*dy);

        if(p.dropPickupTimer <= 0.0f && dist < (p.radius + it->radius + 5.0f)){
            if(it->type == 1){
		    if(p.activeWeapon == 1){
		    	    if(p.axeTier == 1) p.woodCount = p.woodCount + GetRandomValue(1, 3);
		    	    else if(p.axeTier == 2) p.woodCount = p.woodCount + GetRandomValue(2, 3);
		    }
		    else p.woodCount++;
	    }
            if(it->type == 2){
		    if(p.activeWeapon == 2){
		    	    if(p.pickaxeTier == 1) p.stoneCount = p.stoneCount + GetRandomValue(1, 2);
		            else if(p.pickaxeTier == 2) p.stoneCount = p.stoneCount + GetRandomValue(1, 3);
		    }
		    else p.stoneCount++;
	    }

            it = drops.erase(it);
        }
        else ++it;
    }
}
