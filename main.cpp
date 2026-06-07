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
	for(int i = 0; i < 8000; i++){
		GrassBlade blade;
		blade.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		blade.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		blade.size = (float)GetRandomValue(6, 10);
		grassTiles.push_back(blade);
	}
	std::vector<Tree> trees;
	for(int i = 0; i < 60; i++){
		Tree t;
		t.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		t.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		t.radius = (float)GetRandomValue(30, 80);
		trees.push_back(t);
	}

	std::vector<Rock> rocks;
	for(int i = 0; i < 40; i++){
		Rock r;
		r.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		r.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
		r.radius = (float)GetRandomValue(30, 80);
		rocks.push_back(r);
	}

	Vector2 leftHandPos = { 0 }, rightHandPos = { 0 };
	float handRadius = 8.0f;

	SetTargetFPS(60);

	while(!WindowShouldClose()){
		float deltaTime = GetFrameTime();

		UpdateGame(player, mapOffset, trees, rocks, deltaTime, leftHandPos, rightHandPos, handRadius);

		BeginDrawing();
		ClearBackground(BLACK);

		float greenX = -MAP_HALF - mapOffset.x + player.screenCenter.x;
		float greenY = -MAP_HALF - mapOffset.y + player.screenCenter.y;
		DrawRectangle(greenX, greenY, MAP_SIZE, MAP_SIZE, GetColor(0x557A46FF));

		for(const auto& blade : grassTiles){
			float screenX = blade.worldPos.x - mapOffset.x + player.screenCenter.x;
			float screenY = blade.worldPos.y - mapOffset.y + player.screenCenter.y;
			if(screenX >= -50 && screenX <= SCREEN_W + 50 && screenY >= -50 && screenY <= SCREEN_H + 50){
				DrawRectangle(screenX, screenY, blade.size, blade.size, GetColor(0x38532BFF));
			}
		}

		for(const auto& r : rocks){
			float screenX = r.worldPos.x - mapOffset.x + player.screenCenter.x;
			float screenY = r.worldPos.y - mapOffset.y + player.screenCenter.y;
			if(screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100){
				DrawCircleV({screenX, screenY}, r.radius + 3, BLACK);
				DrawCircleV({screenX, screenY}, r.radius, GetColor(0x7A7A7AFF));
			}
		}
		
		for(const auto& t : trees){
			float screenX = t.worldPos.x - mapOffset.x + player.screenCenter.x;
			float screenY = t.worldPos.y - mapOffset.y + player.screenCenter.y;
			if(screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100){
				DrawCircleV({screenX, screenY}, t.radius + 3, BLACK);
				DrawCircleV({screenX, screenY}, t.radius, GetColor(0x2E5A1CFF));
			}
		}

		DrawCircleV(leftHandPos, handRadius + 2, BLACK);
		DrawCircleV(leftHandPos, handRadius, GetColor(0xDF9B7FFF));
		DrawCircleV(rightHandPos, handRadius + 2, BLACK);
		DrawCircleV(rightHandPos, handRadius, GetColor(0xDF9B7FFF));

		DrawCircleV(player.screenCenter, player.radius + 3, BLACK);
		DrawCircleV(player.screenCenter, player.radius, GetColor(0xDF9B7FFF));

		EndDrawing();
	}

	CloseWindow();

	return 0;
}



		

