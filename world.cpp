#include "world.h"
#include "common.h"
#include "player.h"
#include <cmath>
#include <raymath.h>

void World::Init()
{

    for (int i = 0; i < 1000; i++) {
        GrassBlade blade;
        blade.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        blade.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        blade.size = (float)GetRandomValue(4, 6);
        grassTiles.push_back(blade);
    }

    for (int i = 0; i < 2000; i++) {
        Tree t;
        t.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        t.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        t.radius = (float)GetRandomValue(70, 100);
        t.maxRadius = t.radius;
        t.targetRadius = t.radius;
        t.maxHealth = (t.radius < 80.0f) ? 80 : ((t.radius < 90.0f) ? 110 : 140);
        t.health = t.maxHealth;

        t.hitOffset = { 0, 0 };

        trees.push_back(t);
    }

    for (int i = 0; i < 1500; i++) {
        Rock r;
        r.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        r.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        r.radius = (float)GetRandomValue(70, 100);
        r.maxRadius = r.radius;
        r.targetRadius = r.radius;
        r.maxHealth = (r.radius < 80.0f) ? 120 : ((r.radius < 90.0f) ? 140 : 180);
        r.health = r.maxHealth;

        r.hitOffset = { 0, 0 };

        rocks.push_back(r);
    }

    for (int i = 0; i < 500; i++) {
        Gold g;
        g.worldPos.x = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        g.worldPos.y = (float)GetRandomValue(-MAP_HALF, MAP_HALF);
        g.radius = (float)GetRandomValue(70, 100);
        g.maxRadius = g.radius;
	g.targetRadius = g.radius;
        g.maxHealth = (g.radius < 80.0f) ? 120 : ((g.radius < 90.0f) ? 140 : 180);
        g.health = g.maxHealth;

        g.hitOffset = { 0, 0 };

        golds.push_back(g);
    }
}

void World::HandleCollisions(Player& p, Vector2& mapOffset, float oldOffsetX, float oldOffsetY)
{
    for (const auto& t : trees) {
        float dx = mapOffset.x - t.worldPos.x;
        float dy = mapOffset.y - t.worldPos.y;

        float distance = sqrtf(dx * dx + dy * dy);

        float minDist = p.GetRadius() + t.radius - 15.0f;

        if (distance < minDist) {
            if (distance == 0.0f) {
                dx = 1.0f;
                distance = 1.0f;
            }
            float overlap = minDist - distance;

            float nx = dx / distance;
            float ny = dy / distance;

            mapOffset.x += nx * overlap;
            mapOffset.y += ny * overlap;
        }
    }

    for (const auto& r : rocks) {
        float dx = mapOffset.x - r.worldPos.x;
        float dy = mapOffset.y - r.worldPos.y;

        float distance = sqrtf(dx * dx + dy * dy);
        float minDist = p.GetRadius() + r.radius;

        if (distance < minDist) {
            if (distance == 0.0f) {
                dx = 1.0f;
                distance = 1.0f;
            }
            float overlap = minDist - distance;

            float nx = dx / distance;
            float ny = dy / distance;

            mapOffset.x += nx * overlap;
            mapOffset.y += ny * overlap;
        }
    }

    for (const auto& g : golds) {
        float dx = mapOffset.x - g.worldPos.x;
        float dy = mapOffset.y - g.worldPos.y;

        float distance = sqrtf(dx * dx + dy * dy);
        float minDist = p.GetRadius() + g.radius;

        if (distance < minDist) {
            if (distance == 0.0f) {
                dx = 1.0f;
                distance = 1.0f;
            }
            float overlap = minDist - distance;

            float nx = dx / distance;
            float ny = dy / distance;

            mapOffset.x += nx * overlap;
            mapOffset.y += ny * overlap;
        }
    }
}
void World::HandleMeleeAnimation(Player& p, float deltaTime, float strikeFactor,
    float& leftHandOrbit, float& rightHandOrbit,
    float& leftHandAngleOffset, float& rightHandAngleOffset)
{
    if (strikeFactor > 0.001f) {
        if (p.GetAttackCounter() == 1) {
            rightHandOrbit += strikeFactor * 20.0f;
            rightHandAngleOffset -= strikeFactor * 0.6f;
            leftHandOrbit -= strikeFactor;
        } else {
            leftHandOrbit += strikeFactor * 20.0f;
            leftHandAngleOffset -= strikeFactor * 0.6f;
            rightHandOrbit -= strikeFactor;
        }
    }
}

