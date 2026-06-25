#include <cmath>
#include "common.h"
#include "player.h"
#include "world.h"

Player::Player(){
    screenCenter = { (float)SCREEN_W / 2.0f, (float)SCREEN_H / 2.0f };
    baseSpeed = 350.0f;
    speed = baseSpeed;
    baseRadius = 30.0f;
    radius = baseRadius;
    angle = 0.0f;

    health = 100;
    maxHealth = 100;
    stamina = 200.0f;
    maxStamina = 200.0f;
    hpRegenTimer = 5;

    isJumping = false;
    jumpTimer = 0.0f;
    jumpDuration = 0.4f;
    jumpCooldown = 0.5f;
    cooldownTimer = 0.0f;

    isAttacking = false;
    attackTimer = 0.0f;
    attackDuration = 0.3f;
    attackCounter = 0;
    attackCooldown = 0.48f;
    attackCooldownTimer = 0.0f;

    woodCount = 0;
    stoneCount = 0;
    axeTier = 0;
    pickaxeTier = 0;
    activeWeapon = 0;
    dropPickupTimer = 0.0f;
}

void Player::HandleMovement(float deltaTime){
    if(cooldownTimer > 0.0f) cooldownTimer -= deltaTime;

    if(isJumping){
	   jumpTimer -= deltaTime;
	   float progress = jumpTimer / jumpDuration; 
	   radius = baseRadius + sinf(progress * PI) * 8.0f;

        if(jumpTimer <= 0.0f){
		isJumping = false;
        	radius = baseRadius;
        	speed = baseSpeed;
		cooldownTimer = jumpCooldown;
        }
    }
}

void Player::Update(float deltaTime){
    Vector2 mousePos = GetMousePosition();
    angle = atan2f(mousePos.y - screenCenter.y, mousePos.x - screenCenter.x);

    if(attackCooldownTimer > 0.0f) attackCooldownTimer -= deltaTime;
    if(dropPickupTimer > 0.0f) dropPickupTimer -= deltaTime;

    if(isAttacking){
            attackTimer -= deltaTime;
            if(attackTimer < 0.0f) isAttacking = false;
    	    }

    if(stamina < maxStamina){
	    stamina += 20.0f * deltaTime;
	    if(stamina > maxStamina) stamina = maxStamina;
   	    }
    
    if(health < maxHealth){
	    hpRegenTimer += deltaTime;
	    if(hpRegenTimer >= 2.0f){
		    health += 1;
		    hpRegenTimer = 0.0f;
	    	    }
    	    }
    }

void Player::TriggerAttack(){
       if(!isAttacking && attackCooldownTimer <= 0.0f){
           isAttacking = true;
           attackTimer = attackDuration;
           attackCooldownTimer = attackCooldown;
           dropPickupTimer = 0.25f;


    	   if (activeWeapon > 0) attackCounter = 1; 
           else attackCounter = (attackCounter == 0) ? 1 : 0;
    	   }
       }

void Player::HandleInput(){
       if(IsKeyPressed(KEY_ONE)){
              if(axeTier == 0){
            	      if(woodCount >= 10){
                      	      woodCount -= 10;
                    	      axeTier = 1;
                	      activeWeapon = 1;
            		      }
            	      else activeWeapon = 0;
        	      }

              else if(axeTier == 1){
            	      if(woodCount >= 10 && stoneCount >= 15){
                	      woodCount -= 10;
                	      stoneCount -= 15;
                	      axeTier = 2;
                	      activeWeapon = 1;
		      	      }
		      else activeWeapon = 1;
		      }
              else if(axeTier > 1){
            activeWeapon = 1;
        }
    }

    if(IsKeyPressed(KEY_TWO)){
        if(pickaxeTier == 0){
            if(woodCount >= 10){
                woodCount -= 10;
                pickaxeTier = 1;
                activeWeapon = 2;
            }
            else {
                activeWeapon = 0;
            }
        }
        else if(pickaxeTier == 1){
            if(woodCount >= 10 && stoneCount >= 15){
                woodCount -= 10;
                stoneCount -= 15;
                pickaxeTier = 2;
                activeWeapon = 2;
            }
            else {
                activeWeapon = 2;
            }
        }
        else if(pickaxeTier > 1){
            activeWeapon = 2;
        }
    }

    if(IsKeyPressed(KEY_THREE)){
        activeWeapon = 0;
    }

    const float JUMP_STAMINA_COST = 50.0f;

    if (IsKeyDown(KEY_SPACE) && !isJumping && cooldownTimer <= 0.0f){
	if (stamina >= JUMP_STAMINA_COST) {
        	stamina -= JUMP_STAMINA_COST; 
        	isJumping = true;
        	jumpTimer = jumpDuration;
        	speed = baseSpeed + 200.0f;
		}
	else{
	}
    }
}

