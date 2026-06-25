#include "biomes.h"
#include "common.h"
#include "player.h"
#include "renderer.h"
#include "world.h"
#include <raylib.h>

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "Ashcore");

    Player player;
    World world;
    Renderer renderer;

    world.Init();

    Vector2 mapOffset = { 0.0f, 0.0f };
    float spawnAngle = (float)GetRandomValue(0, 360) * (PI / 180.0f);
    float spawnRadius = (float)GetRandomValue(8000, 16000);
    mapOffset.x = cosf(spawnAngle) * spawnRadius;
    mapOffset.y = sinf(spawnAngle) * spawnRadius;

    Vector2 leftHandPos = { 0 }, rightHandPos = { 0 };
    float handRadius; // = 8.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        world.Update(player, mapOffset, deltaTime, leftHandPos, rightHandPos, handRadius);

        BeginDrawing();
        ClearBackground(BLACK);

        renderer.DrawWorld(world, player, mapOffset);
        renderer.DrawPlayer(player, leftHandPos, rightHandPos, handRadius);
        renderer.DrawUI(player);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
