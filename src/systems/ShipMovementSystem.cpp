#include "ShipMovementSystem.h"

void shipMovementSystem(Scene& scene, f32 dt)
{
	for(auto entityId : SceneView<InputComponent, BulletRigidBodyComponent, ShipComponent>(scene)) {
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		InputComponent* input = scene.get<InputComponent>(entityId);
		ShipComponent* ship = scene.get<ShipComponent>(entityId);

		f32 speed = ship->speed;
		f32 rotSpeed = ship->rotSpeed;

		btVector3 torque(0, 0, 0);
		btVector3 force(0, 0, 0);

		btVector3 forward(0, 0, 1);
		btVector3 right(1, 0, 0);
		btVector3 up(0, 1, 0);
		btQuaternion transRot = rbc->rigidBody.getCenterOfMassTransform().getRotation();
		
		forward = forward.rotate(transRot.getAxis(), transRot.getAngle());
		right = right.rotate(transRot.getAxis(), transRot.getAngle());
		up = up.rotate(transRot.getAxis(), transRot.getAngle());

		//strafing
		if(input->isKeyDown(KEY_KEY_W)) {
			//force += irrcomp->getForward() * maxSpeed;
			force += forward * speed;
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			force -= forward * speed;
		}
		if(input->isKeyDown(KEY_KEY_A)) {
			force -= right * speed;
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			force += right * speed;
		}
		if(input->isKeyDown(KEY_SPACE)) {
			force += up * speed;
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			force -= up * speed;
		}

		//rotations
		if(input->isKeyDown(KEY_KEY_Q)) {
			torque += forward * rotSpeed;
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			torque -= forward * rotSpeed;
		}
		if(input->isKeyDown(KEY_KEY_R)) {
			torque -= right * rotSpeed;
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			torque += right * rotSpeed;
		}
		if(input->isKeyDown(KEY_KEY_Z)) {
			torque -= up * rotSpeed;
		}
		if(input->isKeyDown(KEY_KEY_C)) {
			torque += up * rotSpeed;
		}

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			btVector3 ang = rbc->rigidBody.getAngularVelocity();
			btVector3 lin = rbc->rigidBody.getLinearVelocity();
			ang.safeNormalize();
			lin.safeNormalize();
			torque -= ang * rotSpeed;
			force -= lin *speed;
		}

		if (input->mouseControlEnabled) {
			f32 mX = input->mousePosition.X;
			f32 mY = input->mousePosition.Y;

			if (mX > .2f || mX < -.2f) {
				torque += up * rotSpeed * mX;
			}
			if (mY > .2f || mY < -.2f) {
				torque += right * rotSpeed * mY;
			}
		}

		rbc->rigidBody.applyTorqueImpulse(torque * dt);
		rbc->rigidBody.applyCentralImpulse(force * dt);
	}
}