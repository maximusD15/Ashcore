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

    for (const auto& g : world.golds) {
        float screenX = g.worldPos.x - mapOffset.x + player.GetScreenCenter().x + g.hitOffset.x;
        float screenY = g.worldPos.y - mapOffset.y + player.GetScreenCenter().y + g.hitOffset.y;
        if (screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100) {
            DrawCircleV({ screenX, screenY }, g.radius + 3, BLACK);
            DrawCircleV({ screenX, screenY }, g.radius, GetColor(0xFFD700FF));
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
            if (d.type == 1)
                dropColor = BROWN;
            if (d.type == 2)
                dropColor = GRAY;
            if (d.type == 3)
                dropColor = GOLD;
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
            if (player.GetAxeTier() == 1)
                axeColor = BROWN;
            if (player.GetAxeTier() == 2)
                axeColor = DARKGRAY;
            if (player.GetAxeTier() == 3)
                axeColor = YELLOW;
            DrawRectanglePro(Rectangle { weaponPos.x, weaponPos.y, 18.0f, 14.0f }, Vector2 { -20.0f, 10.0f }, weaponAngleDegrees, axeColor);
        } else if (player.GetActiveWeapon() == 2) {
            DrawRectanglePro(Rectangle { weaponPos.x, weaponPos.y, 70.0f, 6.0f }, Vector2 { 50.0f, 3.0f }, weaponAngleDegrees, BROWN);
            Color pickColor;
            if (player.GetPickaxeTier() == 1)
                pickColor = BROWN;
            if (player.GetPickaxeTier() == 2)
                pickColor = DARKGRAY;
            if (player.GetPickaxeTier() == 3)
                pickColor = YELLOW;
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
    /* DrawText(TextFormat("WOOD %d", player.GetWoodCount()), SCREEN_W - 1900, SCREEN_H - 50, 30, WHITE);
     DrawText(TextFormat("STONE %d", player.GetStoneCount()), SCREEN_W - 1900, SCREEN_H - 100, 30, WHITE);
     DrawText(TextFormat("GOLD %d", player.GetGoldCount()), SCREEN_W - 1900, SCREEN_H - 150, 30, WHITE);

     DrawText(TextFormat("WEAPON: %s (TIER %d)",
                  player.GetActiveWeapon() == 0 ? "Fists" : (player.GetActiveWeapon() == 1 ? "Axe" : "Pickaxe"),
                  player.GetActiveWeapon() == 1 ? player.GetAxeTier() : (player.GetActiveWeapon() == 2 ? player.GetPickaxeTier() : 0)),
         SCREEN_W - 1080, SCREEN_H - 50, 20, WHITE);
         */

    float barWidth = 100.0f;
    float barHeight = 7.5f;
    float screenX = player.GetScreenCenter().x - (barWidth / 2.0f);
    float screenY = player.GetScreenCenter().y + player.GetRadius() + 15.0f;

    Rectangle hpBg = { screenX - 2, screenY - 2, barWidth + 4, barHeight + 4 };

    DrawRectangleRounded(hpBg, 20.0f, 100, BLACK);

    float hpPercent = (float)player.GetHealth() / (float)player.GetMaxHealth();
    Rectangle hpBar = { screenX, screenY, barWidth * hpPercent, barHeight };

    if (hpPercent < 0.0f)
        hpPercent = 0.0f;
    DrawRectangleRounded(hpBar, 20.0f, 100, GREEN);

    float staminaY = screenY + barHeight + 5.0f;

    Rectangle staminaBg = { screenX - 2, staminaY - 2, barWidth + 4, barHeight + 4 };

    DrawRectangleRounded(staminaBg, 20.0f, 100, BLACK);
    float staminaPercent = (float)player.GetStamina() / (float)player.GetMaxStamina();
    Rectangle staminaBar = { screenX, staminaY, barWidth * staminaPercent, barHeight };
    if (staminaPercent < 0.0f)
        staminaPercent = 0.0f;
    DrawRectangleRounded(staminaBar, 20.0f, 100, ORANGE);
}

void Renderer::DrawMobs(const std::vector<Mob>& mobs, const Player& player, const Vector2& mapOffset)
{
    Vector2 screenCenter = player.GetScreenCenter();

    for (const auto& mob : mobs) {
        float screenX = mob.worldPos.x - mapOffset.x + screenCenter.x;
        float screenY = mob.worldPos.y - mapOffset.y + screenCenter.y;

        if (screenX >= -100 && screenX <= SCREEN_W + 100 && screenY >= -100 && screenY <= SCREEN_H + 100) {
            Vector2 mobScreenPos = { screenX, screenY };

            if (mob.type == 1) {
                Color pigColor = GetColor(0xFFB6C1FF);
                Color pigNose = GetColor(0xFF69B4FF);
                Color pigEye = BLACK;

                DrawCircleV(mobScreenPos, mob.radius + 2, BLACK);
                DrawCircleV(mobScreenPos, mob.radius, pigColor);

                Vector2 headPos = {
                    mobScreenPos.x + cosf(mob.angle) * (mob.radius * 0.5f),
                    mobScreenPos.y + sinf(mob.angle) * (mob.radius * 0.5f)
                };
                DrawCircleV(headPos, mob.radius * 0.6f + 2, BLACK);
                DrawCircleV(headPos, mob.radius * 0.6f, pigColor);

                Vector2 nosePos = {
                    headPos.x + cosf(mob.angle) * (mob.radius * 0.2f),
                    headPos.y + sinf(mob.angle) * (mob.radius * 0.2f)
                };
                DrawCircleV(nosePos, mob.radius * 0.2f, pigNose);

                float sideAngleOffset = 0.5f;

                Vector2 leftEyePos = {
                    mobScreenPos.x + cosf(mob.angle + sideAngleOffset) * (mob.radius * 0.5f),
                    mobScreenPos.y + sinf(mob.angle + sideAngleOffset) * (mob.radius * 0.5f)
                };

                Vector2 rightEyePos = {
                    mobScreenPos.x + cosf(mob.angle - sideAngleOffset) * (mob.radius * 0.5f),
                    mobScreenPos.y + sinf(mob.angle - sideAngleOffset) * (mob.radius * 0.5f)
                };
                DrawCircleV(leftEyePos, 3.0f, pigEye);
                DrawCircleV(rightEyePos, 3.0f, pigEye);

            }

            else if (mob.type == 2) {
                Color wolfColor = GetColor(0x808080FF);
                Color wolfEye = (mob.health < 25.0f) ? WHITE : YELLOW;

                DrawCircleV(mobScreenPos, mob.radius + 2, BLACK);
                DrawCircleV(mobScreenPos, mob.radius, wolfColor);

                Vector2 snoutPos = {
                    mobScreenPos.x + cosf(mob.angle) * (mob.radius * 0.6f),
                    mobScreenPos.y + sinf(mob.angle) * (mob.radius * 0.6f)
                };

                DrawCircleV(snoutPos, mob.radius * 0.5f + 2, BLACK);
                DrawCircleV(snoutPos, mob.radius * 0.5f, wolfColor);

                float sideAngleOffset = 0.5f;

                Vector2 leftEyePos = {
                    mobScreenPos.x + cosf(mob.angle + sideAngleOffset) * (mob.radius * 0.5f),
                    mobScreenPos.y + sinf(mob.angle + sideAngleOffset) * (mob.radius * 0.5f)
                };

                Vector2 rightEyePos = {
                    mobScreenPos.x + cosf(mob.angle - sideAngleOffset) * (mob.radius * 0.5f),
                    mobScreenPos.y + sinf(mob.angle - sideAngleOffset) * (mob.radius * 0.5f)
                };
                DrawCircleV(leftEyePos, 3.0f, wolfEye);
                DrawCircleV(rightEyePos, 3.0f, wolfEye);
            }

            float barWidth = mob.radius * 2.0f;
            // float barWidth = 100.0f;
            float barHeight = 7.5f;
            float barX = mobScreenPos.x - mob.radius;
            float barY = mobScreenPos.y + mob.radius + 15.0f;

            Rectangle hpBg = { barX - 2, barY - 2, barWidth + 4, barHeight + 4 };

            DrawRectangleRounded(hpBg, 20.0f, 100, BLACK);

            // DrawRectangle(barX, barY, barWidth, barHeight, RED);

            float hpPercent = (float)mob.health / (float)mob.maxHealth;

            Rectangle hpBar = { barX, barY, barWidth * hpPercent, barHeight };

            if (hpPercent < 0.0f)
                hpPercent = 0.0f;
            DrawRectangleRounded(hpBar, 20.0f, 100, GREEN);
        }
    }
}

void Renderer::DrawInventoryUI(const Player& p)
{
    int slotsCount = 5;
    float slotSize = 100.0f;
    float spacing = 10.0f;
    float roundness = 0.2f;
    int segments = 10;

    float totalWidth = (slotSize * slotsCount) + (spacing * (slotsCount - 1));
    float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    float startY = GetScreenHeight() - slotSize - 20.0f;

    for (int i = 0; i < slotsCount; i++) {
        Rectangle slotRect = { startX + i * (slotSize + spacing), startY, slotSize, slotSize };

        bool isSelected = (p.GetSelectedSlot() == i);

        Color bgColor = isSelected ? Fade(BLACK, 0.8f) : Fade(BLACK, 0.5f);
        Color borderColor = isSelected ? GOLD : GRAY;
        float borderThickness = isSelected ? 3.0f : 1.5f;

        DrawRectangleRounded(slotRect, roundness, segments, bgColor);

        DrawRectangleRoundedLines(slotRect, roundness, segments, borderColor);

        if (i == 0) {
            DrawText(TextFormat("Axe\nLv.%d", p.GetAxeTier()), slotRect.x + 20, slotRect.y + 20, 30, GREEN);
        } else if (i == 1) {
            DrawText(TextFormat("Pick\nLv.%d", p.GetPickaxeTier()), slotRect.x + 20, slotRect.y + 20, 30, BLUE);
        } else if (i == 2) {
            DrawText(TextFormat("Wood\nx%d", p.GetWoodCount()), slotRect.x + 15, slotRect.y + 20, 30, ORANGE);
        } else if (i == 3) {
            DrawText(TextFormat("Rock\nx%d", p.GetStoneCount()), slotRect.x + 15, slotRect.y + 20, 30, LIGHTGRAY);
        } else if (i == 4) {
            DrawText(TextFormat("Gold\nx%d", p.GetGoldCount()), slotRect.x + 15, slotRect.y + 20, 30, YELLOW);
        }
    }
}

void Renderer::DrawButton(const Button& btn)
{
    Color currentBgColor = btn.isHovered ? btn.hoverColor : btn.baseColor;
    float borderThickness = btn.isHovered ? 5.0f : 5.0f;

    DrawRectangleRounded(btn.rect, 0.25f, 8, currentBgColor);

    int fontSize = 50;
    int textWidth = MeasureText(btn.text, fontSize);
    float textX = btn.rect.x + (btn.rect.width - textWidth) / 2.0f;
    float textY = btn.rect.y + (btn.rect.height - fontSize) / 2.0f;

    DrawText(btn.text, (int)textX + 2, (int)textY + 2, fontSize, BLACK);
    DrawText(btn.text, (int)textX, (int)textY, fontSize, WHITE);
}

void Renderer::DrawMainMenu(const Button& playButton)
{
    int gridSize = 40;
    for (int x = 0; x < GetScreenWidth(); x += gridSize) {
        DrawLine(x, 0, x, GetScreenHeight(), Fade(DARKGRAY, 0.15f));
    }
    for (int y = 0; y < GetScreenHeight(); y += gridSize) {
        DrawLine(0, y, GetScreenWidth(), y, Fade(DARKGRAY, 0.15f));
    }

    const char* title = "ASHCORE";
    int titleFontSize = 100;
    int titleWidth = MeasureText(title, titleFontSize);
    int titleX = GetScreenWidth() / 2 - titleWidth / 2;
    int titleY = GetScreenHeight() / 2 - 120;

    DrawText(title, titleX + 4, titleY + 4, titleFontSize, GetColor(0x111111FF));
    DrawText(title, titleX, titleY, titleFontSize, GetColor(0xDDDDDDFF));

    DrawButton(playButton);
}

void Renderer::DrawDeathScreen(const World& world, const Player& player, const Vector2& mapOffset,
    const Vector2& leftHand, const Vector2& rightHand, float handRad, float deathTimer)
{
    DrawWorld(world, player, mapOffset);
    DrawMobs(world.mobs, player, mapOffset);
    DrawPlayer(player, leftHand, rightHand, handRad);
    DrawStructures(world, player, mapOffset);

    float fadeAlpha = (3.0f - deathTimer) / 3.0f;
    if (fadeAlpha > 0.75f)
        fadeAlpha = 0.75f;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));

    if (deathTimer < 3.0f) {
        const char* deathText = "YOU WERE SLAIN";
        int fontSize = 50;

        float pulse = sinf(GetTime() * 5.0f) * 10.0f;
        int finalFontSize = fontSize + (int)pulse;

        int textWidth = MeasureText(deathText, finalFontSize);
        int textX = GetScreenWidth() / 2 - textWidth / 2;
        int textY = GetScreenHeight() / 2 - (finalFontSize / 2);

        DrawText(deathText, textX + 3, textY + 3, finalFontSize, GetColor(0x4a0000FF));
        DrawText(deathText, textX, textY, finalFontSize, RED);
    }
}
