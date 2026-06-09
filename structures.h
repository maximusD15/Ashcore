#pragma once
#include <raylib.h>

struct GrassBlade{
	Vector2 worldPos;
	float size;
};
struct Tree{
	Vector2 worldPos;
	float radius;
	int health;
	int maxHealth;
	Vector2 hitOffset = { 0.0f, 0.0f };
};
struct Rock{
	Vector2 worldPos;
	float radius;
	int health;
	int maxHealth;
	Vector2 hitOffset = { 0.0f, 0.0f };
};
struct ResourceDrop{
	Vector2 worldPos;
	int type;
	float radius;
};

