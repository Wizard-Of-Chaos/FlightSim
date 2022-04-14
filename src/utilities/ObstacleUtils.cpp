#include "ObstacleUtils.h"
#include "SceneManager.h"
#include "GameController.h"

EntityId createGasCloud(SceneManager* manager, vector3df position, vector3df scale)
{
	ISceneManager* smgr = manager->controller->smgr;
	Scene* scene = &manager->scene;

	auto cloud = scene->newEntity();
	
	auto obst = scene->assign<ObstacleComponent>(cloud);
	obst->type = OBSTACLE::GAS_CLOUD;

	auto irr = scene->assign<IrrlichtComponent>(cloud);
	irr->name = "Gas Cloud";

	auto ps = smgr->addParticleSystemSceneNode(true, 0, ID_IsSelectable, position, vector3df(0, 0, 0), scale);
	irr->node = ps;
	auto em = ps->createSphereEmitter(position, scale.X/2, vector3df(0, .00005f, 0),
		5, 20, SColor(10, 100, 100, 100), SColor(50, 100, 100, 100), 6000, 20000, 360,
		dimension2df(scale.X / 5, scale.X / 5), dimension2df(scale.X * 10, scale.X * 10));
	ps->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, manager->defaults.defaultCloudTexture);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	initializeDefaultHealth(manager, cloud);

	return cloud;
}