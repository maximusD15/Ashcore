#include <raylib.h>
#include <vector>
#include "common.h"
#include "structures.h"
#include "player.h"
#include "logic.h"

int main(){
	InitWindow(SCREEN_W, SCREEN_H, "Doomed2.io remake");

	Player player = CreatePlayer();
	Vector2 mapOffset = { 0.0f, 0.0f };

	std::vector<GrassBlade> grassTiles;
	for(int i = 0; i < 1000; i++){
		GrassBlade blade;
		blade.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		blade.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		blade.size = (float)GetRandomValue(4, 6);
		grassTiles.push_back(blade);
	}
	std::vector<Tree> trees;
	for(int i = 0; i < 60; i++){
		Tree t;
		t.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		t.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		t.radius = (float)GetRandomValue(50, 100);
		if(t.radius < 65) t.maxHealth = 20;
		if(t.radius < 85) t.maxHealth = 40;
		else t.maxHealth = 60;

		t.health = t.maxHealth;

		trees.push_back(t);
	}

	std::vector<Rock> rocks;
	for(int i = 0; i < 40; i++){
		Rock r;
		r.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		r.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		r.radius = (float)GetRandomValue(50, 100);
		if(r.radius < 65) r.maxHealth = 20;
		if(r.radius < 85) r.maxHealth = 40;
		else r.maxHealth = 60;

		r.health = r.maxHealth;

		rocks.push_back(r);
	}

	Vector2 leftHandPos = { 0 }, rightHandPos = { 0 };
	float handRadius = 8.0f;
	
	std::vector<ResourceDrop> drops;

	SetTargetFPS(60);

	while(!WindowShouldClose()){
		float deltaTime = GetFrameTime();

		UpdateGame(player, mapOffset, trees, rocks, drops, deltaTime, leftHandPos, rightHandPos, handRadius);

		BeginDrawing();
		ClearBackground(BLACK);

		float greenX = -MAP_HALF - mapOffset.x + player.screenCenter.x;
		float greenY = -MAP_HALF - mapOffset.y + player.screenCenter.y;
		DrawRectangle(greenX, greenY, MAP_SIZE, MAP_SIZE, GetColor(0x1B4600E0));

		for(const auto& blade : grassTiles){
			float screenX = blade.worldPos.x - mapOffset.x + player.screenCenter.x;
			float screenY = blade.worldPos.y - mapOffset.y + player.screenCenter.y;
			if(screenX >= -50 && screenX <= SCREEN_W + 50 && screenY >= -50 && screenY <= SCREEN_H + 50){
				DrawRectangle(screenX, screenY, blade.size, blade.size, GetColor(0x38532BFF));
			}
		}

		for(const auto& r : rocks){
			float screenX = r.worldPos.x - mapOffset.x + player.screenCenter.x + r.hitOffset.x;
   			float screenY = r.worldPos.y - mapOffset.y + player.screenCenter.y + r.hitOffset.y;
    			if(screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100){
        			DrawCircleV({screenX, screenY}, r.radius + 3, BLACK);
        			DrawCircleV({screenX, screenY}, r.radius, GetColor(0x7A7A7AFF));
			}
		}
		
		for(const auto& t : trees){
			float screenX = t.worldPos.x - mapOffset.x + player.screenCenter.x + t.hitOffset.x;
    			float screenY = t.worldPos.y - mapOffset.y + player.screenCenter.y + t.hitOffset.y;
    			if(screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100){
        			DrawCircleV({screenX, screenY}, t.radius + 3, BLACK);
        			DrawCircleV({screenX, screenY}, t.radius, GetColor(0x2E5A1CFF));
			}
		}

		for(const auto& drop : drops){
			float screenX = drop.worldPos.x - mapOffset.x + player.screenCenter.x;
			float screenY = drop.worldPos.y - mapOffset.y + player.screenCenter.y;
			if(screenX >= -50 && screenX <= SCREEN_W + 50 && screenY >= -50 && screenY <= SCREEN_H + 50){
				Color dropColor = (drop.type == 1) ? GetColor(0x8B5A2BFF) : GetColor(0x9E9E9EFF);
				DrawCircleV({screenX, screenY}, drop.radius, dropColor);
				DrawCircleV({screenX, screenY}, drop.radius, ColorAlpha(BLACK, 0.3f));
			}
		}

		DrawCircleV(leftHandPos, handRadius + 2, BLACK);
		DrawCircleV(leftHandPos, handRadius, GetColor(0xDF9B7FFF));
		DrawCircleV(rightHandPos, handRadius + 2, BLACK);
		DrawCircleV(rightHandPos, handRadius, GetColor(0xDF9B7FFF));

		Vector2 weaponPos = rightHandPos;

		float weaponAngleDegrees = (player.angle * (180.0f / PI)) - 90.0f;

		if(player.activeWeapon == 1){
			DrawRectanglePro(
				Rectangle{ weaponPos.x, weaponPos.y, 35.0f, 6.0f },
				Vector2{ 0.0f, 3.0f},
				weaponAngleDegrees,
				BROWN
			);
				
			Color axeColor = (player.axeTier == 2) ? DARKGRAY : BROWN;
			DrawRectanglePro(
        			Rectangle{ weaponPos.x, weaponPos.y, 16.0f, 10.0f },
        			Vector2{ -35.0f, -10.0f },
        			weaponAngleDegrees, 
        			axeColor
    			);
		}	
		else if (player.activeWeapon == 2) {
    			DrawRectanglePro(
        			Rectangle{ weaponPos.x, weaponPos.y, 35.0f, 6.0f },
       				Vector2{ 0.0f, 3.0f }, 
       				weaponAngleDegrees, 
        			BROWN
    			);
    
    				Color pickColor = (player.pickaxeTier == 2) ? DARKGRAY : BROWN; 
    			DrawRectanglePro(
        			Rectangle{ weaponPos.x, weaponPos.y, 6.0f, 28.0f },
        			Vector2{ -35.0f, -14.0f },
        			weaponAngleDegrees, 
        			pickColor
    			);
		}


		DrawCircleV(player.screenCenter, player.radius + 3, BLACK);
		DrawCircleV(player.screenCenter, player.radius, GetColor(0xDF9B7FFF));

        	DrawText(TextFormat("WOOD: %d", player.woodCount), 20, 20, 30, WHITE);
        	DrawText(TextFormat("STONE: %d", player.stoneCount), 20, 60, 30, WHITE);
        
        	DrawText(TextFormat("WEAPON: %s (Tier %d)", 
            	player.activeWeapon == 0 ? "Fists" : (player.activeWeapon == 1 ? "Axe" : "Pickaxe"),
            	player.activeWeapon == 1 ? player.axeTier : (player.activeWeapon == 2 ? player.pickaxeTier : 0)), 
            	20, 100, 20, GRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}



		

