#include "ShipUtils.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"

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
	EntityId shipEntity = createShipFromId(1, manager, position);
	auto ship = manager->scene.get<ShipComponent>(shipEntity);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeDefaultWeapon(manager, shipEntity, i);
	}
	return shipEntity;
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

	auto ai = scene->assign<AIComponent>(id);
	ai->AIType = AI_TYPE_DEFAULT;
	ai->reactionSpeed = AI_DEFAULT_REACTION_TIME;
	ai->damageTolerance = AI_DEFAULT_DAMAGE_TOLERANCE;
	ai->timeSinceLastStateCheck = 0;

	return id;
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

void initializeFaction(SceneManager* manager, EntityId id, FACTION_TYPE type, u32 hostiles, u32 friendlies)
{
	auto fac = manager->scene.assign<FactionComponent>(id);
	setFaction(fac, type, hostiles, friendlies);
}

void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies)
{
	fac->type = type;
	fac->hostileTo = hostilities;
	fac->friendlyTo = friendlies;
}

void initializeNeutralFaction(SceneManager* manager, EntityId id)
{
	initializeFaction(manager, id, FACTION_NEUTRAL, 0, 0);
}
void initializeHostileFaction(SceneManager* manager, EntityId id)
{
	initializeFaction(manager, id, FACTION_HOSTILE, FACTION_PLAYER, FACTION_HOSTILE);
}

void initializePlayerFaction(SceneManager* manager, EntityId id)
{
	initializeFaction(manager, id, FACTION_PLAYER, FACTION_HOSTILE, FACTION_PLAYER);
}

bool initializeSensors(SceneManager* manager, EntityId id, f32 range, f32 updateInterval)
{
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto fac = manager->scene.get<FactionComponent>(id);
	if (!rbc || !fac) return false; //check, because sensors REQUIRE a rigid body pos
	auto sensors = manager->scene.assign<SensorComponent>(id);
	sensors->detectionRadius = range;

	sensors->closestContact = INVALID_ENTITY;
	sensors->closestFriendlyContact = INVALID_ENTITY;
	sensors->closestHostileContact = INVALID_ENTITY;
	sensors->targetContact = INVALID_ENTITY;
	sensors->timeSelected = 0;

	sensors->updateInterval = updateInterval;
	f32 start = 10 / (f32)(std::rand() % 100);
	sensors->timeSinceLastUpdate = start; //stagger sensor updates so it doesn't all happen at once

	return true;
}
bool initializeDefaultSensors(SceneManager* manager, EntityId id)
{
	return initializeSensors(manager, id, DEFAULT_SENSOR_RANGE, DEFAULT_SENSOR_UPDATE_INTERVAL);
}

void initializeShields(SceneManager* manager, EntityId id, f32 amount, f32 delay, f32 recharge)
{
	auto shields = manager->scene.assign<ShieldComponent>(id);
	shields->shields = amount;
	shields->maxShields = amount;
	shields->rechargeDelay = delay;
	shields->rechargeRate = recharge;
	shields->timeSinceLastHit = shields->rechargeDelay;
}
void initializeDefaultShields(SceneManager* manager, EntityId objectId)
{
	initializeShields(manager, objectId, DEFAULT_MAX_SHIELDS, DEFAULT_RECHARGE_DELAY, DEFAULT_RECHARGE_RATE);
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
		if (t) tex.push_back(t);
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
	engine->setID(ID_IsNotSelectable);
}