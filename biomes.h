#pragma once
#include <cmath>
#include <raylib.h>

enum BiomeType {
    BIOME_BOSS_LAIR,
    BIOME_LAVA_RING,
    BIOME_WASTELAND,
    BIOME_FOREST,
    BIOME_DESERT,
    BIOME_WINTER
};

inline BiomeType GetBiomeAt(Vector2 worldPos)
{
    float distance = sqrtf(worldPos.x * worldPos.x + worldPos.y * worldPos.y);

    if (distance < 500.0f)
        return BIOME_BOSS_LAIR;
    if (distance < 1000.0f)
        return BIOME_LAVA_RING;
    if (distance < 3500.0f)
        return BIOME_WASTELAND;
    if (distance < 10000.0f)
        return BIOME_FOREST;
    if (distance < 18000.0f)
        return BIOME_DESERT;
    return BIOME_WINTER;
}

inline Color GetBiomeColor(BiomeType type)
{
    switch (type) {
    case BIOME_BOSS_LAIR:
        return GetColor(0x3D2314FF);
    case BIOME_LAVA_RING:
        return GetColor(0x9E2A2BFF);
    case BIOME_WASTELAND:
        return GetColor(0x5A3A22FF);
    case BIOME_FOREST:
        return GetColor(0x1B4600E0);
    case BIOME_DESERT:
        return GetColor(0xDEB887FF);
    case BIOME_WINTER:
        return GetColor(0xEAEAEAFF);
    default:
        return GetColor(0x1B4600E0);
    }
}
