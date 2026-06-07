#pragma once
#include <cmath>
#include <vector>
#include "common.h"
#include "structures.h"
#include "player.h"

inline void UpdateGame(Player& p, Vector2& mapOffset,
		const std::vector<Tree>& trees, const std::vector<Rock>& rocks,
		float deltaTime, Vector2& leftHandPos, Vector2& rightHandPos, float& handRadius){
	
	float oldOffsetX = mapOffset.x;
	if(IsKeyDown(KEY_A)) mapOffset.x -= p.speed;
	if(IsKeyDown(KEY_D)) mapOffset.x += p.speed;
	
	if(mapOffset.x < -MAP_HALF) mapOffset.x = -MAP_HALF;
	if(mapOffset.x > MAP_HALF) mapOffset.x = MAP_HALF;

	for(const auto& t : trees){
		float dx = t.worldPos.x - mapOffset.x;
		float dy = t.worldPos.y - mapOffset.y;
		if(sqrt(dx*dx + dy*dy) < p.radius + t.radius){
			mapOffset.x = oldOffsetX;
			break;
		}
	}
	for(const auto& r : rocks){
		float dx = r.worldPos.x - mapOffset.x;
		float dy = r.worldPos.y - mapOffset.y;
		if(sqrt(dx*dx + dy*dy) < p.radius + r.radius){
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
		p.radius = p.baseRadius + 15.0f;
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

	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !p.isAttacking && p.attackCooldownTimer <= 0.0f){
		p.isAttacking = true;
		p.attackTimer = p.attackDuration;
		p.attackCooldownTimer = p.attackCooldown;
		p.attackCounter++;
	}

	Vector2 mousePos = GetMousePosition();
	float angle = atan2(mousePos.y - p.screenCenter.y, mousePos.x - p.screenCenter.x);

	float leftHandOrbit = p.radius + 5.0f;
	float rightHandOrbit = p.radius + 5.0f;
	float leftHandAngleOffset = 0.7f;
	float rightHandAngleOffset = 0.7f;

	if(p.isAttacking){
		p.attackTimer -= deltaTime;
		if(p.attackTimer < 0.0f) p.isAttacking = false;

		if(p.attackCounter % 2 == 0){
			rightHandOrbit += 15.0f;
			rightHandAngleOffset = 0.2f;
		}
		else{
			leftHandOrbit += 15.0f;
			leftHandAngleOffset = 0.2f;
		}
	}

	leftHandPos = {
		p.screenCenter.x + leftHandOrbit * cosf(angle - leftHandAngleOffset),
		p.screenCenter.y + leftHandOrbit * sinf(angle - leftHandAngleOffset)
	};
	rightHandPos = {
		p.screenCenter.x + rightHandOrbit * cosf(angle + rightHandAngleOffset),
		p.screenCenter.y + rightHandOrbit * sinf(angle + rightHandAngleOffset)
	};
		
	handRadius = p.isJumping ? 12.0f : 8.0f;
}





