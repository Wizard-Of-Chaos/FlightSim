#include "ShipUpdateSystem.h"
#include <iostream>

void jetOn(IParticleEmitter* jet)
{
	if (!jet) return;
	jet->setMaxParticlesPerSecond(150);
	jet->setMinParticlesPerSecond(50);
}
void jetOff(IParticleEmitter* jet)
{
	if (!jet) return;
	jet->setMaxParticlesPerSecond(0);
	jet->setMinParticlesPerSecond(0);
}
void jetPairOn(IParticleEmitter* jet1, IParticleEmitter* jet2)
{
	jetOn(jet1);
	jetOn(jet2);
}

void jetPairOff(IParticleEmitter* jet1, IParticleEmitter* jet2)
{
	jetOff(jet1);
	jetOff(jet2);
}

void setPairDir(IParticleEmitter* jet1, IParticleEmitter* jet2, vector3df dir)
{
	if (!jet1 || !jet2) return;
	jet1->setDirection(dir * .02f);
	jet2->setDirection(dir * .02f);
}

void afterburnerJetOn(IParticleEmitter* engine, ILightSceneNode* light)
{
	if (!engine || !light) return;
	engine->setMaxParticlesPerSecond(500);
	engine->setMinParticlesPerSecond(450);
	light->setRadius(3.f);
}

void afterburnerJetOff(IParticleEmitter* engine, ILightSceneNode* light)
{
	if (!engine || !light) return;
	engine->setMaxParticlesPerSecond(300);
	engine->setMinParticlesPerSecond(100);
	light->setRadius(1.3f);
}

bool velocitySafetyCheck(f32 linVelocity, ThrustComponent* thrust, ShipComponent* ship, btVector3 velDir, btVector3 thrustDir)
{
	if (linVelocity >= thrust->linearMaxVelocity) {
		if (std::abs(velDir.angle(thrustDir)) * RADTODEG >= 90.f) {
			return true;
		}
		return ship->safetyOverride;
	}
	return true;
}

bool angularSafetyCheck(f32 angVelocity, ThrustComponent* thrust, ShipComponent* ship, btVector3 velDir, btVector3 thrustDir)
{
	if (angVelocity >= thrust->angularMaxVelocity) {
		if (std::abs(velDir.angle(thrustDir)) * RADTODEG >= 90.f) {
			return true;
		}
		return ship->safetyOverride;
	}
	return true;
}

