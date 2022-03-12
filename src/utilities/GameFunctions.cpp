#include "GameFunctions.h"
#include "SceneManager.h"
#include "GameController.h"
#include "IrrlichtUtils.h"

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

EntityId createDefaultShip(SceneManager* manager, vector3df position)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipEntity = scene->newEntity();

	loadShip("attributes/ships/tux.gdat", shipEntity, manager);
	auto ship = scene->get<ShipComponent>(shipEntity);
	auto irr = scene->get<IrrlichtComponent>(shipEntity);
	irr->node->setPosition(position);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeDefaultWeapon(manager, shipEntity, i);
	}

	return shipEntity;
}

EntityId createDefaultObstacle(SceneManager* manager, vector3df position)
{
	ISceneManager* smgr = manager->controller->smgr;
	Scene* scene = &manager->scene;

	auto roidEntity = scene->newEntity();
	auto irrComp = scene->assign<IrrlichtComponent>(roidEntity);
	irrComp->node = smgr->addMeshSceneNode(manager->defaults.defaultObstacleMesh);
	irrComp->node->setID(ID_IsSelectable | ID_IsAvoidable);
	irrComp->node->setPosition(position);
	irrComp->node->setName(idToStr(roidEntity).c_str());
	irrComp->name = "Asteroid";

	auto healthComp = scene->assign<HealthComponent>(roidEntity);
	healthComp->health = DEFAULT_MAX_HEALTH;
	healthComp->maxHealth = DEFAULT_MAX_HEALTH;

	auto fac = scene->assign<FactionComponent>(roidEntity);
	fac->type = FACTION_NEUTRAL;

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

bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	auto shipComp = scene->get<ShipComponent>(shipId);

	if (!shipIrr || !shipComp) return false;
	
	auto wepEntity = scene->newEntity();
	loadWeapon("attributes/weapons/plasmablaster.gdat", wepEntity, shipId, manager);
	auto irr = scene->get<IrrlichtComponent>(wepEntity);
	irr->node->setParent(shipIrr->node);
	irr->node->setPosition(shipComp->hardpoints[hardpoint]);
	irr->node->setScale(vector3df(.5f, .5f, .5f));

	shipComp->weapons[hardpoint] = wepEntity;

	return true;
}

bool initializeDefaultRigidBody(SceneManager* manager, EntityId objectId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto objIrr = scene->get<IrrlichtComponent>(objectId);
	
	if (!objIrr) return false;

	BulletRigidBodyComponent* rbc = scene->assign<BulletRigidBodyComponent>(objectId);

	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(irrVecToBt(objIrr->node->getPosition()));
	auto motionState = new btDefaultMotionState(transform);
	aabbox3df bounds = objIrr->node->getBoundingBox();
	vector3df maxEdge = bounds.MaxEdge;
	vector3df minEdge = bounds.MinEdge;
	btVector3 halves((maxEdge.X - minEdge.X) * .5f, (maxEdge.Y - minEdge.Y) * .5f, (maxEdge.Z - minEdge.Z) * .5f);
	auto shape = new btBoxShape(halves);
	btVector3 localInertia;
	f32 mass = 1.f;
	shape->calculateLocalInertia(mass, localInertia);
	rbc->rigidBody = btRigidBody(mass, motionState, shape, localInertia);
	rbc->rigidBody.setSleepingThresholds(0, 0);

	rbc->rigidBody.setUserIndex(getEntityIndex(objectId));
	rbc->rigidBody.setUserIndex2(getEntityVersion(objectId));
	rbc->rigidBody.setUserIndex3(1);

	manager->bulletWorld->addRigidBody(&(rbc->rigidBody));

	return true;
}

bool initializeDefaultPlayer(SceneManager* manager, EntityId shipId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	if (!shipIrr) return false;
	ISceneNode* target = smgr->addEmptySceneNode(0);
	target->setPosition(shipIrr->node->getPosition());
	ICameraSceneNode* camera = smgr->addCameraSceneNode(target, vector3df(0, 5, -15), shipIrr->node->getPosition(), ID_IsNotSelectable, true);
	scene->assign<InputComponent>(shipId);
	auto playerCamera = scene->assign<PlayerComponent>(shipId);
	playerCamera->camera = camera;
	playerCamera->target = target;

	return true;
}

void initializeDefaultHealth(SceneManager* manager, EntityId objectId)
{
	Scene* scene = &manager->scene;

	auto healthComp = scene->assign<HealthComponent>(objectId);
	healthComp->health = 100.f;
	healthComp->maxHealth = 100.f;
}

