#include "ObstacleUtils.h"
#include "GameController.h"
#include "BulletGhostComponent.h"
#include "GameStateController.h"

flecs::entity createDynamicObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	auto obstacle = game_world->entity();

	loadObstacle(id, obstacle);
	auto irr = obstacle.get_mut<IrrlichtComponent>();
	irr->node->setID(ID_IsSelectable);
	irr->node->setPosition(position);
	irr->node->setScale(scale);
	irr->node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	irr->node->setRotation(rotation);
	IMeshSceneNode* n = (IMeshSceneNode*)irr->node;
	n->getMesh()->setHardwareMappingHint(EHM_STATIC);
	btVector3 btscale(irrVecToBt(scale));
	initializeBtRigidBody(obstacle, stateController->assets.getHullAsset(stateController->obstacleData[id]->name), btscale, mass);
	auto rbc = obstacle.get_mut<BulletRigidBodyComponent>();
	rbc->rigidBody.setActivationState(0);
	initializeHealth(obstacle, stateController->obstacleData[id]->health);
	return obstacle;

}
flecs::entity createStaticObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale)
{
	return createDynamicObstacle(id, position, rotation, scale, 0);
}

flecs::entity createAsteroid(vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	flecs::entity id = createDynamicObstacle(0, position, rotation, scale, mass);
	auto rbc = id.get_mut<BulletRigidBodyComponent>();
	u32 roll = std::rand() % 20;

	if (roll < 15) {
		rbc->rigidBody.setActivationState(1);
		rbc->rigidBody.applyTorque(irrVecToBt(randomRotationVector()));
	}
	if (roll == 20) { //Critical hit -- the asteroid is moving!
		rbc->rigidBody.applyCentralImpulse(irrVecToBt(randomVector()));
	}
	return id;
}

flecs::entity createDebris(vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	return createDynamicObstacle(2, position, rotation, scale, mass);
}

flecs::entity createGasCloud(vector3df position, vector3df scale)
{
	auto cloud = game_world->entity();
	if (!loadObstacle(1, cloud)) return INVALID_ENTITY;
	auto irr = cloud.get_mut<IrrlichtComponent>();
	irr->node->setID(ID_IsSelectable);
	irr->node->setPosition(position);
	irr->node->setScale(scale);

	IParticleSystemSceneNode* ps = (IParticleSystemSceneNode*)irr->node;
	auto em = ps->createSphereEmitter(vector3df(0,0,0), scale.X/4, vector3df(0, .00005f, 0),
		20, 100, SColor(10, 100, 100, 100), SColor(50, 100, 100, 100), 1000, 2000, 360,
		dimension2df(scale.X / 5, scale.X / 5), dimension2df(scale.X * 10, scale.X * 10));
	ps->setEmitter(em);
	em->drop();

	auto ghost = cloud.get_mut<BulletGhostComponent>();
	ghost->shape = btSphereShape(scale.X / 4);
	ghost->ghost = btGhostObject();
	btTransform transform;
	transform.setOrigin(irrVecToBt(position));
	ghost->ghost.setWorldTransform(transform);
	ghost->ghost.setCollisionShape(&ghost->shape);
	bWorld->addCollisionObject(&ghost->ghost);

	setIdOnBtObject(&ghost->ghost, cloud);

	gameController->registerDeathCallback(cloud, gasDeathExplosion);
	return cloud;
}

void gasDeathExplosion(flecs::entity id)
{
	auto irr = id.get<IrrlichtComponent>();
	vector3df pos = irr->node->getAbsolutePosition();
	vector3df scale = irr->node->getScale();
	f32 avgscale = (scale.X + scale.Y + scale.Z);
	f32 rad = irr->node->getBoundingBox().getExtent().getLength() * avgscale;
	explode(pos, 3.f, avgscale, rad, 80.f, 1200.f);
}