#include "ShipUtils.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include "SensorComponent.h"
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

	initializeShipParticles(shipEntity);
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

EntityId createAIShipFromLoadout(LoadoutData loadout, vector3df position, vector3df rotation, AI_TYPE type, FACTION_TYPE fac, u32 hostilities, u32 friendlies)
{
	EntityId id = createShipFromId(loadout.shipId, position, rotation);
	auto ship = sceneManager->scene.get<ShipComponent>(id);

	for (u32 i = 0; i < ship->hardpointCount; ++i) {
		initializeWeaponFromId(loadout.weaponIds[i], id, i);
	}
	initializeDefaultHealth(id);
	initializeDefaultShields(id);
	initializeShipCollisionBody(id, loadout.shipId);
	initializeFaction(id, fac, hostilities, friendlies);
	initializeDefaultSensors(id);

	auto ai = sceneManager->scene.assign<AIComponent>(id);
	ai->AIType = type;
	ai->reactionSpeed = AI_DEFAULT_REACTION_TIME;
	ai->damageTolerance = AI_DEFAULT_DAMAGE_TOLERANCE;
	ai->timeSinceLastStateCheck = 0;
	return id;
}
EntityId createDefaultAIShip(vector3df position, vector3df rotation)
{
	Scene* scene = &sceneManager->scene;

	EntityId id = createDefaultShip(position, rotation);
	auto irr = scene->get<IrrlichtComponent>(id);
	irr->name = "AI Ship";
	initializeDefaultHealth(id);
	initializeDefaultShields(id);
	initializeShipCollisionBody(id, 1);
	initializeHostileFaction(id);
	initializeDefaultSensors(id);

	auto ai = scene->assign<AIComponent>(id);
	ai->AIType = AI_TYPE_DEFAULT;
	ai->reactionSpeed = AI_DEFAULT_REACTION_TIME;
	ai->damageTolerance = AI_DEFAULT_DAMAGE_TOLERANCE;
	ai->timeSinceLastStateCheck = 10.f / (f32)(std::rand() % 100);

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
	initializePlayerFaction(shipEntity);

	initializeShipCollisionBody(shipEntity, stateController->playerShip);
	initializeDefaultHealth(shipEntity);
	initializeDefaultShields(shipEntity);
	initializeDefaultSensors(shipEntity);
	initializeDefaultHUD(shipEntity);

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
	f32 start = 10.f / (f32)(std::rand() % 100);
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
	ps->setMaterialTexture(0, stateController->assets.getTextureAsset("defaultJetTexture"));
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	return ps;
}
void initializeShipParticles(EntityId id)
{
	auto particles = sceneManager->scene.assign<ShipParticleComponent>(id);
	auto ship = sceneManager->scene.get<ShipComponent>(id);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);

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
	for (s32 i = 4; i > 0; --i) {
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

EntityId createShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot)
{
	auto id = createShipFromId(inst.ship.shipDataId, pos, rot);
	ShipComponent* ship = sceneManager->scene.get<ShipComponent>(id);
	*ship = inst.ship;
	for (u32 i = 0; i < ship->hardpointCount; ++i) {
		WeaponInfoComponent wepReplace = inst.weps[i];
		initializeWeaponFromId(wepReplace.wepDataId, id, i);
		if (wepReplace.type != WEP_NONE) {
			WeaponInfoComponent* wep = sceneManager->scene.get<WeaponInfoComponent>(ship->weapons[i]);
			if (wep) {
				wep->ammunition = wepReplace.ammunition;
			}
		}
	}
	initializeWeaponFromId(inst.physWep.wepDataId, id, 0, true);

	return id;
}

EntityId createPlayerShipFromInstance(vector3df pos, vector3df rot)
{
	ShipInstance inst = stateController->campaign.playerShip;
	u32 shipId = inst.ship.shipDataId;

	auto id = createShipFromInstance(inst, pos, rot);
	initializeDefaultPlayer(id);
	initializeNeutralFaction(id);

	initializeShipCollisionBody(id, shipId);
	initializeDefaultShields(id);
	initializeDefaultSensors(id);

	initializeHealth(id, stateController->campaign.playerShip.hp.maxHealth);
	auto hp = sceneManager->scene.get<HealthComponent>(id);
	*hp = stateController->campaign.playerShip.hp;

	initializeDefaultHUD(id);

	auto ship = sceneManager->scene.get<ShipComponent>(id);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
	auto meshnode = (IMeshSceneNode*)irr->node;
	ShipData* data = stateController->shipData[ship->shipDataId];
	IMesh* m = stateController->assets.getMeshAsset(data->name);
	IMeshSceneNode* mesh = smgr->addMeshSceneNode(m, irr->node);
	mesh->setScale(vector3df(1.2f));
	mesh->setPosition(vector3df(0.f, 0.f, .5f));
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	if (!gpu) return id;
	ship->shieldshader.setShip(mesh);

	std::string fname = "effects/shaders/shield.hlsl";
	ship->shadermat = gpu->addHighLevelShaderMaterialFromFiles(
		fname.c_str(), "vertexMain", EVST_VS_1_1,
		fname.c_str(), "pixelMain", EPST_PS_1_1,
		&ship->shieldshader, EMT_TRANSPARENT_ADD_COLOR, 0, EGSL_DEFAULT);
	mesh->setMaterialFlag(EMF_LIGHTING, false);
	mesh->setMaterialFlag(EMF_BLEND_OPERATION, true);
	mesh->setMaterialTexture(0, driver->getTexture(stateController->shipData[ship->shipDataId]->shipTexture.c_str()));
	mesh->setMaterialType((E_MATERIAL_TYPE)ship->shadermat);
	return id;
}