bool initializeDefaultHUD(SceneManager* manager, EntityId playerId)
{
	Scene* scene = &manager->scene;
	auto player = scene->get<PlayerComponent>(playerId);
	if (!player) return false;

	dimension2du baseSize = dimension2du(960, 540);
	player->rootHUD = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	HUDElement* crossHUD = new HUDCrosshair(manager, player->rootHUD);
	HUDElement* selectHUD = new HUDActiveSelection(manager, player->rootHUD);
	HUDElement* healthHUD = new HUDHealthBar(manager, player->rootHUD);
	HUDElement* speedHUD = new HUDVelocityBar(manager, player->rootHUD);
	player->HUD.push_back(crossHUD);
	player->HUD.push_back(selectHUD);
	player->HUD.push_back(healthHUD);
	player->HUD.push_back(speedHUD);
	return true;
}

void initializeDefaultSensors(SceneManager* manager, EntityId id)
{
	Scene* scene = &manager->scene;
	auto irr = scene->get<IrrlichtComponent>(id);
	auto fac = scene->get<FactionComponent>(id);
	if (!irr || !fac) return; //check, because sensors REQUIRE an irrlicht pos
	auto sensors = scene->assign<SensorComponent>(id);
	sensors->closestContact = INVALID_ENTITY;
	sensors->closestFriendlyContact = INVALID_ENTITY;
	sensors->closestHostileContact = INVALID_ENTITY;
	sensors->detectionRadius = DEFAULT_SENSOR_RANGE;
}

EntityId createDefaultAIShip(SceneManager* manager, vector3df position)
{
	Scene* scene = &manager->scene;

	EntityId id = createDefaultShip(manager, position);
	auto irr = scene->get<IrrlichtComponent>(id);
	irr->name = "AI Ship";
	initializeDefaultHealth(manager, id);
	initializeDefaultRigidBody(manager, id);

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
	ship->engineJetEmit = manager->controller->smgr->addParticleSystemSceneNode(true, irr->node);
	ship->engineJetEmit->setID(ID_IsNotSelectable);
	auto em = ship->engineJetEmit->createSphereEmitter(
		ship->engineJetPos, .6f, getNodeBackward(irr->node) * .002f,
		100, 300, SColor(0, 255, 255, 255), SColor(0, 255, 255, 255),
		50, 200, 1, dimension2df(1.2f, 1.2f), dimension2df(1.4f, 1.4f));
	ship->engineJetEmit->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ship->engineJetEmit->createFadeOutParticleAffector();
	ship->engineJetEmit->addAffector(paf);
	paf->drop();
	ship->engineJetEmit->setMaterialFlag(EMF_LIGHTING, false);
	ship->engineJetEmit->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ship->engineJetEmit->setMaterialTexture(0, manager->defaults.defaultEngineJetTexture);
	ship->engineJetEmit->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	auto engine = manager->controller->smgr->addLightSceneNode(irr->node, ship->engineJetPos, SColorf(0.f, 1.f, 0.f), 1.3f);
	ship->engineLight = engine;
	auto bill = manager->controller->smgr->addBillboardSceneNode(engine, dimension2df(2.f, 2.f));
	engine->setID(ID_IsNotSelectable);
	bill->setID(ID_IsNotSelectable);
	bill->setMaterialTexture(0, manager->defaults.defaultEngineJetTexture);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialFlag(EMF_LIGHTING, false);
}

EntityId explode(SceneManager* manager, vector3df position, f32 duration)
{
	EntityId id = manager->scene.newEntity();
	auto explodeinfo = manager->scene.assign<ExplosionComponent>(id);
	explodeinfo->duration = duration;
	explodeinfo->lifetime = 0;
	explodeinfo->explosion = manager->controller->smgr->addParticleSystemSceneNode(true, 0, ID_IsNotSelectable, position);
	auto em = explodeinfo->explosion->createSphereEmitter(vector3df(0,0,0), 5.f, vector3df(0.05f, 0.f, 0.f), 200, 500, SColor(0, 255, 255, 255), SColor(0, 255, 255, 255),
		50, 200, 360, dimension2df(1.f, 1.f), dimension2df(10.f, 10.f));
	explodeinfo->explosion->setEmitter(em);
	em->drop();
	IParticleAffector* paf = explodeinfo->explosion->createFadeOutParticleAffector(SColor(0,0,0,0),100);
	explodeinfo->explosion->addAffector(paf);
	paf->drop();
	explodeinfo->explosion->setMaterialFlag(EMF_LIGHTING, false);
	explodeinfo->explosion->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	explodeinfo->explosion->setMaterialTexture(0, manager->defaults.defaultExplosion);
	explodeinfo->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	return id;
}