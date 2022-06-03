#pragma once
#ifndef DAMAGETRACKINGCOMPONENT_H
#define DAMAGETRACKINGCOMPONENT_H
#include "BaseHeader.h"
#include <vector>
#include <iostream>
#include <map>

// Different types of damage.
enum DAMAGE_TYPE {
	NONE,
	PLASMA,
	EXPLOSIVE,
	KINETIC, //bullets
	IMPACT, //smacking into something
	VELOCITY, // moving too fast
	EMP
};

//Convenience map for strings to types used for loading weapon data.
const std::map<std::string, DAMAGE_TYPE> damageStrings = {
	{"none", DAMAGE_TYPE::NONE},
	{"plasma", PLASMA},
	{"explosive", EXPLOSIVE},
	{"kinetic", KINETIC},
	{"impact", IMPACT},
	{"velocity", VELOCITY},
	{"emp", EMP}
};

/*
* A damage instance incldues the time of being struck, who its from, and who it's to, along with the type and amount.
* They are handled properly in the DamageSystem.cpp file.
*/
struct DamageInstance
{
	DamageInstance(flecs::entity fr, flecs::entity to, DAMAGE_TYPE type, f32 amt, u32 time) : from(fr), to(to), type(type), amount(amt), time(time){}
	u32 time;
	flecs::entity from;
	flecs::entity to;
	DAMAGE_TYPE type;
	f32 amount;
};

/*
* The damage tracking component does what it says on the tin - tracks damage to anything with a health bar.
* When a thing takes damage, the attacker registers a damage instance on the victim's damage tracking component, which then gets applied to the health
* bar in the appropriate system.
*/
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