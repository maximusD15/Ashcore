#pragma once
#include <raylib.h>

const int SCREEN_W = 1920;
const int SCREEN_H = 1080;

const float MAP_SIZE = 50000.0f;
const float MAP_HALF = MAP_SIZE / 2.0f;

struct Button {
    Rectangle rect;
    const char* text;
    Color baseColor;
    Color hoverColor;
    bool isHovered;

    void Update()
    {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, rect);
    }

    bool isClicked()
    {
        return isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
};
