#include "ObstacleUtils.h"
#include "SceneManager.h"
#include "GameController.h"
#include "BulletGhostComponent.h"
#include "GameStateController.h"

EntityId createDynamicObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	auto obstacle = sceneManager->scene.newEntity();

	loadObstacle(id, obstacle);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(obstacle);
	irr->node->setID(ID_IsSelectable);
	irr->node->setPosition(position);
	irr->node->setScale(scale);
	irr->node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	irr->node->setRotation(rotation);
	IMeshSceneNode* n = (IMeshSceneNode*)irr->node;
	n->getMesh()->setHardwareMappingHint(EHM_STATIC);
	btVector3 btscale(irrVecToBt(scale));
	initializeBtRigidBody(obstacle, stateController->assets.getHullAsset(stateController->obstacleData[id]->name), btscale, mass);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(obstacle);
	rbc->rigidBody.setActivationState(0);
	initializeHealth(obstacle, stateController->obstacleData[id]->health);
	return obstacle;

}
EntityId createStaticObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale)
{
	return createDynamicObstacle(id, position, rotation, scale, 0);
}

EntityId createAsteroid(vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	EntityId id = createDynamicObstacle(0, position, rotation, scale, mass);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
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

EntityId createDebris(vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	return createDynamicObstacle(2, position, rotation, scale, mass);
}

EntityId createGasCloud(vector3df position, vector3df scale)
{
	Scene* scene = &sceneManager->scene;

	auto cloud = scene->newEntity();
	if (!loadObstacle(1, cloud)) return INVALID_ENTITY;
	auto irr = scene->get<IrrlichtComponent>(cloud);
	irr->node->setID(ID_IsSelectable);
	irr->node->setPosition(position);
	irr->node->setScale(scale);

	IParticleSystemSceneNode* ps = (IParticleSystemSceneNode*)irr->node;
	auto em = ps->createSphereEmitter(vector3df(0,0,0), scale.X/4, vector3df(0, .00005f, 0),
		20, 100, SColor(10, 100, 100, 100), SColor(50, 100, 100, 100), 1000, 2000, 360,
		dimension2df(scale.X / 5, scale.X / 5), dimension2df(scale.X * 10, scale.X * 10));
	ps->setEmitter(em);
	em->drop();

	auto ghost = scene->assign<BulletGhostComponent>(cloud);
	ghost->shape = btSphereShape(scale.X / 4);
	ghost->ghost = btGhostObject();
	btTransform transform;
	transform.setOrigin(irrVecToBt(position));
	ghost->ghost.setWorldTransform(transform);
	ghost->ghost.setCollisionShape(&ghost->shape);
	bWorld->addCollisionObject(&ghost->ghost);
	ghost->ghost.setUserIndex(getEntityIndex(cloud));
	ghost->ghost.setUserIndex2(getEntityVersion(cloud));
	ghost->ghost.setUserIndex3(1);

	gameController->registerDeathCallback(cloud, gasDeathExplosion);
	return cloud;
}

void gasDeathExplosion(EntityId id)
{
	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
	vector3df pos = irr->node->getAbsolutePosition();
	vector3df scale = irr->node->getScale();
	f32 avgscale = (scale.X + scale.Y + scale.Z);
	f32 rad = irr->node->getBoundingBox().getExtent().getLength() * avgscale;
	explode(pos, 3.f, avgscale, rad, 80.f, 1200.f);
}