void World::UpdateVisualEffects()
{
    float deltaTime = GetFrameTime();

    for (auto& t : trees) {
        t.hitOffset.x *= 0.85f;
        t.hitOffset.y *= 0.85f;
        if (fabsf(t.hitOffset.x) < 0.05f)
            t.hitOffset.x = 0.0f;
        if (fabsf(t.hitOffset.y) < 0.05f)
            t.hitOffset.y = 0.0f;
        t.radius += (t.targetRadius - t.radius) * 15.0f * deltaTime;
    }
    for (auto& r : rocks) {
        r.hitOffset.x *= 0.85f;
        r.hitOffset.y *= 0.85f;
        if (fabsf(r.hitOffset.x) < 0.05f)
            r.hitOffset.x = 0.0f;
        if (fabsf(r.hitOffset.y) < 0.05f)
            r.hitOffset.y = 0.0f;
        r.radius += (r.targetRadius - r.radius) * 15.0f * deltaTime;
    }
    for (auto& g : golds) {
        g.hitOffset.x *= 0.85f;
        g.hitOffset.y *= 0.85f;
        if (fabsf(g.hitOffset.x) < 0.05)
            g.hitOffset.x = 0.0f;
        if (fabsf(g.hitOffset.y) < 0.05)
            g.hitOffset.y = 0.0f;
        g.radius += (g.targetRadius - g.radius) * 15.0f * deltaTime;
    }
}

void World::HandleAttackHitbox(Player& p, const Vector2& mapOffset)
{
    int damage = 1;
    if (p.GetActiveWeapon() == 1)
        damage = p.GetAxeTier() * GetRandomValue(1, 2);

    Vector2 mousePos = GetMousePosition();
    float attackAngle = atan2f(mousePos.y - p.GetScreenCenter().y, mousePos.x - p.GetScreenCenter().x);

    Vector2 hitWorldPos = {
        mapOffset.x + cosf(attackAngle) * (p.GetRadius() + 35.0f),
        mapOffset.y + sinf(attackAngle) * (p.GetRadius() + 35.0f)
    };
    for (size_t i = 0; i < trees.size(); i++) {
        float dx = trees[i].worldPos.x - hitWorldPos.x;
        float dy = trees[i].worldPos.y - hitWorldPos.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < trees[i].radius + 10.0f) {
            trees[i].health -= damage;

            float hpPercent = (float)trees[i].health / (float)trees[i].maxHealth;

            if (hpPercent <= 0.25f) {
                trees[i].targetRadius = trees[i].maxRadius * 0.70f;
            } else if (hpPercent <= 0.50f) {
                trees[i].targetRadius = trees[i].maxRadius * 0.80f;
            } else if (hpPercent <= 0.75f) {
                trees[i].targetRadius = trees[i].maxRadius * 0.90f;
            } else {
                trees[i].targetRadius = trees[i].maxRadius;
            }

            float screenTreeX = trees[i].worldPos.x - mapOffset.x + p.GetScreenCenter().x;
            float screenTreeY = trees[i].worldPos.y - mapOffset.y + p.GetScreenCenter().y;
            float pushAngle = atan2f(screenTreeY - p.GetScreenCenter().y, screenTreeX - p.GetScreenCenter().x);

            trees[i].hitOffset.x = cosf(pushAngle) * 12.0f;
            trees[i].hitOffset.y = sinf(pushAngle) * 12.0f;

            ResourceDrop drop;
            drop.type = 1;
            drop.radius = 15.0f;
            float randomSpread = (GetRandomValue(-45, 45) * PI) / 180.0f;
            float dropAngle = pushAngle + PI + randomSpread;

            drop.worldPos.x = trees[i].worldPos.x - cosf(pushAngle) * trees[i].radius + cosf(dropAngle) * 35.0f;
            drop.worldPos.y = trees[i].worldPos.y - sinf(pushAngle) * trees[i].radius + sinf(dropAngle) * 35.0f;

            drops.push_back(drop);
            return;
        }
    }

    int mineDamage = 0;
    if (p.GetActiveWeapon() == 2)
        mineDamage = p.GetPickaxeTier() * GetRandomValue(1, 2);

    if (mineDamage > 0) {
        for (size_t i = 0; i < rocks.size(); i++) {
            float dx = rocks[i].worldPos.x - hitWorldPos.x;
            float dy = rocks[i].worldPos.y - hitWorldPos.y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < rocks[i].radius + 10.0f) {
                rocks[i].health -= mineDamage;

                float hpPercent = (float)rocks[i].health / (float)rocks[i].maxHealth;

                if (hpPercent <= 0.30f) {
                    rocks[i].targetRadius = rocks[i].maxRadius * 0.70f;
                } else if (hpPercent <= 0.55f) {
                    rocks[i].targetRadius = rocks[i].maxRadius * 0.80f;
                } else if (hpPercent <= 0.80f) {
                    rocks[i].targetRadius = rocks[i].maxRadius * 0.90f;
                } else {
                    rocks[i].targetRadius = rocks[i].maxRadius;
                }

                float screenRockX = rocks[i].worldPos.x - mapOffset.x + p.GetScreenCenter().x;
                float screenRockY = rocks[i].worldPos.y - mapOffset.y + p.GetScreenCenter().y;
                float pushAngle = atan2f(screenRockY - p.GetScreenCenter().y, screenRockX - p.GetScreenCenter().x);

                rocks[i].hitOffset.x = cosf(pushAngle) * 12.0f;
                rocks[i].hitOffset.y = sinf(pushAngle) * 12.0f;

                ResourceDrop drop;
                drop.type = 2;
                drop.radius = 15.0f;
                float randomSpread = (GetRandomValue(-45, 45) * PI) / 180.0f;
                float dropAngle = pushAngle + PI + randomSpread;

                drop.worldPos.x = rocks[i].worldPos.x - cosf(pushAngle) * rocks[i].radius + cosf(dropAngle) * 35.0f;
                drop.worldPos.y = rocks[i].worldPos.y - sinf(pushAngle) * rocks[i].radius + sinf(dropAngle) * 35.0f;

                drops.push_back(drop);
                return;
            }
        }
    }
    if (mineDamage > 1 && p.GetPickaxeTier() > 1) {
        for (size_t i = 0; i < golds.size(); i++) {
            float dx = golds[i].worldPos.x - hitWorldPos.x;
            float dy = golds[i].worldPos.y - hitWorldPos.y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < golds[i].radius + 10.0f) {
                golds[i].health -= mineDamage;

                float hpPercent = (float)golds[i].health / (float)golds[i].maxHealth;

                if (hpPercent <= 0.30f) {
                    golds[i].targetRadius = golds[i].maxRadius * 0.70f;
                } else if (hpPercent <= 0.55f) {
                    golds[i].targetRadius = golds[i].maxRadius * 0.80f;
                } else if (hpPercent <= 0.80f) {
                    golds[i].targetRadius = golds[i].maxRadius * 0.90f;
                } else {
                    golds[i].targetRadius = golds[i].maxRadius;
                }
                float screenGoldX = golds[i].worldPos.x - mapOffset.x + p.GetScreenCenter().x;
                float screenGoldY = golds[i].worldPos.y - mapOffset.y + p.GetScreenCenter().y;
                float pushAngle = atan2f(screenGoldY - p.GetScreenCenter().y, screenGoldX - p.GetScreenCenter().x);

                golds[i].hitOffset.x = cosf(pushAngle) * 12.0f;
                golds[i].hitOffset.y = sinf(pushAngle) * 12.0f;

                ResourceDrop drop;
                drop.type = 3;
                drop.radius = 15.0f;
                float randomSpread = (GetRandomValue(-45, 45) * PI) / 180.0f;
                float dropAngle = pushAngle + PI + randomSpread;

                drop.worldPos.x = golds[i].worldPos.x - cosf(pushAngle) * golds[i].radius + cosf(dropAngle) * 35.0f;
                drop.worldPos.y = golds[i].worldPos.y - sinf(pushAngle) * golds[i].radius + sinf(dropAngle) * 35.0f;

                drops.push_back(drop);
                return;
            }
        }
    }
}
//}

