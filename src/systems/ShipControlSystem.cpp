#include "ShipControlSystem.h"
#include <iostream>

void shipControlSystem(Scene& scene, f32 dt)
{
	for(auto entityId : SceneView<InputComponent, ShipComponent>(scene)) {
		InputComponent* input = scene.get<InputComponent>(entityId);
		ShipComponent* ship = scene.get<ShipComponent>(entityId);

		//strafing
		if(input->isKeyDown(KEY_KEY_W)) {
			ship->moves[SHIP_THRUST_FORWARD] = true;
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			ship->moves[SHIP_STRAFE_BACKWARD] = true;
		}
		if(input->isKeyDown(KEY_KEY_A)) {
			ship->moves[SHIP_STRAFE_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			ship->moves[SHIP_STRAFE_RIGHT] = true;
		}
		if(input->isKeyDown(KEY_SPACE)) {
			ship->moves[SHIP_STRAFE_UP] = true;
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			ship->moves[SHIP_STRAFE_DOWN] = true;
		}

		//rotations
		if(input->isKeyDown(KEY_KEY_Z)) {
			ship->moves[SHIP_ROLL_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_C)) {
			ship->moves[SHIP_ROLL_RIGHT] = true;
		}
		if(input->isKeyDown(KEY_KEY_R)) {
			ship->moves[SHIP_PITCH_UP] = true;
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			ship->moves[SHIP_PITCH_DOWN] = true;
		}
		if (input->isKeyDown(KEY_KEY_Q)) {
			ship->moves[SHIP_YAW_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			ship->moves[SHIP_YAW_RIGHT] = true;
		}

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			ship->moves[SHIP_STOP_ROTATION] = true;
			ship->moves[SHIP_STOP_VELOCITY] = true;
		}

		if (input->mouseControlEnabled) {
			f32 mX = input->mousePosition.X;
			f32 mY = input->mousePosition.Y;

			if (mX > .2f || mX < -.2f) {
				if (mX > 0.f) {
					ship->moves[SHIP_YAW_RIGHT] = true;
				} else {
					ship->moves[SHIP_YAW_LEFT] = true;
				}
				ship->curYaw = mX;
			}
			if (mY > .2f || mY < -.2f) {
				if (mY > 0.f) {
					ship->moves[SHIP_PITCH_DOWN] = true;
				} else {
					ship->moves[SHIP_PITCH_UP] = true;
				}
				ship->curPitch = mY;
			}
			if ((mX <= .2f && mX >= -.2f) && (mY <= .2f && mY >= -.2f)) {
				ship->moves[SHIP_STOP_ROTATION] = true;
			}
		}
	}
}