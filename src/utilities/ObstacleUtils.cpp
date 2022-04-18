#include "ObstacleUtils.h"
#include "SceneManager.h"
#include "GameController.h"
#include "BulletGhostComponent.h"
#include "GameStateController.h"


EntityId createAsteroid(vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	Scene* scene = &sceneManager->scene;

	auto roidEntity = scene->newEntity();

	auto obst = scene->assign<ObstacleComponent>(roidEntity);
	obst->type = OBSTACLE::ASTEROID;

	auto irrComp = scene->assign<IrrlichtComponent>(roidEntity);
	irrComp->node = smgr->addMeshSceneNode(stateController->assets.getMeshAsset("asteroidMesh"));
	irrComp->node->setMaterialTexture(0, stateController->assets.getTextureAsset("asteroidTexture"));
	irrComp->node->setID(ID_IsSelectable | ID_IsAvoidable);
	irrComp->node->setPosition(position);
	irrComp->node->setName(idToStr(roidEntity).c_str());
	irrComp->node->setScale(scale);
	irrComp->name = "Asteroid";
	irrComp->node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	irrComp->node->setRotation(rotation);

	IMeshSceneNode* n = (IMeshSceneNode*)irrComp->node;
	n->getMesh()->setHardwareMappingHint(EHM_STATIC);
	//manager->controller->driver->addOcclusionQuery(irrComp->node, n->getMesh());

	btVector3 btscale(irrVecToBt(scale));
	initializeBtRigidBody(roidEntity, stateController->assets.getHullAsset("asteroidHull"), btscale, mass);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(roidEntity);
	rbc->rigidBody.setActivationState(0);

	initializeDefaultHealth(roidEntity);

	return roidEntity;
}

EntityId createGasCloud(vector3df position, vector3df scale)
{
	Scene* scene = &sceneManager->scene;

	auto cloud = scene->newEntity();
	
	auto obst = scene->assign<ObstacleComponent>(cloud);
	obst->type = OBSTACLE::GAS_CLOUD;

	auto irr = scene->assign<IrrlichtComponent>(cloud);
	irr->name = "Gas Cloud";

	auto ps = smgr->addParticleSystemSceneNode(true, 0, ID_IsSelectable, position, vector3df(0, 0, 0), scale);
	irr->node = ps;
	irr->node->setName(idToStr(cloud).c_str());
	auto em = ps->createSphereEmitter(vector3df(0,0,0), scale.X/4, vector3df(0, .00005f, 0),
		20, 100, SColor(10, 100, 100, 100), SColor(50, 100, 100, 100), 1000, 2000, 360,
		dimension2df(scale.X / 5, scale.X / 5), dimension2df(scale.X * 10, scale.X * 10));
	ps->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, stateController->assets.getTextureAsset("defaultCloud"));
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

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

	initializeHealth(cloud, 10);
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