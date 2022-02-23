#include "ShipUpdateSystem.h"

void turnOnJets(IParticleEmitter* jet1, IParticleEmitter* jet2)
{
	jet1->setMaxParticlesPerSecond(800);
	jet1->setMinParticlesPerSecond(500);
	jet2->setMaxParticlesPerSecond(800);
	jet2->setMinParticlesPerSecond(500);
}

void turnOffJets(IParticleEmitter* jet1, IParticleEmitter* jet2)
{
	jet1->setMaxParticlesPerSecond(0);
	jet1->setMinParticlesPerSecond(0);
	jet2->setMaxParticlesPerSecond(0);
	jet2->setMinParticlesPerSecond(0);
}

void shipUpdateSystem(Scene& scene, f32 dt)
{
	for (auto entityId : SceneView<ShipComponent, BulletRigidBodyComponent>(scene)) {
		auto ship = scene.get<ShipComponent>(entityId);
		auto irr = scene.get<IrrlichtComponent>(entityId);
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		btRigidBody* body = &rbc->rigidBody;

		btVector3 torque(0, 0, 0);
		btVector3 force(0, 0, 0);

		IParticleEmitter* up1 = ship->upJetEmit[0]->getEmitter();
		IParticleEmitter* up2 = ship->upJetEmit[1]->getEmitter();
		IParticleEmitter* down1 = ship->downJetEmit[0]->getEmitter();
		IParticleEmitter* down2 = ship->downJetEmit[1]->getEmitter();
		IParticleEmitter* left1 = ship->leftJetEmit[0]->getEmitter();
		IParticleEmitter* left2 = ship->leftJetEmit[1]->getEmitter();
		IParticleEmitter* right1 = ship->rightJetEmit[0]->getEmitter();
		IParticleEmitter* right2 = ship->rightJetEmit[1]->getEmitter();

		up1->setDirection(getNodeUp(irr->node) * .02f);
		up2->setDirection(getNodeUp(irr->node) * .02f);
		down1->setDirection(getNodeDown(irr->node) * .02f);
		down2->setDirection(getNodeDown(irr->node) * .02f);
		left1->setDirection(getNodeLeft(irr->node) * .02f);
		left2->setDirection(getNodeLeft(irr->node) * .02f);
		right1->setDirection(getNodeRight(irr->node) * .02f);
		right2->setDirection(getNodeRight(irr->node) * .02f);

		turnOffJets(up1, up2);
		turnOffJets(down1, down2);
		turnOffJets(left1, left2);
		turnOffJets(right1, right2);

		if (ship->moves[SHIP_STRAFE_DOWN]) {
			turnOnJets(up1, up2);
			force += getForceDown(body, ship);
		}
		if (ship->moves[SHIP_STRAFE_UP]) {
			turnOnJets(down1, down2);
			force += getForceUp(body, ship);
		}
		if (ship->moves[SHIP_STRAFE_LEFT]) {
			turnOnJets(right1, right2);
			force += getForceLeft(body, ship);
		}
		if (ship->moves[SHIP_STRAFE_RIGHT]) {
			turnOnJets(left1, left2);
			force += getForceRight(body, ship);
		}
		if (ship->moves[SHIP_THRUST_FORWARD]) {
			force += getForceForward(body, ship);
		}
		if (ship->moves[SHIP_STRAFE_BACKWARD]) {
			force += getForceBackward(body, ship);
		}

		f32 pitchSensitivity = 1.f;
		f32 yawSensitivity = 1.f;
		if (ship->curPitch > 0 || ship->curPitch < 0) pitchSensitivity = ship->curPitch * .5f;
		if (ship->curYaw > 0 || ship->curYaw < 0) pitchSensitivity = ship->curYaw * .5f;

		if (ship->moves[SHIP_PITCH_UP]) {
			turnOnJets(down2, up1);
			torque += getTorquePitchUp(body, ship) * pitchSensitivity;
		}
		if (ship->moves[SHIP_PITCH_DOWN]) {
			turnOnJets(down1, up2);
			torque += getTorquePitchDown(body, ship) * pitchSensitivity;
		}
		if (ship->moves[SHIP_YAW_LEFT]) {
			turnOnJets(right2, left1);
			torque += getTorqueYawLeft(body, ship) * yawSensitivity;
		}
		if (ship->moves[SHIP_YAW_RIGHT]) {
			turnOnJets(right1, left2);
			torque += getTorqueYawRight(body, ship) * yawSensitivity;
		}
		if (ship->moves[SHIP_ROLL_LEFT]) {
			torque += getTorqueRollLeft(body, ship);
		}
		if (ship->moves[SHIP_ROLL_RIGHT]) {
			torque += getTorqueRollRight(body, ship);
		}
		if (ship->moves[SHIP_STOP_ROTATION]) {
			torque += getTorqueToStopAngularVelocity(body, ship);
		}
		if (ship->moves[SHIP_STOP_VELOCITY]) {
			force += getForceToStopLinearVelocity(body, ship);
		}

		for (u32 i = 0; i < SHIP_MAX_MOVEMENTS; ++i) {
			ship->moves[i] = false;
		}
		ship->curPitch = 0.f;
		ship->curYaw = 0.f;

		rbc->rigidBody.applyTorqueImpulse(torque * dt);
		rbc->rigidBody.applyCentralImpulse(force * dt);
	}
}