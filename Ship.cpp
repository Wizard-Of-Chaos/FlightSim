#include "Ship.h"
#include "Controller.h"
#include <iostream>
#include <cmath>

Ship::Ship(IAnimatedMesh* nship, IAnimatedMeshSceneNode* nnode, f32 mass, f32 inertia, Controller* cont)
{
	ship = nship;
	node = nnode;
	controller = cont;
	velocity = vector3df(0, 0, 0);
	force = vector3df(0, 0, 0);
	torque = vector3df(0, 0, 0);
	rotVelocity = vector3df(0, 0, 0);
	rigidBodyComponent = {
		vector3df(),
		quaternion(node->getRotation() * DEGTORAD),
		node->getPosition(),
		vector3df(),
		quaternion(),
		vector3df(),
		vector3df(),
		mass,
		1/mass,
		inertia,
		1/inertia
	};

	hardpoints[0] = vector3df(2.4816f, .25f, -6.0088f);
	hardpoints[1] = vector3df(-2.4816f, .25f, -6.0088f);
	IMesh* wepMesh = controller->smgr->getMesh("models/wazer/wazer.obj");
	IMeshSceneNode* wepNode1 = controller->smgr->addMeshSceneNode(wepMesh, node, -1, hardpoints[0], vector3df(0, 0, 0), vector3df(.5f, .5f, .5f));
	IMeshSceneNode* wepNode2 = controller->smgr->addMeshSceneNode(wepMesh, node, -1, hardpoints[1], vector3df(0, 0, 0), vector3df(.5f, .5f, .5f));
	weapons[0] = new Weapon(wepNode1, wepMesh, controller);
	weapons[1] = new Weapon(wepNode2, wepMesh, controller);
}

Ship::Ship()
{
	ship = 0;
	node = 0;
	velocity = vector3df(0, 0, 0);
	force = vector3df(0, 0, 0);
	torque = vector3df(0, 0, 0);
	rotVelocity = vector3df(0, 0, 0);
	rigidBodyComponent = {
		vector3df(),
		quaternion(0,1,0,0),
		vector3df(),
		vector3df(),
		quaternion(),
		vector3df(),
		vector3df()
	};
}

//returns a normalized vector for the forward position
vector3df Ship::getForward()
{
	if (node) {
		vector3df rotation = node->getRotation();
		vector3df forward = vector3df(0, 0, 1); //I think our model might be backwards lmao
		return rotation.rotationToDirection(forward);
	}
	return vector3df(0, 0, 0);
}

vector3df Ship::getBackward()
{
	return -getForward();
}

vector3df Ship::getLeft()
{
	if (node) {
		vector3df rotation = node->getRotation();
		vector3df left = vector3df(-1, 0, 0);
		return rotation.rotationToDirection(left);
	}
	return vector3df(0, 0, 0);
}

vector3df Ship::getRight()
{
	return -getLeft();
}

vector3df Ship::getUp()
{
	if (node) {
		vector3df rotation = node->getRotation();
		vector3df up = vector3df(0, 1, 0);
		return rotation.rotationToDirection(up);
	}
	return vector3df(0, 0, 0);
}

vector3df Ship::getDown()
{
	return -getUp();

}
void Ship::update(f32 time)
{
	posUpdate(time);
}

void Ship::posUpdate(f32 time)
{
	//position
	vector3df impulse = force * time;
	Physics::applyImpulse(&rigidBodyComponent, impulse);

	node->setPosition(rigidBodyComponent.position);
	force = vector3df();
	
	vector3df angularImpulse = torque * time;
	Physics::applyAngularImpulse(&rigidBodyComponent, angularImpulse);
	vector3df eulerOrientation = vector3df();
	rigidBodyComponent.orientation.toEuler(eulerOrientation);
	eulerOrientation = eulerOrientation * RADTODEG;
	node->setRotation(eulerOrientation);
	torque = vector3df();

}

void Ship::turnToPos(vector3df pos)
{
	matrix4 localTransform = node->getAbsoluteTransformation(); //transforms global position to local position
	localTransform.makeInverse();
	vector3df localPos = pos;
	localTransform.transformVect(localPos);
	localPos.normalize();
	torque.X += localPos.Y;
	torque.Y -= localPos.X;
}

void Ship::fireWeapons(f32 time)
{
	for (int i = 0; i < 2; ++i) {
		weapons[i]->fire(time);
	}
}

void Ship::accelerateForward()
{
	force += (getForward() * maxSpeed);
}

void Ship::accelerateBackward()
{

	force += (getBackward() * maxSpeed);
}

void Ship::strafeLeft()
{
	force += (getLeft() * maxSpeed);
}

void Ship::strafeRight()
{
	force += (getRight() * maxSpeed);
}

void Ship::strafeUp()
{
	force += (getUp() * maxSpeed);
}

void Ship::strafeDown()
{
	force += (getDown() * maxSpeed);
}

//rotations
void Ship::yawLeft()
{
	torque.Y -= maxRotSpeed + rigidBodyComponent.angularVelocity.Y;
}

void Ship::yawRight()
{
	torque.Y +=  maxRotSpeed - rigidBodyComponent.angularVelocity.Y;
}

void Ship::pitchUp()
{
	torque.X -= maxRotSpeed + rigidBodyComponent.angularVelocity.X;
}

void Ship::pitchDown()
{
	torque.X += maxRotSpeed - rigidBodyComponent.angularVelocity.X;
}
void Ship::rollLeft()
{
	torque.Z += maxRotSpeed - rigidBodyComponent.angularVelocity.Z;
}
void Ship::rollRight()
{
	torque.Z -= maxRotSpeed + rigidBodyComponent.angularVelocity.Z;
}

void Ship::stopMoving()
{
	torque += (-rigidBodyComponent.angularVelocity) * maxRotSpeed;
	force += (-rigidBodyComponent.velocity * maxSpeed);
}