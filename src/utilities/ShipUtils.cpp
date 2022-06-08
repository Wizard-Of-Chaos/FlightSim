#include "ShipUtils.h"
#include "GameController.h"
#include "GameStateController.h"
#include "SensorComponent.h"
#include <iostream>

flecs::entity createShipFromId(u32 id, vector3df position, vector3df rotation)
{
	auto shipEntity = game_world->entity();
	loadShip(id, shipEntity);
	if (!shipEntity.has<ShipComponent>() || !shipEntity.has<IrrlichtComponent>()) return INVALID_ENTITY;
	auto ship = shipEntity.get_mut<ShipComponent>();
	auto irr = shipEntity.get_mut<IrrlichtComponent>();

	irr->node->setPosition(position);
	irr->node->setRotation(rotation);
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		ship->weapons[i] = INVALID_ENTITY;
	}

	initializeShipParticles(shipEntity);
	return shipEntity;
}

flecs::entity createDefaultShip(vector3df position, vector3df rotation)
{
	flecs::entity shipEntity = createShipFromId(1, position, rotation);
	auto ship = shipEntity.get_mut<ShipComponent>();

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		initializeDefaultWeapon(shipEntity, i);
	}
	return shipEntity;
}

flecs::entity createDefaultAIShip(vector3df position, vector3df rotation)
{
	flecs::entity id = createDefaultShip(position, rotation);
	auto irr = id.get_mut<IrrlichtComponent>();
	irr->name = "AI Ship";
	initializeDefaultHealth(id);
	initializeDefaultShields(id);
	initializeShipCollisionBody(id, 1);
	initializeHostileFaction(id);
	initializeDefaultSensors(id);
	initializeDefaultAI(id);

	return id;
}

bool initializeShipCollisionBody(flecs::entity entityId, u32 shipId, bool carrier)
{
	if (!entityId.has<ShipComponent>()) return false;
	auto shipComp = entityId.get_mut<ShipComponent>();

	btVector3 scale(1.f, 1.f, 1.f);
	btScalar mass = 1.f;
	btConvexHullShape hull = stateController->shipData[shipId]->collisionShape;
	if (carrier) {
		hull = stateController->carrierData[shipId]->collisionShape;
		CarrierData* data = stateController->carrierData[shipId];
		scale = irrVecToBt(data->carrierComponent.scale);
		mass = data->mass;
	}
	return initializeBtRigidBody(entityId, hull, scale, mass);
}

bool initializeWeaponFromId(u32 id, flecs::entity shipId, int hardpoint, bool phys)
{
	if (id <= 0) return false;

	if (!shipId.has<ShipComponent>() || !shipId.has<IrrlichtComponent>()) return false;

	auto shipIrr = shipId.get<IrrlichtComponent>();
	auto shipComp = shipId.get_mut<ShipComponent>();

	auto wepEntity = game_world->entity().child_of(shipId);

	loadWeapon(id, wepEntity, shipId, phys);
	auto irr = wepEntity.get_mut<IrrlichtComponent>();
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

	return true;
}

bool initializeDefaultWeapon(flecs::entity shipId, int hardpoint)
{
	return initializeWeaponFromId(1, shipId, hardpoint);
}

void initializeFaction(flecs::entity id, FACTION_TYPE type, u32 hostiles, u32 friendlies)
{
	auto fac = id.get_mut<FactionComponent>();
	setFaction(fac, type, hostiles, friendlies);
}

void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies)
{
	fac->type = type;
	fac->hostileTo = hostilities;
	fac->friendlyTo = friendlies;
}

void initializeNeutralFaction(flecs::entity id)
{
	initializeFaction(id, FACTION_NEUTRAL, 0, 0);
}
void initializeHostileFaction(flecs::entity id)
{
	initializeFaction(id, FACTION_HOSTILE, FACTION_PLAYER, FACTION_HOSTILE);
}

void initializePlayerFaction(flecs::entity id)
{
	initializeFaction(id, FACTION_PLAYER, FACTION_HOSTILE, FACTION_PLAYER);
}

bool initializeSensors(flecs::entity id, f32 range, f32 updateInterval)
{
	//Faction component and rigid body component are both REQUIRED for sensors (the one for hostiles, the other for positioning).
	if (!id.has<BulletRigidBodyComponent>() || !id.has<FactionComponent>()) return false;
	auto sensors = id.get_mut<SensorComponent>();
	sensors->detectionRadius = range;

	sensors->closestContact = INVALID_ENTITY;
	sensors->closestFriendlyContact = INVALID_ENTITY;
	sensors->closestHostileContact = INVALID_ENTITY;
	sensors->targetContact = INVALID_ENTITY;
	sensors->timeSelected = 0;

	sensors->updateInterval = updateInterval;
	f32 start = 10.f / (f32)(std::rand() % 100);
	sensors->timeSinceLastUpdate = start; //stagger sensor updates so it doesn't all happen at once

	return true;
}
bool initializeDefaultSensors(flecs::entity id)
{
	return initializeSensors(id, DEFAULT_SENSOR_RANGE, DEFAULT_SENSOR_UPDATE_INTERVAL);
}

