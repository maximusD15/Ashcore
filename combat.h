#pragma once
#include <vector>
#include <cmath>
#include "structures.h"
#include "player.h"

inline void HandleMeleeAnimation(Player& p, float deltaTime, float strikeFactor, 
		float& leftHandOrbit, float& rightHandOrbit,
		float& leftHandAngleOffset, float& rightHandAngleOffset){
	
	if(p.isAttacking){
		if(p.attackCounter == 1){
			rightHandOrbit += strikeFactor * 25.0f;
			rightHandAngleOffset -= strikeFactor * 0.6;
		}
		else{
			leftHandOrbit += strikeFactor * 25.0f;
			leftHandAngleOffset -= strikeFactor * 0.6;
		}
	}
}

inline void UpdateVisualEffects(std::vector<Tree>& trees, std::vector<Rock>& rocks){
	for(auto& t : trees){
		t.hitOffset.x *= 0.85;
		t.hitOffset.y *= 0.85;
		if(fabsf(t.hitOffset.x) < 0.05f) t.hitOffset.x = 0.0f;
		if(fabs(t.hitOffset.y) < 0.05f) t.hitOffset.y = 0.0f;
	}
	for(auto& r : rocks){
		r.hitOffset.x *= 0.85;
		r.hitOffset.y *- 0.85;
		if(fabsf(r.hitOffset.x) < 0.05) r.hitOffset.x = 0.0f;
		if(fabsf(r.hitOffset.y) < 0.05) r.hitOffset.y = 0.0f;
	}
}

inline void HandleAttackHitbox(Player& p, const Vector2& mapOffset,
		std::vector<Tree>& trees, std::vector<Rock>& rocks, 
		std::vector<ResourceDrop>& drops){

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
		
		if(dist < trees[i].radius + 10.0f){
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

			drops.push_back(drop);
			return;
		}
	}

	int mineDamage = 0;
	if(p.activeWeapon == 2) mineDamage = p.pickaxeTier * GetRandomValue(1, 3);

	if(mineDamage > 0){
		for(size_t i = 0; i < rocks.size(); i++){
			float dx = rocks[i].worldPos.x - hitWorldPos.x;
			float dy = rocks[i].worldPos.y - hitWorldPos.y;
			float dist = sqrtf(dx*dx + dy*dy);
			
			if(dist < rocks[i].radius + 10.0f){
				rocks[i].health -= mineDamage;
				
				float screenRockX = rocks[i].worldPos.x - mapOffset.x + p.screenCenter.x;
				float screenRockY = rocks[i].worldPos.y - mapOffset.y + p.screenCenter.y;
				float pushAngle = atan2f(screenRockY - p.screenCenter.y, screenRockX - p.screenCenter.x);

				rocks[i].hitOffset.x = cosf(pushAngle) * 12.0f;
				rocks[i].hitOffset.y = sinf(pushAngle) * 12.0f;

				ResourceDrop drop;
				drop.type = 2;
				drop.radius = 15.0f;

				float RandomSpread = (GetRandomValue(-45, 45) * PI) / 180.0f;
				float dropAngle = pushAngle + PI + RandomSpread;

				drop.worldPos.x = rocks[i].worldPos.x - cosf(pushAngle) * rocks[i].radius + cosf(dropAngle) * 35.0f;
				drop.worldPos.y = rocks[i].worldPos.y - sinf(pushAngle) * rocks[i].radius + sinf(dropAngle) * 35.0f;

				drops.push_back(drop);
				return;
			}
		}
	}
}





