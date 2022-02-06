#include "ShipMovementSystem.h"
#include <iostream>

void shipMovementSystem(Scene& scene, f32 dt)
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
			force += getForceForward(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			force += getForceBackward(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_A)) {
			force += getForceLeft(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			force += getForceRight(body, ship);
		}
		if(input->isKeyDown(KEY_SPACE)) {
			force += getForceUp(body, ship);
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			force += getForceDown(body, ship);
		}

		//rotations
		if(input->isKeyDown(KEY_KEY_Z)) {
			torque += getTorqueRollLeft(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_C)) {
			torque += getTorqueRollRight(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_R)) {
			torque += getTorquePitchUp(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			torque += getTorquePitchDown(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_Q)) {
			torque += getTorqueYawLeft(body, ship);
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			torque += getTorqueYawRight(body, ship);
		}

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			torque += getTorqueToStopAngularVelocity(body, ship);
			force += getForceToStopLinearVelocity(body, ship);
		}

		if (input->mouseControlEnabled) {
			f32 mX = input->mousePosition.X;
			f32 mY = input->mousePosition.Y;

			if (mX > .2f || mX < -.2f) {
				torque += getTorqueYawRight(body, ship) * mX;
			}
			if (mY > .2f || mY < -.2f) {
				torque += getTorquePitchUp(body, ship) * mY;
			}
		}

		rbc->rigidBody.applyTorqueImpulse(torque * dt);
		rbc->rigidBody.applyCentralImpulse(force * dt);
	}
}