void initializeShields(flecs::entity id, f32 amount, f32 delay, f32 recharge)
{
	auto shields = id.get_mut<ShieldComponent>();
	shields->shields = amount;
	shields->maxShields = amount;
	shields->rechargeDelay = delay;
	shields->rechargeRate = recharge;
	shields->timeSinceLastHit = shields->rechargeDelay;
}
void initializeDefaultShields(flecs::entity objectId)
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
	ps->setMaterialTexture(0, stateController->assets.getTextureAsset("defaultJetTexture"));
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	return ps;
}
void initializeShipParticles(flecs::entity id)
{
	auto particles = id.get_mut<ShipParticleComponent>(); // this should assign the particle component
	auto ship = id.get<ShipComponent>();
	auto irr = id.get<IrrlichtComponent>();

	for (u32 i = 0; i < 2; ++i) {
		particles->upJetEmit[i] = createShipJet(irr->node, ship->upJetPos[i], getNodeUp(irr->node));
		particles->downJetEmit[i] = createShipJet(irr->node, ship->downJetPos[i], getNodeDown(irr->node));
		particles->leftJetEmit[i] = createShipJet(irr->node, ship->leftJetPos[i], getNodeLeft(irr->node));
		particles->rightJetEmit[i] = createShipJet(irr->node, ship->rightJetPos[i], getNodeRight(irr->node));
		particles->reverseJetEmit[i] = createShipJet(irr->node, ship->reverseJetPos[i], getNodeForward(irr->node));
	}
	particles->engineJetEmit = smgr->addVolumeLightSceneNode(irr->node,
		ID_IsNotSelectable, 256, 256, SColor(255, 100, 250, 100), SColor(0, 0, 0, 0), ship->engineJetPos,
		vector3df(-90, 0, 0), vector3df(2, 1, 2));

	array<ITexture*> tex;
	for (s32 i = 4; i > 0; --i) { //todo: set up custom engine loops for other ships
		stringc str = "effects/engine/engine";
		str += i;
		str += ".png";
		ITexture* t = driver->getTexture(str.c_str());
		if (t) tex.push_back(t);
	}

	ISceneNodeAnimator* glowie = smgr->createTextureAnimator(tex, 50);
	particles->engineJetEmit->addAnimator(glowie);
	glowie->drop(); //Terry would be proud

	particles->engineJetEmit->setMaterialFlag(EMF_LIGHTING, false);
	particles->engineJetEmit->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	particles->engineJetEmit->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);


	auto engine = smgr->addLightSceneNode(irr->node, ship->engineJetPos, SColorf(0.f, 1.f, 0.f), 1.3f);
	particles->engineLight = engine;
	engine->setID(ID_IsNotSelectable);
}

flecs::entity createShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot)
{
	auto id = createShipFromId(inst.ship.shipDataId, pos, rot);
	if (id == INVALID_ENTITY) return id;
	ShipComponent* ship = id.get_mut<ShipComponent>();
	*ship = inst.ship;
	for (u32 i = 0; i < ship->hardpointCount; ++i) {
		WeaponInfoComponent wepReplace = inst.weps[i];
		initializeWeaponFromId(wepReplace.wepDataId, id, i);
		if (wepReplace.type != WEP_NONE) {
			WeaponInfoComponent* wep = ship->weapons[i].get_mut<WeaponInfoComponent>(); 
			if (wep) {
				wep->ammunition = wepReplace.ammunition;
			}
		}
	}
	initializeDefaultHealth(id);
	HealthComponent* hp = id.get_mut<HealthComponent>();
	*hp = inst.hp;
	initializeShipCollisionBody(id, inst.ship.shipDataId);

	initializeWeaponFromId(inst.physWep.wepDataId, id, 0, true);

	return id;
}
flecs::entity createAIShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot)
{
	auto id = createShipFromInstance(inst, pos, rot);
	if (id == INVALID_ENTITY) return id;
	auto irr = id.get_mut<IrrlichtComponent>(id);
	irr->name = "AI Ship";
	initializeDefaultShields(id);
	initializeNeutralFaction(id);
	initializeDefaultSensors(id);
	initializeDefaultAI(id);
	return id;
}

flecs::entity createFriendlyAIShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot)
{
	auto id = createAIShipFromInstance(inst, pos, rot);
	if (id == INVALID_ENTITY) return id;
	auto fac = id.get_mut<FactionComponent>();
	setFaction(fac, FACTION_PLAYER, FACTION_HOSTILE, FACTION_PLAYER);
	return id;
}

flecs::entity createHostileAIShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot)
{
	auto id = createAIShipFromInstance(inst, pos, rot);
	if (id == INVALID_ENTITY) return id;
	auto fac = id.get_mut<FactionComponent>();
	setFaction(fac, FACTION_HOSTILE, FACTION_PLAYER, FACTION_HOSTILE);
	return id;
}

flecs::entity carrierSpawnShip(ShipInstance& inst, vector3df spawnPos, vector3df spawnRot, FactionComponent* carrFac)
{
	auto id = createAIShipFromInstance(inst, spawnPos, spawnPos);
	if (id == INVALID_ENTITY) return id;
	auto fac = id.get_mut<FactionComponent>();
	*fac = *carrFac;
	auto obj = id.get_mut<ObjectiveComponent>();
	obj->type = OBJ_DESTROY;
	return id;
}

flecs::entity createPlayerShipFromInstance(vector3df pos, vector3df rot)
{
	ShipInstance inst = stateController->campaign.playerShip;
	u32 shipId = inst.ship.shipDataId;

	auto id = createShipFromInstance(inst, pos, rot);
	initializeDefaultPlayer(id);
	initializeNeutralFaction(id);
	initializeDefaultShields(id);
	initializeDefaultSensors(id);
	initializeDefaultHUD(id);
	return id;
}