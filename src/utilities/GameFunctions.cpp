#include "GameFunctions.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include "IrrlichtUtils.h"
#include <iostream>

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
	f32 x = static_cast<f32>(rand() / static_cast <f32>(180));
	f32 y = static_cast<f32>(rand() / static_cast <f32>(180));
	f32 z = static_cast<f32>(rand() / static_cast <f32>(180));

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

bool isPointInSphere(vector3df& point, vector3df& center, f32& radius)
{
	return (pow(point.X - center.X, 2.f) + pow(point.Y - center.Y, 2.f) + pow(point.Z - center.Z, 2.f) < pow(radius, 2));
}

//Sets the defaults in the scene manager for ship meshes.
void setDefaults(SceneManager* manager)
{
	ISceneManager* smgr = manager->controller->smgr;
	IVideoDriver* driver = manager->controller->driver;
	ISoundEngine* sndeng = manager->controller->soundEngine;

	manager->defaults.defaultShipMesh = smgr->getMesh("models/tux/Tux.obj");
	manager->defaults.defaultShipTexture = driver->getTexture("models/tux/BulletShipTex.png");
	manager->defaults.defaultWeaponMesh = smgr->getMesh("models/wazer/wazer.obj");
	manager->defaults.defaultObstacleMesh = smgr->getMesh("models/asteroid/Asteroid.obj");
	manager->defaults.defaultObstacleTexture = driver->getTexture("models/asteroid/Asteroid.jpg");
	if (!loadHull("attributes/hulls/Asteroid.bullet", manager->defaults.defaultObstacleHull)) {
		manager->defaults.defaultObstacleHull = createCollisionShapeFromMesh(manager->defaults.defaultObstacleMesh);
		saveHull("attributes/hulls/Asteroid.bullet", manager->defaults.defaultObstacleHull);
	}
	manager->defaults.defaultMissileMesh = smgr->getMesh("models/basicmissile/Missile.obj");

	manager->defaults.defaultProjectileTexture = driver->getTexture("effects/particlered.bmp");
	manager->defaults.defaultCrosshairTexture = driver->getTexture("hud/crosshair.png");
	manager->defaults.defaultSelectionTexture = driver->getTexture("hud/selection.png");
	manager->defaults.defaultContactTexture = driver->getTexture("hud/contact.png");
	manager->defaults.defaultContactMarkerTexture = driver->getTexture("hud/contactmarker.png");
	manager->defaults.defaultHealthBarTexture = driver->getTexture("hud/hp.png");
	manager->defaults.defaultVelocityBarTexture = driver->getTexture("hud/speed.png");
	manager->defaults.defaultFuelBarTexture = driver->getTexture("hud/fuel.png");

	manager->defaults.defaultJetTexture = driver->getTexture("effects/smokejet.png");
	manager->defaults.defaultEngineJetTexture = driver->getTexture("effects/tuxengine.png");
	manager->defaults.defaultExplosion = driver->getTexture("effects/kaboom.png");

	manager->defaults.defaultJetSoundLoop = sndeng->getSoundSource("audio/jetthrust.ogg");
	manager->defaults.defaultMusic = sndeng->getSoundSource("audio/music/space_boogaloo.ogg");
	manager->defaults.defaultEngineSoundLoop = sndeng->getSoundSource("audio/engineloop.ogg");
	manager->defaults.defaultLaserSound = sndeng->getSoundSource("audio/laser_shoot.ogg");

	manager->defaults.defaultHUDFont = manager->controller->guienv->getFont("fonts/AgencyFB14px/AgencyFB14px.xml");
}

void initializeNeutralFaction(SceneManager* manager, EntityId id)
{
	Scene* scene = &manager->scene;
	auto fac = scene->assign<FactionComponent>(id);
	fac->type = FACTION_NEUTRAL;
	fac->hostileTo = 0;
	fac->friendlyTo = 0;
}
void initializeHostileFaction(SceneManager* manager, EntityId id)
{
	Scene* scene = &manager->scene;
	auto fac = scene->assign<FactionComponent>(id);
	fac->type = FACTION_HOSTILE;
	fac->hostileTo = FACTION_PLAYER;
	fac->friendlyTo = FACTION_HOSTILE;
}

void initializePlayerFaction(SceneManager* manager, EntityId id)
{
	Scene* scene = &manager->scene;
	auto fac = scene->assign<FactionComponent>(id);
	fac->type = FACTION_PLAYER;
	fac->hostileTo = FACTION_HOSTILE;
	fac->friendlyTo = FACTION_PLAYER;
}