void shipUpdateSystem(flecs::iter it, ThrustComponent* thrc, ShipComponent* shpc, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc, ShipParticleComponent* prtc)
{
	for (auto i : it) {
		f32 dt = it.delta_time();
		auto ship = &shpc[i];
		auto irr = &irrc[i];
		auto rbc = &rbcs[i];
		auto particles = &prtc[i];
		auto thrust = &thrc[i];

		flecs::entity entityId = it.entity(i);
		if (!entityId.is_alive()) return;

		btRigidBody* body = rbc->rigidBody;

		f32 linVel = body->getLinearVelocity().length();
		f32 angVel = body->getAngularVelocity().length();
		btVector3 linDir = body->getLinearVelocity();
		btVector3 angDir = body->getAngularVelocity();

		velocitySafeNormalize(linDir);
		velocitySafeNormalize(angDir);

		//Ugly looking, but all these emitters need adjusting every time the ship moves
		IParticleEmitter* up1 =nullptr, *up2=nullptr, *down1=nullptr, *down2=nullptr, 
			*left1=nullptr, *left2=nullptr, *right1=nullptr, *right2=nullptr, *back1=nullptr, *back2=nullptr;

		if(particles->upJetEmit[0]) up1 = particles->upJetEmit[0]->getEmitter();
		if(particles->upJetEmit[1]) up2 = particles->upJetEmit[1]->getEmitter();
		if(particles->downJetEmit[0]) down1 = particles->downJetEmit[0]->getEmitter();
		if(particles->downJetEmit[1]) down2 = particles->downJetEmit[1]->getEmitter();
		if(particles->leftJetEmit[0]) left1 = particles->leftJetEmit[0]->getEmitter();
		if(particles->leftJetEmit[1]) left2 = particles->leftJetEmit[1]->getEmitter();
		if(particles->rightJetEmit[0]) right1 = particles->rightJetEmit[0]->getEmitter();
		if(particles->rightJetEmit[1]) right2 = particles->rightJetEmit[1]->getEmitter();
		if(particles->reverseJetEmit[0]) back1 = particles->reverseJetEmit[0]->getEmitter();
		if(particles->reverseJetEmit[1]) back2 = particles->reverseJetEmit[1]->getEmitter();

		IVolumeLightSceneNode* engine = particles->engineJetEmit;

		setPairDir(up1, up2, getNodeUp(irr->node));
		setPairDir(down1, down2, getNodeDown(irr->node));
		setPairDir(left1, left2, getNodeLeft(irr->node));
		setPairDir(right1, right2, getNodeRight(irr->node));
		setPairDir(back1, back2, getNodeForward(irr->node));
		if(engine) engine->setScale(vector3df(1, 1, 1));

		jetPairOff(up1, up2);
		jetPairOff(down1, down2);
		jetPairOff(left1, left2);
		jetPairOff(right1, right2);
		jetPairOff(back1, back2);

		if (thrust->moves[STRAFE_DOWN]) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyDown(body))) {
				jetPairOn(up1, up2);
			}
			else {
				thrust->moves[STRAFE_DOWN] = false;
			}
		}
		if (thrust->moves[STRAFE_UP]) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyUp(body))) {
				jetPairOn(down1, down2);
			}
			else {
				thrust->moves[STRAFE_UP] = false;
			}
		}
		if (thrust->moves[STRAFE_LEFT]) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyLeft(body))) {
				jetPairOn(right1, right2);
			}
			else {
				thrust->moves[STRAFE_LEFT] = false;
			}
		}
		if (thrust->moves[STRAFE_RIGHT]) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyRight(body))) {
				jetPairOn(left1, left2);
			}
			else {
				thrust->moves[STRAFE_RIGHT] = false;
			}
		}
		if (thrust->moves[THRUST_FORWARD]) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyForward(body))) {
				//turn on jet pairs, maybe? or turn on the engine light more
			}
			else {
				thrust->moves[THRUST_FORWARD] = false;
			}
		}
		if (thrust->moves[THRUST_BACKWARD]) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyBackward(body))) {
				jetPairOn(back1, back2);
			}
			else {
				thrust->moves[THRUST_BACKWARD] = false;
			}
		}

		bool aft = thrust->moves[THRUST_BOOST];
		if (aft && ship->afterburnerFuel > 0) {
			if (velocitySafetyCheck(linVel, thrust, ship, linDir, getRigidBodyForward(body))) {
				ship->afterburnerFuel -= dt * ship->afterburnerFuelEfficiency;
			}
			else {
				thrust->moves[THRUST_BOOST] = false;
				aft = false;
			}
		}
		if (aft != ship->afterburnerOn && ship->afterburnerFuel > 0) {
			ship->afterburnerOn = aft;
		}

		if (thrust->moves[PITCH_UP]) {
			if (angularSafetyCheck(angVel, thrust, ship, angDir, getRigidBodyLeft(body))) {
				jetPairOn(down2, up1);
			}
			else {
				thrust->moves[PITCH_UP] = false;
			}
		}
		if (thrust->moves[PITCH_DOWN]) {
			if (angularSafetyCheck(angVel, thrust, ship, angDir, getRigidBodyRight(body))) {
				jetPairOn(down1, up2);
			}
			else {
				thrust->moves[PITCH_DOWN] = false;
			}
		}
		if (thrust->moves[YAW_LEFT]) {
			if (angularSafetyCheck(angVel, thrust, ship, angDir, getRigidBodyDown(body))) {
				jetPairOn(right2, left1);
			}
			else {
				thrust->moves[YAW_LEFT] = false;
			}
		}
		if (thrust->moves[YAW_RIGHT]) {
			if (angularSafetyCheck(angVel, thrust, ship, angDir, getRigidBodyUp(body))) {
				jetPairOn(right1, left2);
			}
			else {
				thrust->moves[YAW_LEFT] = false;
			}
		}
		if (thrust->moves[ROLL_LEFT]) {
			if (angularSafetyCheck(angVel, thrust, ship, angDir, getRigidBodyForward(body))) {

			}
			else {
				thrust->moves[ROLL_LEFT] = false;
			}
		}
		if (thrust->moves[ROLL_RIGHT]) {
			if (angularSafetyCheck(angVel, thrust, ship, angDir, getRigidBodyBackward(body))) {
			}
			else {
				thrust->moves[ROLL_LEFT] = false;
			}
		}
		//adjusts engine light
		f32 zPercent = rbc->rigidBody->getLinearVelocity().length() / thrust->linearMaxVelocity;
		if(engine) engine->setScale(vector3df(std::max(zPercent, 3.5f), std::max(5*zPercent, .1f), std::max(zPercent, 3.5f)));
	}
}