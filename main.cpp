#include "biomes.h"
#include "common.h"
#include "player.h"
#include "renderer.h"
#include "world.h"
#include <raylib.h>

enum GameState {
    STATE_MENU,
    STATE_GAME,
    STATE_DEATH
};

int main()
{

    InitWindow(0, 0, "ASHCORE");

    Player player;
    World world;
    Renderer renderer;

    world.Init();

    GameState currentState = STATE_MENU;

    Vector2 mapOffset = { 0.0f, 0.0f };
    float spawnAngle = (float)GetRandomValue(0, 360) * (PI / 180.0f);
    float spawnRadius = (float)GetRandomValue(8000, 16000);
    mapOffset.x = cosf(spawnAngle) * spawnRadius;
    mapOffset.y = sinf(spawnAngle) * spawnRadius;

    Vector2 leftHandPos = { 0 }, rightHandPos = { 0 };
    float handRadius;

    SetTargetFPS(120);

    float deathTimer = 0.0f;
    const float DEATH_SCREEN_DURATION = 3.0f;

    float btnW = 220.0f;
    float btnH = 60.0f;
    Button playButton = {
        { 0.0f, 0.0f, btnW, btnH },
        "PLAY",
        Fade(ORANGE, 0.7f),
        Fade(GRAY, 0.85f),
        false
    };

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }
        float deltaTime = GetFrameTime();

        switch (currentState) {
        case STATE_MENU:
            playButton.rect.x = GetScreenWidth() / 2.0f - playButton.rect.width / 2.0f;
            playButton.rect.y = GetScreenHeight() / 2.0f + 20.0f;

            playButton.Update();

            if (playButton.isClicked() || IsKeyPressed(KEY_SPACE)) {
                world.Init();

                float spawnAngle = (float)GetRandomValue(0, 360) * (PI / 180.0f);
                float spawnRadius = (float)GetRandomValue(8000, 16000);
                mapOffset.x = cosf(spawnAngle) * spawnRadius;
                mapOffset.y = sinf(spawnAngle) * spawnRadius;
                player.ResetPlayerStates();

                currentState = STATE_GAME;
            }
            break;

        case STATE_GAME:
            world.Update(player, mapOffset, deltaTime, leftHandPos, rightHandPos, handRadius);
            if (player.GetHealth() <= 0) {
                deathTimer = DEATH_SCREEN_DURATION;
                currentState = STATE_DEATH;
            }
            break;

        case STATE_DEATH:
            deathTimer -= deltaTime;
            if (deathTimer <= 0.0f) {
                currentState = STATE_MENU;
            }
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
        case STATE_MENU:
            renderer.DrawMainMenu(playButton);
            break;

        case STATE_GAME:

            renderer.DrawWorld(world, player, mapOffset);
            renderer.DrawMobs(world.mobs, player, mapOffset);
            renderer.DrawPlayer(player, leftHandPos, rightHandPos, handRadius);
            renderer.DrawStructures(world, player, mapOffset);
            renderer.DrawUI(player);
            renderer.DrawInventoryUI(player);
            break;
        case STATE_DEATH:

            renderer.DrawDeathScreen(world, player, mapOffset, leftHandPos, rightHandPos, handRadius, deathTimer);
            break;
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