void World::UpdateWorldEntities(Player& p, const Vector2& mapOffset)
{
    for (auto it = trees.begin(); it != trees.end();) {
        if (it->health <= 0)
            it = trees.erase(it);
        else
            ++it;
    }
    for (auto it = rocks.begin(); it != rocks.end();) {
        if (it->health <= 0)
            it = rocks.erase(it);
        else
            ++it;
    }
    for (auto it = golds.begin(); it != golds.end();) {
        if (it->health <= 0)
            it = golds.erase(it);
        else
            ++it;
    }
    for (auto it = drops.begin(); it != drops.end();) {
        float dx = it->worldPos.x - mapOffset.x;
        float dy = it->worldPos.y - mapOffset.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (p.GetDropPickupTimer() <= 0.0f && dist < (p.GetRadius() + it->radius + 5.0f)) {
            if (it->type == 1) {
                if (p.GetActiveWeapon() == 1) {
                    //p.AddWood((p.GetAxeTier() == 1) ? GetRandomValue(1, 3) : GetRandomValue(2, 3));
		    if(p.GetAxeTier() == 1) p.AddWood(GetRandomValue(1, 2));
		    if(p.GetAxeTier() == 2) p.AddWood(GetRandomValue(2, 3));
		    if(p.GetAxeTier() == 3) p.AddWood(GetRandomValue(2, 4));
                } else
                    p.AddWood(1);
            }
            if (it->type == 2) {
                if (p.GetActiveWeapon() == 2) {
                    //p.AddStone((p.GetPickaxeTier() == 2) ? GetRandomValue(1, 2) : GetRandomValue(1, 3));
		    if(p.GetPickaxeTier() == 1) p.AddStone(GetRandomValue(1, 2));
		    if(p.GetPickaxeTier() == 2) p.AddStone(GetRandomValue(1, 3));
		    if(p.GetPickaxeTier() == 3) p.AddStone(GetRandomValue(2, 3));
                } else
                    p.AddStone(1);
            }
            if (it->type == 3) {
                if (p.GetActiveWeapon() == 2) {
                    //p.AddGold((p.GetPickaxeTier() > 2) ? GetRandomValue(1, 2) : 1);
		    if(p.GetPickaxeTier() == 2) p.AddGold(1);
		    if(p.GetPickaxeTier() == 3) p.AddGold(GetRandomValue(1, 2));
                } else
                    p.AddGold(1);
            }
            it = drops.erase(it);
        } else
            ++it;
    }
}
void World::Update(Player& p, Vector2& mapOffset, float deltaTime,
    Vector2& leftHandPos, Vector2& rightHandPos, float& handRadius)
{
    Vector2 targetVelocity = { 0.0f, 0.0f };

    float oldOffsetX = mapOffset.x;
    float oldOffsetY = mapOffset.y;

    float friction = 10.0f;

    if (p.GetJumpTimer() <= 0.0f) {
        Vector2 moveDir = { 0, 0 };

        if (IsKeyDown(KEY_A))
            moveDir.x -= 1.0f;
        if (IsKeyDown(KEY_D))
            moveDir.x += 1.0f;
        if (IsKeyDown(KEY_W))
            moveDir.y -= 1.0f;
        if (IsKeyDown(KEY_S))
            moveDir.y += 1.0f;

        if (moveDir.x != 0 || moveDir.y != 0)
            moveDir = Vector2Normalize(moveDir);

        targetVelocity = {
            moveDir.x * p.GetSpeed(),
            moveDir.y * p.GetSpeed()
        };
        p.GetVelocity().x += (targetVelocity.x - p.GetVelocity().x) * friction * deltaTime;
        p.GetVelocity().y += (targetVelocity.y - p.GetVelocity().y) * friction * deltaTime;
    } else {
    }
    mapOffset.x += p.GetVelocity().x * deltaTime;
    mapOffset.y += p.GetVelocity().y * deltaTime;

    if (mapOffset.x < -MAP_HALF)
        mapOffset.x = -MAP_HALF;
    if (mapOffset.x > MAP_HALF)
        mapOffset.x = MAP_HALF;
    if (mapOffset.y < -MAP_HALF)
        mapOffset.y = -MAP_HALF;
    if (mapOffset.y > MAP_HALF)
        mapOffset.y = MAP_HALF;

    p.HandleMovement(deltaTime);
    HandleCollisions(p, mapOffset, oldOffsetX, oldOffsetY);
    p.Update(deltaTime);
    p.HandleInput();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !p.IsAttacking() && p.GetAttackCooldownTimer() <= 0.0f) {
        p.TriggerAttack();
        HandleAttackHitbox(p, mapOffset);
    }

    float leftHandOrbit = p.GetRadius() + 2.0f;
    float rightHandOrbit = p.GetRadius() + 2.0f;
    float leftHandAngleOffset = 0.75f;
    float rightHandAngleOffset = 0.75f;

    float rawStrikeFactor = sinf((p.GetAttackCooldownTimer() / 0.5f) * PI);
    if (rawStrikeFactor < 0.0f)
        rawStrikeFactor = 0.0f;

    if (!p.IsAttacking())
        rawStrikeFactor = 0.0f;

    static float smoothStrikeFactor = 0.0f;
    smoothStrikeFactor += (rawStrikeFactor - smoothStrikeFactor) * 30.0f * deltaTime;

    float visualAngle = p.GetAngle();
    if (smoothStrikeFactor > 0.001) {
        float angleModifier = (p.GetAttackCounter() == 0) ? 0.3f : -0.3f;
        visualAngle += smoothStrikeFactor * angleModifier;
    }

    HandleMeleeAnimation(p, deltaTime, smoothStrikeFactor,
        leftHandOrbit, rightHandOrbit,
        leftHandAngleOffset, rightHandAngleOffset);

    leftHandPos = {
        p.GetScreenCenter().x + leftHandOrbit * cosf(visualAngle - leftHandAngleOffset),
        p.GetScreenCenter().y + leftHandOrbit * sinf(visualAngle - leftHandAngleOffset)
    };
    rightHandPos = {
        p.GetScreenCenter().x + rightHandOrbit * cosf(visualAngle + rightHandAngleOffset),
        p.GetScreenCenter().y + rightHandOrbit * sinf(visualAngle + rightHandAngleOffset)
    };

    float progress = p.GetJumpTimer() / p.GetJumpDuration();
    handRadius = 8.0f + sinf(progress * PI) * 2.0f;

    UpdateVisualEffects();
    UpdateWorldEntities(p, mapOffset);
}
