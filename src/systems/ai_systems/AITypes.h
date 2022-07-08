#pragma once
#ifndef AITYPES_H
#define AITYPES_H
#include "BaseHeader.h"
#include "AIComponent.h"

struct ShipComponent;
struct BulletRigidBodyComponent;
struct SensorComponent;
struct IrrlichtComponent;
struct HealthComponent;

class AIType
{
public:
	virtual void stateCheck(AIComponent* aiComp, SensorComponent* sensors, HealthComponent* hp) = 0;
	virtual void idle(ShipComponent* ship, BulletRigidBodyComponent* rbc) = 0;
	virtual void flee(ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, flecs::entity fleeTarget) = 0;
	virtual void pursue(ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, SensorComponent* sensors, flecs::entity pursuitTarget, f32 dt) = 0;
};

class DefaultAI : public AIType
{
public:
	virtual void stateCheck(AIComponent* aiComp, SensorComponent* sensors, HealthComponent* hp);
	virtual void idle(ShipComponent* ship, BulletRigidBodyComponent* rbc);
	virtual void flee(ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, flecs::entity fleeTarget);
	virtual void pursue(ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, SensorComponent* sensors, flecs::entity pursuitTarget, f32 dt);
};

class AceAI : public DefaultAI
{
public:
private:
};
#endif 