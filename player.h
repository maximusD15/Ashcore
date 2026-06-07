#pragma once
#include <raylib.h>

struct Player{
	Vector2 screenCenter;
	float baseSpeed;
	float speed;
	float baseRadius;
	float radius;

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
};

inline Player CreatePlayer(){
	Player p;
	p.screenCenter = { (float)SCREEN_W / 2.0f, (float)SCREEN_H / 2.0f };
	p.baseSpeed = 5.0f;
	p.speed = p.baseSpeed;
    	p.baseRadius = 30.0f;
    	p.radius = p.baseRadius;

    	p.isJumping = false;
   	p.jumpTimer = 0.0f;
    	p.jumpDuration = 0.3f;
   	p.jumpCooldown = 0.5f;
    	p.cooldownTimer = 0.0f;

    	p.isAttacking = false;
    	p.attackTimer = 0.0f;
    	p.attackDuration = 0.3f;
    	p.attackCounter = 0;
    	p.attackCooldown = 0.55f;
    	p.attackCooldownTimer = 0.0f;

	return p;
}

