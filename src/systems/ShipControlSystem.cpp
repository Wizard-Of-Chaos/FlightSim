#include "ShipControlSystem.h"
#include <iostream>

void shipControlSystem(Scene& scene, f32 dt)
{
	for(auto entityId : SceneView<InputComponent, BulletRigidBodyComponent, ShipComponent>(scene)) {
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		btRigidBody* body = &rbc->rigidBody;
		InputComponent* input = scene.get<InputComponent>(entityId);
		ShipComponent* ship = scene.get<ShipComponent>(entityId);

		btVector3 torque(0, 0, 0);
		btVector3 force(0, 0, 0);

		//strafing
		if(input->isKeyDown(KEY_KEY_W)) {
			ship->moves[SHIP_THRUST_FORWARD] = true;
			force += getForceForward(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			ship->moves[SHIP_STRAFE_BACKWARD] = true;
			force += getForceBackward(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_A)) {
			ship->moves[SHIP_STRAFE_LEFT] = true;
			force += getForceLeft(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			ship->moves[SHIP_STRAFE_RIGHT] = true;
			force += getForceRight(body, ship);
		}
		if(input->isKeyDown(KEY_SPACE)) {
			ship->moves[SHIP_STRAFE_UP] = true;
			force += getForceUp(body, ship);
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			ship->moves[SHIP_STRAFE_DOWN] = true;
			force += getForceDown(body, ship);
		}

		//rotations
		if(input->isKeyDown(KEY_KEY_Z)) {
			ship->moves[SHIP_ROLL_LEFT] = true;
			torque += getTorqueRollLeft(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_C)) {
			ship->moves[SHIP_ROLL_RIGHT] = true;
			torque += getTorqueRollRight(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_R)) {
			ship->moves[SHIP_PITCH_UP] = true;
			torque += getTorquePitchUp(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			ship->moves[SHIP_PITCH_DOWN] = true;
			torque += getTorquePitchDown(body, ship);
		}
		if (input->isKeyDown(KEY_KEY_Q)) {
			ship->moves[SHIP_YAW_LEFT] = true;
			torque += getTorqueYawLeft(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			ship->moves[SHIP_YAW_RIGHT] = true;
			torque += getTorqueYawRight(body, ship);
		}

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			ship->moves[SHIP_STOP_ROTATION] = true;
			ship->moves[SHIP_STOP_VELOCITY] = true;
			torque += getTorqueToStopAngularVelocity(body, ship);
			force += getForceToStopLinearVelocity(body, ship);
		}

		if (input->mouseControlEnabled) {
			f32 mX = input->mousePosition.X;
			f32 mY = input->mousePosition.Y;

			if (mX > .2f || mX < -.2f) {
				if (mX > 0) {
					ship->moves[SHIP_YAW_RIGHT] = true;
				} else {
					ship->moves[SHIP_YAW_LEFT] = true;
				}
				torque += getTorqueYawRight(body, ship) * mX * .5f;
			}
			if (mY > .2f || mY < -.2f) {
				if (mY > 0) {
					ship->moves[SHIP_PITCH_DOWN] = true;
				} else {
					ship->moves[SHIP_PITCH_UP] = true;
				}
				torque += getTorquePitchDown(body, ship) * mY * .5f;
			}
			if ((mX < .2f && mX > -.2f) && (mY < .2f && mY > -.2f)) {
				torque += getTorqueToStopAngularVelocity(body, ship);
			}
		}

		rbc->rigidBody.applyTorqueImpulse(torque * dt);
		rbc->rigidBody.applyCentralImpulse(force * dt);
	}
}