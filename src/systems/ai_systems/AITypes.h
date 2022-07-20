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
struct ThrustComponent;

class AIType
{
public:
	virtual void stateCheck(AIComponent* aiComp, SensorComponent* sensors, HealthComponent* hp) = 0;
	virtual bool distanceToWingCheck(AIComponent* aiComp, BulletRigidBodyComponent* rbc) = 0;
	virtual bool combatDistanceToWingCheck(AIComponent* aiComp, BulletRigidBodyComponent* rbc) = 0;
	virtual void idle(ThrustComponent* thrust, ShipComponent* ship, BulletRigidBodyComponent* rbc) = 0;
	virtual void flee(ThrustComponent* thrust, ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, flecs::entity fleeTarget) = 0;
	virtual void pursue(ThrustComponent* thrust, ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, SensorComponent* sensors, flecs::entity pursuitTarget, f32 dt) = 0;
	virtual void pursueOnWing(ThrustComponent* thrust, AIComponent* aiComp, ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, SensorComponent* sensors, flecs::entity pursuitTarget, f32 dt) = 0;
	virtual void formOnWing(ThrustComponent* thrust, AIComponent* aiComp, ShipComponent* ship, BulletRigidBodyComponent* rbc, f32 dt) = 0;
};

class DefaultShipAI : public AIType
{
public:
	virtual void stateCheck(AIComponent* aiComp, SensorComponent* sensors, HealthComponent* hp);
	virtual bool distanceToWingCheck(AIComponent* aiComp, BulletRigidBodyComponent* rbc);
	virtual bool combatDistanceToWingCheck(AIComponent* aiComp, BulletRigidBodyComponent* rbc);
	virtual void idle(ThrustComponent* thrust, ShipComponent* ship, BulletRigidBodyComponent* rbc);
	virtual void flee(ThrustComponent* thrust, ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, flecs::entity fleeTarget);
	virtual void pursue(ThrustComponent* thrust, ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, SensorComponent* sensors, flecs::entity pursuitTarget, f32 dt);
	virtual void pursueOnWing(ThrustComponent* thrust, AIComponent* aiComp, ShipComponent* ship, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, SensorComponent* sensors, flecs::entity pursuitTarget, f32 dt);
	virtual void formOnWing(ThrustComponent* thrust, AIComponent* aiComp, ShipComponent* ship, BulletRigidBodyComponent* rbc, f32 dt);
};

class AceAI : public DefaultShipAI
{
public:
private:
};
#endif 