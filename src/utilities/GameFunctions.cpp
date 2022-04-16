#include "GameFunctions.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include "IrrlichtUtils.h"
#include <iostream>

//Sets the defaults in the scene manager for ship meshes.
void setDefaults()
{
	sceneManager->defaults.defaultShipMesh = smgr->getMesh("models/tux/Tuxedo.obj");
	sceneManager->defaults.defaultShipTexture = driver->getTexture("models/tux/tuxedotex.png");
	sceneManager->defaults.defaultWeaponMesh = smgr->getMesh("models/wazer/wazer.obj");
	sceneManager->defaults.defaultObstacleMesh = smgr->getMesh("models/asteroid/Asteroid.obj");
	sceneManager->defaults.defaultObstacleTexture = driver->getTexture("models/asteroid/Asteroid.jpg");
	if (!loadHull("attributes/hulls/Asteroid.bullet", sceneManager->defaults.defaultObstacleHull)) {
		sceneManager->defaults.defaultObstacleHull = createCollisionShapeFromMesh(sceneManager->defaults.defaultObstacleMesh);
		saveHull("attributes/hulls/Asteroid.bullet", sceneManager->defaults.defaultObstacleHull);
	}
	sceneManager->defaults.defaultMissileMesh = smgr->getMesh("models/basicmissile/Missile.obj");

	sceneManager->defaults.defaultProjectileTexture = driver->getTexture("effects/particlered.bmp");
	sceneManager->defaults.defaultCrosshairTexture = driver->getTexture("hud/crosshair.png");
	sceneManager->defaults.defaultSelectionTexture = driver->getTexture("hud/selection.png");
	sceneManager->defaults.defaultContactTexture = driver->getTexture("hud/contact.png");
	sceneManager->defaults.defaultContactMarkerTexture = driver->getTexture("hud/contactmarker.png");
	sceneManager->defaults.defaultHealthBarTexture = driver->getTexture("hud/hp.png");
	sceneManager->defaults.defaultVelocityBarTexture = driver->getTexture("hud/speed.png");
	sceneManager->defaults.defaultFuelBarTexture = driver->getTexture("hud/fuel.png");

	sceneManager->defaults.defaultJetTexture = driver->getTexture("effects/smokejet.png");
	sceneManager->defaults.defaultEngineJetTexture = driver->getTexture("effects/tuxengine.png");
	sceneManager->defaults.defaultExplosion = driver->getTexture("effects/kaboom.png");

	sceneManager->defaults.defaultJetSoundLoop = soundEngine->getSoundSource("audio/jetthrust.ogg");
	sceneManager->defaults.defaultMusic = soundEngine->getSoundSource("audio/music/space_boogaloo.ogg");
	sceneManager->defaults.defaultEngineSoundLoop = soundEngine->getSoundSource("audio/engineloop.ogg");
	sceneManager->defaults.defaultLaserSound = soundEngine->getSoundSource("audio/laser_shoot.ogg");
	sceneManager->defaults.bonk = soundEngine->getSoundSource("audio/impulsecannon.ogg");
	sceneManager->defaults.bonk->setDefaultVolume(10.f);
	sceneManager->defaults.bonk->setDefaultMinDistance(200.f);
	sceneManager->defaults.defaultLaserSound->setDefaultVolume(1.f);
	sceneManager->defaults.defaultLaserSound->setDefaultMinDistance(2.f);

	sceneManager->defaults.defaultGunSound = soundEngine->getSoundSource("audio/pew.ogg");
	sceneManager->defaults.defaultGunSound->setDefaultVolume(1.f);
	sceneManager->defaults.defaultGunSound->setDefaultMinDistance(3.f);

	sceneManager->defaults.defaultCloudTexture = driver->getTexture("effects/cloud.png");

	sceneManager->defaults.defaultHUDFont = guienv->getFont("fonts/AgencyFB14px/AgencyFB14px.xml");
}


//Convenience functions to swap back and forth between irrlicht and bullet vectors.
vector3df btVecToIrr(btVector3 vec)
{
	return vector3df(vec.x(), vec.y(), vec.z());
}

btVector3 irrVecToBt(vector3df vec)
{
	return btVector3(vec.X, vec.Y, vec.Z);
}

//Convenience functions that create a (sorta) random vector in Irrlicht space.
vector3df randomVector()
{
	f32 x = static_cast<f32>(rand() / static_cast <f32>(900));
	f32 y = static_cast<f32>(rand() / static_cast <f32>(900));
	f32 z = static_cast<f32>(rand() / static_cast <f32>(900));

	return vector3df(x, y, z);
}

vector3df randomRotationVector()
{
	f32 x = static_cast<f32>(rand() / static_cast <f32>(360)) - 180.f;
	f32 y = static_cast<f32>(rand() / static_cast <f32>(360)) - 180.f;
	f32 z = static_cast<f32>(rand() / static_cast <f32>(360)) - 180.f;

	return vector3df(x, y, z);
}

