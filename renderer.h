#pragma once

#include "player.h"
#include "world.h"
#include <raylib.h>

class Renderer {
public:
    void DrawWorld(const World& world, const Player& player, const Vector2& mapOffset);
    void DrawPlayer(const Player& player, const Vector2& leftHandPos, const Vector2& rightHandPos, float handRadius);
    void DrawUI(const Player& player);
};
