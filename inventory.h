#pragma once
#include "player.h"
#include "raylib.h"

inline void HandleWeaponSelectionAndCraft(Player& p){
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

	if (IsKeyPressed(KEY_TWO)){
        	if (p.pickaxeTier == 0) {
            		if (p.woodCount >= 10) {
                		p.woodCount -= 10;
               			p.pickaxeTier = 1;
                		p.activeWeapon = 2;
            			}
       			}
        	else if (p.pickaxeTier == 1 && p.woodCount >= 15 && p.stoneCount >= 15){
            		p.woodCount -= 15;
            		p.stoneCount -= 15;
            		p.pickaxeTier = 2;
            		p.activeWeapon = 2;
        	}
       		else if (p.pickaxeTier > 0) {
            		p.activeWeapon = 2;
        		}
    		}
    if (IsKeyPressed(KEY_THREE)) p.activeWeapon = 0;
}

inline void UpdateWorldEntities(Player& p, const Vector2& mapOffset, 
                                std::vector<Tree>& trees, std::vector<Rock>& rocks, 
                                std::vector<ResourceDrop>& drops) {
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
                    if(p.axeTier == 1) p.woodCount += GetRandomValue(1, 3);
                    else if(p.axeTier == 2) p.woodCount += GetRandomValue(2, 3);
                }
                else p.woodCount++;
            }
            if(it->type == 2){
                if(p.activeWeapon == 2){
                    if(p.pickaxeTier == 1) p.stoneCount += GetRandomValue(1, 2);
                    else if(p.pickaxeTier == 2) p.stoneCount += GetRandomValue(1, 3);
                }
                else p.stoneCount++;
            }
            it = drops.erase(it);
        }
        else ++it;
    }
}