vector3df getPointInSphere(vector3df center, f32 radius)
{
	f32 dist = 1.5f; // init to greater than 1
	f32 x, y, z;
	while (dist > 1.f) {
		f32 low = -1.f;
		f32 high = 1.f;
		x = low + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (high - low)));
		y = low + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (high - low)));
		z = low + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (high - low)));
		dist = (x * x) + (y* y) + (z* z);
	}
	vector3df pos(x * radius, y * radius, z * radius);
	return center + pos;
}

bool isPointInSphere(vector3df& point, vector3df center, f32 radius)
{
	return (pow(point.X - center.X, 2.f) + pow(point.Y - center.Y, 2.f) + pow(point.Z - center.Z, 2.f) < pow(radius, 2));
}

void destroyObject(EntityId id)
{
	if (!sceneManager->scene.entityInUse(id)) return;
	auto irrComp = sceneManager->scene.get<IrrlichtComponent>(id);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);

	auto ship = sceneManager->scene.get<ShipComponent>(id);
	if (ship) {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			destroyObject(ship->weapons[i]);
		}
	}

	if (irrComp) {
		irrComp->node->removeAll();
		irrComp->node->remove();
	}
	if (rbc) {
		bWorld->removeRigidBody(&rbc->rigidBody);
	}
	auto ghost = sceneManager->scene.get<BulletGhostComponent>(id);
	if (ghost) {
		bWorld->removeCollisionObject(&ghost->ghost);
	}

	sceneManager->scene.destroyEntity(id);
}

bool initializeDefaultPlayer(EntityId shipId)
{
	Scene* scene = &sceneManager->scene;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	if (!shipIrr) return false;
	ISceneNode* target = smgr->addEmptySceneNode(0);
	target->setPosition(shipIrr->node->getPosition());
	ICameraSceneNode* camera = smgr->addCameraSceneNode(target, vector3df(0, 5, -20), shipIrr->node->getPosition(), ID_IsNotSelectable, true);
	scene->assign<InputComponent>(shipId);
	auto player = scene->assign<PlayerComponent>(shipId);
	player->camera = camera;
	player->target = target;

	player->thrust = vector3df(0, 0, 0);
	player->rotation = vector3df(0, 0, 0);

	return true;
}

void initializeHealth(EntityId id, f32 healthpool)
{
	auto hp = sceneManager->scene.assign<HealthComponent>(id);
	auto dmg = sceneManager->scene.assign<DamageTrackingComponent>(id);
	hp->health = healthpool;
	hp->maxHealth = healthpool;
}
void initializeDefaultHealth(EntityId objectId)
{
	initializeHealth(objectId, DEFAULT_MAX_HEALTH);
}

bool initializeDefaultHUD(EntityId playerId)
{
	Scene* scene = &sceneManager->scene;
	auto player = scene->get<PlayerComponent>(playerId);
	if (!player) return false;

	dimension2du baseSize = dimension2du(960, 540);
	player->rootHUD = guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	//HUDElement* crossHUD = new HUDCrosshair(player->rootHUD);
	HUDElement* selectHUD = new HUDActiveSelection(player->rootHUD);
	HUDElement* healthHUD = new HUDHealthBar(player->rootHUD);
	HUDElement* speedHUD = new HUDVelocityBar(player->rootHUD);
	//player->HUD.push_back(crossHUD);
	player->HUD.push_back(selectHUD);
	player->HUD.push_back(healthHUD);
	player->HUD.push_back(speedHUD);
	return true;
}

EntityId explode(vector3df position, f32 duration, f32 scale, f32 radius, f32 damage, f32 force)
{
	EntityId id = sceneManager->scene.newEntity();
	auto exp = sceneManager->scene.assign<ExplosionComponent>(id);
	exp->duration = duration;
	exp->lifetime = 0;
	exp->explosion = smgr->addParticleSystemSceneNode(true, 0, ID_IsNotSelectable, position);
	f32 scalefac = scale;
	auto em = exp->explosion->createPointEmitter(vector3df(0.04f * scalefac, 0.f, 0.f), 200, 500, SColor(0, 255, 255, 255), SColor(0, 255, 255, 255),
		50, 1000, 360, dimension2df(1.f, 1.f), dimension2df(5.f * scalefac, 5.f * scalefac));

	exp->explosion->setEmitter(em);
	em->drop();
	IParticleAffector* paf = exp->explosion->createFadeOutParticleAffector(SColor(0,0,0,0),100);
	exp->explosion->addAffector(paf);
	paf->drop();
	exp->explosion->setMaterialFlag(EMF_LIGHTING, false);
	exp->explosion->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	exp->explosion->setMaterialTexture(0, sceneManager->defaults.defaultExplosion);
	exp->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	exp->light = smgr->addLightSceneNode(0, position, SColorf(1.f, .9f, .1f));

	exp->force = force;
	exp->damage = damage;
	exp->radius = radius;

	return id;
}