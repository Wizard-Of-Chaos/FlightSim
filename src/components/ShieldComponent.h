#pragma once
#ifndef SHIELDCOMPONENT_H
#define SHIELDCOMPONENT_H
#include "BaseHeader.h"

const f32 DEFAULT_MAX_SHIELDS = 100.f;
const f32 DEFAULT_RECHARGE_RATE = 10.f;
const f32 DEFAULT_RECHARGE_DELAY = 5.f;

/*
* Shield components do what you think they do. They effectively act as an extra health bar and block most types of damage before it actually
* gets to the health bar. They include the recharge rate and the delay before recharging, as well as the time since last hit to keep track of when
* to start recharging.
* 
* Shields don't block all types of damage. For more examples, see the damage tracking system.
*/
struct ShieldComponent
{
	f32 shields;
	f32 maxShields;
	f32 rechargeRate; //how much shields per second
	f32 rechargeDelay; //how long before recharge, in seconds
	f32 timeSinceLastHit; //how long since a hit, in seconds
};

#endif 