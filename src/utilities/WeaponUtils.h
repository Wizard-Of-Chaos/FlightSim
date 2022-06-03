#pragma once
#ifndef WEAPONUTILS_H
#define WEAPONUTILS_H
#include "BaseHeader.h"
/*
* This function gets called when a projectile smacks into something else, and it fires off the appropriate functions.
* For example, the impulse cannon and missiles call an explosion on impact, and the gravity bolas sets a couple of entity flags.
*/
void handleProjectileImpact(flecs::entity proj, flecs::entity impacted);

void impulseBlasterImpact(flecs::entity proj, flecs::entity impacted);
void missileImpact(flecs::entity proj);
void gravityBolasImpact(flecs::entity proj, flecs::entity impacted);
/*
* If weapons have anything they should be doing in their downtime, that'll get called here. For instance, the gravity bolas
* needs to keep track of how long the connection between two entities lasts.
*/
void handleSpecialWepFunctions(flecs::entity wep, f32 dt);

void gravityBolasHandler(flecs::entity wep, f32 dt);


#endif 