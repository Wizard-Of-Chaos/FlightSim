#pragma once
#ifndef SHIELDCOMPONENT_H
#define SHIELDCOMPONENT_H
#include "BaseHeader.h"

const f32 DEFAULT_MAX_SHIELDS = 100.f;
const f32 DEFAULT_RECHARGE_RATE = 10.f;
const f32 DEFAULT_RECHARGE_DELAY = 5.f;

struct ShieldComponent
{
	f32 shields;
	f32 maxShields;
	f32 rechargeRate; //how much shields per second
	f32 rechargeDelay; //how long before recharge, in seconds
	f32 timeSinceLastHit; //how long since a hit, in seconds
};

#endif 