void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies)
{
	fac->type = type;
	fac->hostileTo = hostilities;
	fac->friendlyTo = friendlies;
}

EntityId createShipFromId(u32 id, SceneManager* manager, vector3df position)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipEntity = scene->newEntity();

	loadShip(id, shipEntity, manager);
	auto ship = scene->get<ShipComponent>(shipEntity);
	auto irr = scene->get<IrrlichtComponent>(shipEntity);
	irr->node->setPosition(position);
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		ship->weapons[i] = INVALID_ENTITY;
	}

	return shipEntity;
}

EntityId createDefaultShip(SceneManager* manager, vector3df position)
{
	EntityId shipEntity = createShipFromId(0, manager, position);
	auto ship = manager->scene.get<ShipComponent>(shipEntity);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeDefaultWeapon(manager, shipEntity, i);
	}
	return shipEntity;
}

EntityId createDefaultObstacle(SceneManager* manager, vector3df position, vector3df rotation, vector3df scale, f32 mass)
{
	ISceneManager* smgr = manager->controller->smgr;
	Scene* scene = &manager->scene;

	auto roidEntity = scene->newEntity();

	auto obst = scene->assign<ObstacleComponent>(roidEntity);
	obst->type = OBSTACLE::ASTEROID;

	auto irrComp = scene->assign<IrrlichtComponent>(roidEntity);
	irrComp->node = smgr->addMeshSceneNode(manager->defaults.defaultObstacleMesh);
	irrComp->node->setMaterialTexture(0, manager->defaults.defaultObstacleTexture);
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
	initializeBtRigidBody(manager, roidEntity, manager->defaults.defaultObstacleHull, btscale, mass);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(roidEntity);
	rbc->rigidBody.setActivationState(0);

	initializeDefaultHealth(manager, roidEntity);

	return roidEntity;
}

void destroyObject(SceneManager* manager, EntityId id)
{
	auto irrComp = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);

	auto ship = manager->scene.get<ShipComponent>(id);
	if (ship) {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			destroyObject(manager, ship->weapons[i]);
		}
	}

	if (irrComp) {
		irrComp->node->removeAll();
		irrComp->node->remove();
	}
	if (rbc) {
		manager->bulletWorld->removeRigidBody(&rbc->rigidBody);
	}

	manager->scene.destroyEntity(id);
}

bool initializeWeaponFromId(u32 id, SceneManager* manager, EntityId shipId, int hardpoint)
{
	if (id <= 0) return false;

	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	auto shipComp = scene->get<ShipComponent>(shipId);

	if (!shipIrr || !shipComp) return false;
	
	auto wepEntity = scene->newEntity();
	loadWeapon(id, wepEntity, shipId, manager);
	auto irr = scene->get<IrrlichtComponent>(wepEntity);
	irr->node->setParent(shipIrr->node);
	irr->node->setPosition(shipComp->hardpoints[hardpoint]);
	irr->node->setScale(vector3df(.5f, .5f, .5f));

	shipComp->weapons[hardpoint] = wepEntity;

	auto parentCmp = scene->assign<ParentComponent>(wepEntity);
	parentCmp->parentId = shipId;

	return true;
}

bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint)
{
	return initializeWeaponFromId(1, manager, shipId, hardpoint);
}

bool initializeBtRigidBody(SceneManager* manager, EntityId entityId, btConvexHullShape shape, btVector3& scale, f32 mass)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto objIrr = scene->get<IrrlichtComponent>(entityId);

	if (!objIrr) return false;

	BulletRigidBodyComponent* rbc = scene->assign<BulletRigidBodyComponent>(entityId);
	rbc->shape = shape;
	rbc->shape.setLocalScaling(scale);
	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(irrVecToBt(objIrr->node->getPosition()));
	btQuaternion q;
	btVector3 rot = irrVecToBt(objIrr->node->getRotation());
	q.setEuler(rot.y(), rot.x(), rot.z());
	transform.setRotation(q);
	auto motionState = new btDefaultMotionState(transform);

	btVector3 localInertia;
	rbc->shape.calculateLocalInertia(mass, localInertia);
	rbc->rigidBody = btRigidBody(mass, motionState, &rbc->shape, localInertia);
	rbc->rigidBody.setSleepingThresholds(0, 0);

	rbc->rigidBody.setUserIndex(getEntityIndex(entityId));
	rbc->rigidBody.setUserIndex2(getEntityVersion(entityId));
	rbc->rigidBody.setUserIndex3(1);

	manager->bulletWorld->addRigidBody(&(rbc->rigidBody));

	return true;
}

