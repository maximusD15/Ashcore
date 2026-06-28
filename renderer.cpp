#include "renderer.h"
#include "biomes.h"
#include "common.h"
#include <cmath>

void Renderer::DrawWorld(const World& world, const Player& player, const Vector2& mapOffset)
{
    Vector2 worldCenterOnScreen = {
        -mapOffset.x + player.GetScreenCenter().x,
        -mapOffset.y + player.GetScreenCenter().y
    };

    DrawCircleV(worldCenterOnScreen, 30000.0f, GetBiomeColor(BIOME_WINTER));
    DrawCircleV(worldCenterOnScreen, 20000.0f, GetBiomeColor(BIOME_DESERT));
    DrawCircleV(worldCenterOnScreen, 15000.0f, GetBiomeColor(BIOME_FOREST));
    DrawCircleV(worldCenterOnScreen, 7000.0f, GetBiomeColor(BIOME_WASTELAND));
    DrawCircleV(worldCenterOnScreen, 4500.0f, GetBiomeColor(BIOME_LAVA_RING));
    DrawCircleV(worldCenterOnScreen, 3000.0f, GetBiomeColor(BIOME_BOSS_LAIR));

    for (const auto& blade : world.grassTiles) {
        float screenX = blade.worldPos.x - mapOffset.x + player.GetScreenCenter().x;
        float screenY = blade.worldPos.y - mapOffset.y + player.GetScreenCenter().y;
        if (screenX >= -50 && screenX <= SCREEN_W + 50 && screenY >= -50 && screenY <= SCREEN_H + 50) {
            DrawRectangle(screenX, screenY, blade.size, blade.size, GetColor(0x38532BFF));
        }
    }
}

void Renderer::DrawStructures(const World& world, const Player& player, const Vector2& mapOffset)
{
    for (const auto& r : world.rocks) {
        float screenX = r.worldPos.x - mapOffset.x + player.GetScreenCenter().x + r.hitOffset.x;
        float screenY = r.worldPos.y - mapOffset.y + player.GetScreenCenter().y + r.hitOffset.y;
        if (screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100) {
            DrawCircleV({ screenX, screenY }, r.radius + 3, BLACK);
            DrawCircleV({ screenX, screenY }, r.radius, GetColor(0x7A7A7AFF));
        }
    }
    
    for(const auto& g : world.golds){
	    float screenX = g.worldPos.x - mapOffset.x + player.GetScreenCenter().x + g.hitOffset.x;
	    float screenY = g.worldPos.y - mapOffset.y + player.GetScreenCenter().y + g.hitOffset.y;
	    if(screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100){
		    DrawCircleV({ screenX, screenY}, g.radius + 3, BLACK);
		    DrawCircleV({ screenX, screenY}, g.radius, GetColor(0xFFD700FF));
	    }
    }

    for (const auto& t : world.trees) {
        float screenX = t.worldPos.x - mapOffset.x + player.GetScreenCenter().x + t.hitOffset.x;
        float screenY = t.worldPos.y - mapOffset.y + player.GetScreenCenter().y + t.hitOffset.y;
        if (screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100) {
            DrawCircleV({ screenX, screenY }, t.radius + 3, BLACK);
            DrawCircleV({ screenX, screenY }, t.radius, GetColor(0x2E5A1CFF));
        }
    }

    for (const auto& d : world.drops) {
        float screenX = d.worldPos.x - mapOffset.x + player.GetScreenCenter().x;
        float screenY = d.worldPos.y - mapOffset.y + player.GetScreenCenter().y;
        if (screenX >= -50 && screenX <= SCREEN_W + 50 && screenY >= -50 && screenY <= SCREEN_H + 50) {
            Color dropColor;
	    if(d.type == 1) dropColor = BROWN;
	    if(d.type == 2) dropColor = GRAY;
	    if(d.type == 3) dropColor = GOLD;
            DrawCircleV({ screenX, screenY }, d.radius, dropColor);
            DrawCircleV({ screenX, screenY }, d.radius, ColorAlpha(BLACK, 0.3f));
        }
    }
}

