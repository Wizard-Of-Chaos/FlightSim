#include "ShipUtils.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include <iostream>

EntityId createShipFromId(u32 id, vector3df position, vector3df rotation)
{
	Scene* scene = &sceneManager->scene;

	auto shipEntity = scene->newEntity();

	loadShip(id, shipEntity);
	auto ship = scene->get<ShipComponent>(shipEntity);
	auto irr = scene->get<IrrlichtComponent>(shipEntity);
	irr->node->setPosition(position);
	irr->node->setRotation(rotation);
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		ship->weapons[i] = INVALID_ENTITY;
	}

	return shipEntity;
}

EntityId createDefaultShip(vector3df position, vector3df rotation)
{
	EntityId shipEntity = createShipFromId(1, position, rotation);
	auto ship = sceneManager->scene.get<ShipComponent>(shipEntity);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeDefaultWeapon(shipEntity, i);
	}
	return shipEntity;
}

EntityId createDefaultAIShip(vector3df position, vector3df rotation)
{
	Scene* scene = &sceneManager->scene;

	EntityId id = createDefaultShip(position, rotation);
	auto irr = scene->get<IrrlichtComponent>(id);
	irr->name = "AI Ship";
	initializeDefaultHealth(id);
	initializeDefaultShields(id);
	initializeShipCollisionBody(id, 0);

	auto ai = scene->assign<AIComponent>(id);
	ai->AIType = AI_TYPE_DEFAULT;
	ai->reactionSpeed = AI_DEFAULT_REACTION_TIME;
	ai->damageTolerance = AI_DEFAULT_DAMAGE_TOLERANCE;
	ai->timeSinceLastStateCheck = 0;

	return id;
}
bool initializeShipCollisionBody(EntityId entityId, u32 shipId)
{
	Scene* scene = &sceneManager->scene;

	auto shipComp = scene->get<ShipComponent>(entityId);

	if (!shipComp) return false;
	btVector3 scale(1.f, 1.f, 1.f);
	return initializeBtRigidBody(entityId, stateController->shipData[shipId]->collisionShape, scale, 1.f);
}

EntityId createPlayerShipFromLoadout(vector3df pos, vector3df rot)
{
	EntityId shipEntity = createShipFromId(stateController->playerShip, pos, rot);
	auto ship = sceneManager->scene.get<ShipComponent>(shipEntity);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeWeaponFromId(stateController->playerWeapons[i], shipEntity, i);
	}
	initializeWeaponFromId(stateController->playerPhysWeapon, shipEntity, 0, true);

	initializeDefaultPlayer(shipEntity);
	initializeShipCollisionBody(shipEntity, stateController->playerShip);
	initializeNeutralFaction(shipEntity);
	initializeDefaultHealth(shipEntity);
	initializeDefaultShields(shipEntity);
	initializeDefaultSensors(shipEntity);
	initializeDefaultHUD(shipEntity);
	initializeShipParticles(shipEntity);

	return shipEntity;
}

bool initializeWeaponFromId(u32 id, EntityId shipId, int hardpoint, bool phys)
{
	if (id <= 0) return false;

	Scene* scene = &sceneManager->scene;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	auto shipComp = scene->get<ShipComponent>(shipId);

	if (!shipIrr || !shipComp) return false;

	auto wepEntity = scene->newEntity();
	loadWeapon(id, wepEntity, shipId, phys);
	auto irr = scene->get<IrrlichtComponent>(wepEntity);
	irr->node->setParent(shipIrr->node);
	if (!phys) {
		irr->node->setPosition(shipComp->hardpoints[hardpoint]);
		shipComp->weapons[hardpoint] = wepEntity;
	}
	else {
		irr->node->setPosition(shipComp->physWeaponHardpoint);
		shipComp->physWeapon = wepEntity;
	}
	irr->node->setScale(vector3df(.5f, .5f, .5f));

	auto parentCmp = scene->assign<ParentComponent>(wepEntity);
	parentCmp->parentId = shipId;

	return true;
}

bool initializeDefaultWeapon(EntityId shipId, int hardpoint)
{
	return initializeWeaponFromId(1, shipId, hardpoint);
}

void initializeFaction(EntityId id, FACTION_TYPE type, u32 hostiles, u32 friendlies)
{
	auto fac = sceneManager->scene.assign<FactionComponent>(id);
	setFaction(fac, type, hostiles, friendlies);
}