bool initializeShipCollisionBody(SceneManager* manager, EntityId entityId, u32 shipId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;
	GameStateController* cont = manager->controller->stateController;

	auto shipComp = scene->get<ShipComponent>(entityId);

	if (!shipComp) return false;
	btVector3 scale(1.f, 1.f, 1.f);
	return initializeBtRigidBody(manager, entityId, cont->shipData[shipId]->collisionShape, scale, 1.f);
}

bool initializeDefaultPlayer(SceneManager* manager, EntityId shipId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

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

void initializeDefaultHealth(SceneManager* manager, EntityId objectId)
{
	Scene* scene = &manager->scene;

	auto healthComp = scene->assign<HealthComponent>(objectId);
	auto dmgtracker = scene->assign<DamageTrackingComponent>(objectId);
	healthComp->health = DEFAULT_MAX_HEALTH;
	healthComp->maxHealth = DEFAULT_MAX_HEALTH;
}

void initializeDefaultShields(SceneManager* manager, EntityId objectId)
{
	Scene* scene = &manager->scene;

	auto shields = scene->assign<ShieldComponent>(objectId);
	shields->shields = DEFAULT_MAX_SHIELDS;
	shields->maxShields = DEFAULT_MAX_SHIELDS;
	shields->rechargeDelay = DEFAULT_RECHARGE_DELAY;
	shields->rechargeRate = DEFAULT_RECHARGE_RATE;
	shields->timeSinceLastHit = shields->rechargeDelay;
}

bool initializeDefaultHUD(SceneManager* manager, EntityId playerId)
{
	Scene* scene = &manager->scene;
	auto player = scene->get<PlayerComponent>(playerId);
	if (!player) return false;

	dimension2du baseSize = dimension2du(960, 540);
	player->rootHUD = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	//HUDElement* crossHUD = new HUDCrosshair(manager, player->rootHUD);
	HUDElement* selectHUD = new HUDActiveSelection(manager, player->rootHUD);
	HUDElement* healthHUD = new HUDHealthBar(manager, player->rootHUD);
	HUDElement* speedHUD = new HUDVelocityBar(manager, player->rootHUD);
	//player->HUD.push_back(crossHUD);
	player->HUD.push_back(selectHUD);
	player->HUD.push_back(healthHUD);
	player->HUD.push_back(speedHUD);
	return true;
}

void initializeDefaultSensors(SceneManager* manager, EntityId id)
{
	Scene* scene = &manager->scene;
	auto rbc = scene->get<BulletRigidBodyComponent>(id);
	auto fac = scene->get<FactionComponent>(id);
	if (!rbc || !fac) return; //check, because sensors REQUIRE an irrlicht pos
	auto sensors = scene->assign<SensorComponent>(id);
	sensors->detectionRadius = DEFAULT_SENSOR_RANGE;

	sensors->closestContact = INVALID_ENTITY;
	sensors->closestFriendlyContact = INVALID_ENTITY;
	sensors->closestHostileContact = INVALID_ENTITY;
	sensors->targetContact = INVALID_ENTITY;
	sensors->timeSelected = 0;

	sensors->updateInterval = DEFAULT_SENSOR_UPDATE_INTERVAL;
	f32 start = 10 / (f32)(std::rand() % 100);
	sensors->timeSinceLastUpdate = start; //stagger sensor updates so it doesn't all happen at once
}

EntityId createDefaultAIShip(SceneManager* manager, vector3df position)
{
	Scene* scene = &manager->scene;

	EntityId id = createDefaultShip(manager, position);
	auto irr = scene->get<IrrlichtComponent>(id);
	irr->name = "AI Ship";
	initializeDefaultHealth(manager, id);
	initializeDefaultShields(manager, id);
	initializeShipCollisionBody(manager, id, 0);
	//initializeDefaultRigidBody(manager, id);

	auto ai = scene->assign<AIComponent>(id);
	ai->AIType = AI_TYPE_DEFAULT;
	ai->reactionSpeed = AI_DEFAULT_REACTION_TIME;
	ai->damageTolerance = AI_DEFAULT_DAMAGE_TOLERANCE;
	ai->timeSinceLastStateCheck = 0;

	return id;
}

IParticleSystemSceneNode* createShipJet(SceneManager* manager, ISceneNode* node, vector3df pos, vector3df dir)
{
	ISceneManager* smgr = manager->controller->smgr;

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(true, node);
	ps->setID(ID_IsNotSelectable);
	IParticleEmitter* em = ps->createSphereEmitter(
		pos, .1f, dir * .02f,
		0, 0, SColor(255, 180, 180, 180), SColor(255, 210, 210, 210),
		50, 200, 2, dimension2df(.2f, .2f), dimension2df(.35f, .35f)
	);
	ps->setEmitter(em);
	em->drop();

	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, manager->defaults.defaultJetTexture);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	return ps;
}

