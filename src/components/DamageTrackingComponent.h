#pragma once
#ifndef DAMAGETRACKINGCOMPONENT_H
#define DAMAGETRACKINGCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"
#include <vector>
#include <iostream>

enum DAMAGE_TYPE {
	NONE,
	PLASMA,
	EXPLOSIVE,
	IMPACT,
	VELOCITY, // moving too fast
	EMP
};

struct DamageInstance
{
	DamageInstance(EntityId fr, EntityId to, DAMAGE_TYPE type, f32 amt, u32 time) : from(fr), to(to), type(type), amount(amt), time(time){}
	u32 time;
	EntityId from;
	EntityId to;
	DAMAGE_TYPE type;
	f32 amount;
};

struct DamageTrackingComponent
{
	std::vector<DamageInstance> instances;
	DAMAGE_TYPE lastDamageType = DAMAGE_TYPE::NONE;
	u32 lastDamageTime;
	void registerDamageInstance(DamageInstance dmg) {
		instances.push_back(dmg);
	}
};

#endif 