void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies)
{
	fac->type = type;
	fac->hostileTo = hostilities;
	fac->friendlyTo = friendlies;
}

void initializeNeutralFaction(EntityId id)
{
	initializeFaction(id, FACTION_NEUTRAL, 0, 0);
}
void initializeHostileFaction(EntityId id)
{
	initializeFaction(id, FACTION_HOSTILE, FACTION_PLAYER, FACTION_HOSTILE);
}

void initializePlayerFaction(EntityId id)
{
	initializeFaction(id, FACTION_PLAYER, FACTION_HOSTILE, FACTION_PLAYER);
}

bool initializeSensors(EntityId id, f32 range, f32 updateInterval)
{
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
	auto fac = sceneManager->scene.get<FactionComponent>(id);
	if (!rbc || !fac) return false; //check, because sensors REQUIRE a rigid body pos
	auto sensors = sceneManager->scene.assign<SensorComponent>(id);
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
bool initializeDefaultSensors(EntityId id)
{
	return initializeSensors(id, DEFAULT_SENSOR_RANGE, DEFAULT_SENSOR_UPDATE_INTERVAL);
}

void initializeShields(EntityId id, f32 amount, f32 delay, f32 recharge)
{
	auto shields = sceneManager->scene.assign<ShieldComponent>(id);
	shields->shields = amount;
	shields->maxShields = amount;
	shields->rechargeDelay = delay;
	shields->rechargeRate = recharge;
	shields->timeSinceLastHit = shields->rechargeDelay;
}
void initializeDefaultShields(EntityId objectId)
{
	initializeShields(objectId, DEFAULT_MAX_SHIELDS, DEFAULT_RECHARGE_DELAY, DEFAULT_RECHARGE_RATE);
}

IParticleSystemSceneNode* createShipJet(ISceneNode* node, vector3df pos, vector3df dir)
{

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(true, node);
	ps->setID(ID_IsNotSelectable);
	IParticleEmitter* em = ps->createSphereEmitter(
		vector3df(0,0,0), .1f, dir * .02f,
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
	ps->setMaterialTexture(0, sceneManager->defaults.defaultJetTexture);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	return ps;
}
void initializeShipParticles(EntityId id)
{
	auto ship = sceneManager->scene.get<ShipComponent>(id);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);

	for (u32 i = 0; i < 2; ++i) {
		ship->upJetEmit[i] = createShipJet(irr->node, ship->upJetPos[i], getNodeUp(irr->node));
		ship->downJetEmit[i] = createShipJet(irr->node, ship->downJetPos[i], getNodeDown(irr->node));
		ship->leftJetEmit[i] = createShipJet(irr->node, ship->leftJetPos[i], getNodeLeft(irr->node));
		ship->rightJetEmit[i] = createShipJet(irr->node, ship->rightJetPos[i], getNodeRight(irr->node));
		ship->reverseJetEmit[i] = createShipJet(irr->node, ship->reverseJetPos[i], getNodeForward(irr->node));
	}
	ship->engineJetEmit = smgr->addVolumeLightSceneNode(irr->node,
		ID_IsNotSelectable, 256, 256, SColor(255, 100, 250, 100), SColor(0, 0, 0, 0), ship->engineJetPos,
		vector3df(-90, 0, 0), vector3df(2, 1, 2));

	array<ITexture*> tex;
	for (s32 i = 4; i > 0; --i) {
		stringc str = "effects/engine/engine";
		str += i;
		str += ".png";
		ITexture* t = driver->getTexture(str.c_str());
		if (t) tex.push_back(t);
	}

	ISceneNodeAnimator* glowie = smgr->createTextureAnimator(tex, 50);
	ship->engineJetEmit->addAnimator(glowie);
	glowie->drop(); //Terry would be proud

	ship->engineJetEmit->setMaterialFlag(EMF_LIGHTING, false);
	ship->engineJetEmit->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ship->engineJetEmit->setMaterialTexture(0, sceneManager->defaults.defaultEngineJetTexture);
	ship->engineJetEmit->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);


	auto engine = smgr->addLightSceneNode(irr->node, ship->engineJetPos, SColorf(0.f, 1.f, 0.f), 1.3f);
	ship->engineLight = engine;
	engine->setID(ID_IsNotSelectable);
}