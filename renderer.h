#pragma once

#include "common.h"
#include "player.h"
#include "world.h"
#include <raylib.h>

class Renderer {
public:
    void DrawWorld(const World& world, const Player& player, const Vector2& mapOffset);
    void DrawPlayer(const Player& player, const Vector2& leftHandPos, const Vector2& rightHandPos, float handRadius);
    void DrawMobs(const std::vector<Mob>& mobs, const Player& player, const Vector2& mapOffset);
    void DrawStructures(const World& world, const Player& player, const Vector2& mapOffset);
    void DrawUI(const Player& player);
    void DrawInventoryUI(const Player& p);
    void DrawMainMenu(const Button& playButton);
    void DrawDeathScreen(const World& world, const Player& player, const Vector2& mapOffset,
        const Vector2& leftHand, const Vector2& rightHand, float handRad, float deathTimer);
    void DrawButton(const Button& btn);
};
