#pragma once
#include <raylib.h>

struct Player{
	Vector2 screenCenter;
	float baseSpeed;
	float speed;
	float baseRadius;
	float radius;
	float angle;

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

	int axeTier;
	int pickaxeTier;

	int activeWeapon;
};

inline Player CreatePlayer(){
	Player p;
	p.screenCenter = { (float)SCREEN_W / 2.0f, (float)SCREEN_H / 2.0f };
	p.baseSpeed = 5.0f;
	p.speed = p.baseSpeed;
    	p.baseRadius = 30.0f;
    	p.radius = p.baseRadius;
	p.angle = 0.0f;

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

	p.woodCount = 0;
	p.stoneCount = 0;
	p.axeTier = 0;
	p.pickaxeTier = 0;
	p.activeWeapon = 0;

	return p;
}