void Renderer::DrawPlayer(const Player& player, const Vector2& leftHandPos, const Vector2& rightHandPos, float handRadius)
{
    if (player.GetActiveWeapon() > 0) {
        Vector2 weaponPos = rightHandPos;
        float currentWeaponAngle = atan2f(weaponPos.y - player.GetScreenCenter().y, weaponPos.x - player.GetScreenCenter().x);
        float weaponAngleDegrees = (currentWeaponAngle * (180.0f / PI)) + 50.0f;

        if (player.GetActiveWeapon() == 1) {
            DrawRectanglePro(Rectangle { weaponPos.x, weaponPos.y, 70.0f, 6.0f }, Vector2 { 50.0f, 3.0f }, weaponAngleDegrees, BROWN);
	    Color axeColor;
	    if(player.GetAxeTier() == 1) axeColor = BROWN;
	    if(player.GetAxeTier() == 2) axeColor = DARKGRAY;
	    if(player.GetAxeTier() == 3) axeColor = YELLOW;
            DrawRectanglePro(Rectangle { weaponPos.x, weaponPos.y, 18.0f, 14.0f }, Vector2 { -20.0f, 10.0f }, weaponAngleDegrees, axeColor);
        } else if (player.GetActiveWeapon() == 2) {
            DrawRectanglePro(Rectangle { weaponPos.x, weaponPos.y, 70.0f, 6.0f }, Vector2 { 50.0f, 3.0f }, weaponAngleDegrees, BROWN);
	    Color pickColor;
	    if(player.GetPickaxeTier() == 1) pickColor = BROWN;
	    if(player.GetPickaxeTier() == 2) pickColor = DARKGRAY;
	    if(player.GetPickaxeTier() == 3) pickColor = YELLOW;
            DrawRectanglePro(Rectangle { weaponPos.x, weaponPos.y, 6.0f, 30.0f }, Vector2 { -20.0f, 15.0f }, weaponAngleDegrees, pickColor);
        }
    }

    DrawCircleV(leftHandPos, handRadius + 2, BLACK);
    DrawCircleV(rightHandPos, handRadius + 2, BLACK);
    DrawCircleV(leftHandPos, handRadius, GetColor(0xDF9B7FFF));
    DrawCircleV(rightHandPos, handRadius, GetColor(0xDF9B7FFF));

    DrawCircleV(player.GetScreenCenter(), player.GetRadius() + 3, BLACK);
    DrawCircleV(player.GetScreenCenter(), player.GetRadius(), GetColor(0xDF9B7FFF));
}

void Renderer::DrawUI(const Player& player)
{
    DrawText(TextFormat("WOOD %d", player.GetWoodCount()), SCREEN_W - 1900, SCREEN_H - 50, 30, WHITE);
    DrawText(TextFormat("STONE %d", player.GetStoneCount()), SCREEN_W - 1900, SCREEN_H - 100, 30, WHITE);
    DrawText(TextFormat("GOLD %d", player.GetGoldCount()), SCREEN_W - 1900, SCREEN_H - 150, 30, WHITE);

    DrawText(TextFormat("WEAPON: %s (TIER %d)",
                 player.GetActiveWeapon() == 0 ? "Fists" : (player.GetActiveWeapon() == 1 ? "Axe" : "Pickaxe"),
                 player.GetActiveWeapon() == 1 ? player.GetAxeTier() : (player.GetActiveWeapon() == 2 ? player.GetPickaxeTier() : 0)),
        SCREEN_W - 1080, SCREEN_H - 50, 20, WHITE);

    float barWidth = 100.0f;
    float barHeight = 7.5f;
    float screenX = player.GetScreenCenter().x - (barWidth / 2.0f);
    float screenY = player.GetScreenCenter().y + player.GetRadius() + 15.0f;

    DrawRectangle(screenX - 2, screenY - 2, barWidth + 4, barHeight + 4, BLACK);
    float hpPercent = (float)player.GetHealth() / (float)player.GetMaxHealth();
    if (hpPercent < 0.0f)
        hpPercent = 0.0f;
    DrawRectangle(screenX, screenY, barWidth * hpPercent, barHeight, GREEN);

    float staminaY = screenY + barHeight + 5.0f;
    DrawRectangle(screenX - 2, staminaY - 2, barWidth + 4, barHeight + 4, BLACK);
    float staminaPercent = (float)player.GetStamina() / (float)player.GetMaxStamina();
    if (staminaPercent < 0.0f)
        staminaPercent = 0.0f;
    DrawRectangle(screenX, staminaY, barWidth * staminaPercent, barHeight, ORANGE);
}
