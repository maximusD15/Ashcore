#pragma once
#include <vector>
#include <cmath>
#include "structures.h"
#include "player.h"

inline void HandleCollisions(Player& p, Vector2& mapOffset, float oldOffsetX, float oldOffsetY,
		const std::vector<Tree>& trees, const std::vector<Rock>& rocks){
	// ----------------------- COLLISIONS X ---------------------- //
	for(const auto& t : trees){
		float dx = t.worldPos.x - mapOffset.x;
		float dy = t.worldPos.y - mapOffset.y;
		if(sqrtf(dx*dx + dy*dy) < p.radius + t.radius - 10.0f){
			mapOffset.x = oldOffsetX;
			break;
		}
	}
	for(const auto& r : rocks){
		float dx = r.worldPos.x - mapOffset.x;
		float dy = r.worldPos.y - mapOffset.y;
		if(sqrtf(dx*dx + dy*dy) < p.radius + r.radius){
			mapOffset.x = oldOffsetX;
			break;
		}
	}
	// ---------------------- COLLISIONS Y -----------------------//
	for(const auto& t : trees){
		float dx = t.worldPos.x - mapOffset.x;
		float dy = t.worldPos.y - mapOffset.y;
		if(sqrtf(dx*dx + dy*dy) < p.radius + t.radius - 10.0f){
			mapOffset.y = oldOffsetY;
			break;
		}
	}
	for(const auto& r : rocks){
		float dx = r.worldPos.x - mapOffset.x;
		float dy = r.worldPos.y - mapOffset.y;
		if(sqrtf(dx*dx + dy*dy) < p.radius + r.radius){
			mapOffset.y = oldOffsetY;
			break;
		}
	}
}

