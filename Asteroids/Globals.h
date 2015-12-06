#pragma once

#include "Utility.h"

extern float updateFPS; // How many times per second fixed update methods are called
extern float fixedDeltaTime; // The interval (in seconds) between fixed update calls
extern Vector2 screenResolution;
extern float viewPaneZoom; // At '1' the viewable coordinates correspond 1:1 to the screen resolution
extern float visibleX, visibleY, halfVisibleX, halfVisibleY; // The number of units (for positioning) displayed on screen with the specific zoom settings
extern Color backgroundColor;
extern float screenBorder; // Objects aren't wrapped around until they have moved this many units off screen
extern float playerAcceleration; // When player presses forward, this amount is added to ship velocity every fixed update
extern float maxPlayerSpeed; // The player's velocity magnitude is capped at this amount
extern float playerRotationSpeed; // Speed the player turns at when holding down rotation keys
extern float baseProjectileSpeed; // How fast the projectile starts at (before adding the player's velocity to it)
extern float projectileSpawnOffset; // How many units in front of the ship should playerProjectiles spawn
extern float projectileLifetime; // How many seconds should playerProjectiles live before automatically destroying
extern int maxProjectiles; // The maximum number of playerProjectiles allowed on screen at once

extern float ProgramTime();