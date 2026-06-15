#pragma once
#include <raylib.h>
#include "common.h"
#include "player.h"

inline void HandlePlayerMovement(Player& p, Vector2& mapOffset, float deltaTime){

	if(IsKeyDown(KEY_A)) mapOffset.x -= p.speed;
	if(IsKeyDown(KEY_D)) mapOffset.x += p.speed;
	if(IsKeyDown(KEY_W)) mapOffset.y -= p.speed;
	if(IsKeyDown(KEY_S)) mapOffset.y += p.speed;

	if(mapOffset.x < -MAP_HALF) mapOffset.x = -MAP_HALF;
	if(mapOffset.x > MAP_HALF) mapOffset.x = MAP_HALF;
	if(mapOffset.y < -MAP_HALF) mapOffset.y = -MAP_HALF;
	if(mapOffset.y > MAP_HALF) mapOffset.y = MAP_HALF;

	if(IsKeyDown(KEY_SPACE) && !p.isJumping && p.cooldownTimer <= 0.0f){
		p.isJumping = true;
		p.jumpTimer = p.jumpDuration;
		p.radius = p.baseRadius + 5.0f;
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
	else if(p.cooldownTimer > 0.0f){
		p.cooldownTimer -= deltaTime;
	}
}


