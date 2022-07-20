#include "ThrustSystem.h"
#include "ShipMovementUtils.h"

void thrustSystem(flecs::iter it, ThrustComponent* thrc, HealthComponent* hpc, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc)
{
	for (auto i : it) {
		flecs::entity entityId = it.entity(i);
		if (!entityId.is_alive()) return;

		auto thrust = &thrc[i];
		auto hp = &hpc[i];
		auto rbc = &rbcs[i];
		auto irr = &irrc[i];

		f32 dt = it.delta_time();
		btRigidBody* body = rbc->rigidBody;

		btVector3 torque(0, 0, 0);
		btVector3 force(0, 0, 0);

		if (thrust->moves[THRUST_FORWARD]) {
			force += getForceForward(body, thrust);
		}
		if (thrust->moves[THRUST_BACKWARD]) {
			force += getForceBackward(body, thrust);
		}
		if (thrust->moves[THRUST_BOOST]) {
			force += getForceBoost(body, thrust);
		}
		if (thrust->moves[STRAFE_LEFT]) {
			force += getForceLeft(body, thrust);
		}
		if (thrust->moves[STRAFE_RIGHT]) {
			force += getForceRight(body, thrust);
		}
		if (thrust->moves[STRAFE_UP]) {
			force += getForceUp(body, thrust);
		}
		if (thrust->moves[STRAFE_DOWN]) {
			force += getForceDown(body, thrust);
		}

		if (thrust->moves[PITCH_UP]) {
			torque += getTorquePitchUp(body, thrust);
		}
		if (thrust->moves[PITCH_DOWN]) {
			torque += getTorquePitchDown(body, thrust);
		}
		if (thrust->moves[YAW_LEFT]) {
			torque += getTorqueYawLeft(body, thrust);
		}
		if (thrust->moves[YAW_RIGHT]) {
			torque += getTorqueYawRight(body, thrust);
		}
		if (thrust->moves[ROLL_LEFT]) {
			torque += getTorqueRollLeft(body, thrust);
		}
		if (thrust->moves[ROLL_RIGHT]) {
			torque += getTorqueRollRight(body, thrust);
		}

		if (thrust->moves[STOP_ROTATION]) {
			torque += getTorqueToStopAngularVelocity(body, thrust);
		}
		if (thrust->moves[STOP_VELOCITY]) {
			force += getForceToStopLinearVelocity(body, thrust);
		}

		for (u32 i = 0; i < MAX_MOVEMENTS; ++i) {
			thrust->moves[i] = false;
		}
		rbc->rigidBody->applyTorqueImpulse(torque * dt);
		rbc->rigidBody->applyCentralImpulse(force * dt);

		//over-speed damage here
		f32 linVel = body->getLinearVelocity().length();
		f32 angVel = body->getAngularVelocity().length();

		if (linVel > thrust->linearMaxVelocity + 1.f) {
			f32 over = linVel - thrust->linearMaxVelocity;
			hp->health -= over * thrust->velocityTolerance * dt;
		}
		if (angVel > thrust->angularMaxVelocity + .2f) {
			f32 over = angVel - thrust->angularMaxVelocity;
			hp->health -= over * thrust->velocityTolerance * dt;
		}
	}
}