void initializeShipParticles(SceneManager* manager, EntityId id)
{
	auto ship = manager->scene.get<ShipComponent>(id);
	auto irr = manager->scene.get<IrrlichtComponent>(id);

	for (u32 i = 0; i < 2; ++i) {
		ship->upJetEmit[i] = createShipJet(manager, irr->node, ship->upJetPos[i], getNodeUp(irr->node));
		ship->downJetEmit[i] = createShipJet(manager, irr->node, ship->downJetPos[i], getNodeDown(irr->node));
		ship->leftJetEmit[i] = createShipJet(manager, irr->node, ship->leftJetPos[i], getNodeLeft(irr->node));
		ship->rightJetEmit[i] = createShipJet(manager, irr->node, ship->rightJetPos[i], getNodeRight(irr->node));
		ship->reverseJetEmit[i] = createShipJet(manager, irr->node, ship->reverseJetPos[i], getNodeForward(irr->node));
	}
	ship->engineJetEmit = manager->controller->smgr->addVolumeLightSceneNode(irr->node,
		ID_IsNotSelectable, 256, 256, SColor(255, 100, 250, 100), SColor(0, 0, 0, 0), ship->engineJetPos,
		vector3df(-90, 0, 0), vector3df(2, 1, 2));

	array<ITexture*> tex;
	IVideoDriver* driver = manager->controller->driver;
	for (s32 i = 4; i > 0; --i) {
		stringc str = "effects/engine/engine";
		str += i;
		str += ".png";
		ITexture* t = driver->getTexture(str.c_str());
		if(t) tex.push_back(t);
	}
	ISceneNodeAnimator* glowie = manager->controller->smgr->createTextureAnimator(tex, 50);
	ship->engineJetEmit->addAnimator(glowie);
	glowie->drop(); //Terry would be proud

	ship->engineJetEmit->setMaterialFlag(EMF_LIGHTING, false);
	ship->engineJetEmit->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ship->engineJetEmit->setMaterialTexture(0, manager->defaults.defaultEngineJetTexture);
	ship->engineJetEmit->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);


	auto engine = manager->controller->smgr->addLightSceneNode(irr->node, ship->engineJetPos, SColorf(0.f, 1.f, 0.f), 1.3f);
	ship->engineLight = engine;
	//auto bill = manager->controller->smgr->addBillboardSceneNode(engine, dimension2df(2.f, 2.f));
	engine->setID(ID_IsNotSelectable);
	/*
	bill->setID(ID_IsNotSelectable);
	bill->setMaterialTexture(0, manager->defaults.defaultEngineJetTexture);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialFlag(EMF_LIGHTING, false);
	*/

}

EntityId explode(SceneManager* manager, vector3df position, f32 duration, f32 scale, f32 radius, f32 damage, f32 force)
{
	EntityId id = manager->scene.newEntity();
	auto exp = manager->scene.assign<ExplosionComponent>(id);
	exp->duration = duration;
	exp->lifetime = 0;
	exp->explosion = manager->controller->smgr->addParticleSystemSceneNode(true, 0, ID_IsNotSelectable, position);
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
	exp->explosion->setMaterialTexture(0, manager->defaults.defaultExplosion);
	exp->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	exp->light = manager->controller->smgr->addLightSceneNode(0, position, SColorf(1.f, .9f, .1f));

	exp->force = force;
	exp->damage = damage;
	exp->radius = radius;

	return id;
}

EntityId createPlayerShipFromLoadout(SceneManager* manager, vector3df pos)
{
	GameStateController* stCtrl = manager->controller->stateController;
	EntityId shipEntity = createShipFromId(stCtrl->playerShip, manager, pos);
	auto ship = manager->scene.get<ShipComponent>(shipEntity);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeWeaponFromId(stCtrl->playerWeapons[i], manager, shipEntity, i);
	}

	initializeDefaultPlayer(manager, shipEntity);
	initializeShipCollisionBody(manager, shipEntity, stCtrl->playerShip);
	initializeNeutralFaction(manager, shipEntity);
	initializeDefaultHealth(manager, shipEntity);
	initializeDefaultShields(manager, shipEntity);
	initializeDefaultSensors(manager, shipEntity);
	initializeDefaultHUD(manager, shipEntity);
	initializeShipParticles(manager, shipEntity);

	return shipEntity;
}