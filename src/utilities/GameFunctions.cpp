#include "GameFunctions.h"
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

void destroyObject(flecs::entity id)
{
	if (!id.is_alive()) return;

	if (id.has<ShipComponent>()) {
		auto ship = id.get_mut<ShipComponent>();
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			destroyObject(ship->weapons[i]);
		}
	}

	if (id.has<IrrlichtComponent>()) {
		auto irr = id.get_mut<IrrlichtComponent>();
		irr->node->removeAll();
		irr->node->remove();
	}
	if (id.has<BulletRigidBodyComponent>()) {
		auto rbc = id.get_mut<BulletRigidBodyComponent>();
		bWorld->removeRigidBody(&rbc->rigidBody);
	}
	if (id.has<BulletGhostComponent>()) {
		auto ghost = id.get_mut<BulletGhostComponent>();
		bWorld->removeCollisionObject(&ghost->ghost);
	}

	id.destruct();
}

bool initializeDefaultPlayer(flecs::entity shipId)
{
	if (!shipId.has<IrrlichtComponent>()) return false;
	auto shipIrr = shipId.get<IrrlichtComponent>();

	ISceneNode* target = smgr->addEmptySceneNode(0);
	target->setPosition(shipIrr->node->getPosition());
	ICameraSceneNode* camera = smgr->addCameraSceneNode(target, vector3df(0, 5, -20), shipIrr->node->getPosition(), ID_IsNotSelectable, true);
	shipId.add<InputComponent>();
	auto player = shipId.get_mut<PlayerComponent>();
	player->camera = camera;
	player->target = target;

	player->thrust = vector3df(0, 0, 0);
	player->rotation = vector3df(0, 0, 0);
	gameController->playerEntity = shipId;
	return true;
}

void initializeHealth(flecs::entity id, f32 healthpool)
{
	auto hp = id.get_mut<HealthComponent>();
	auto dmg = id.get_mut<DamageTrackingComponent>();
	hp->health = healthpool;
	hp->maxHealth = healthpool;
}
void initializeDefaultHealth(flecs::entity objectId)
{
	initializeHealth(objectId, DEFAULT_MAX_HEALTH);
}

bool initializeDefaultHUD(flecs::entity playerId)
{
	if (!playerId.has<PlayerComponent>()) return false;
	auto player = playerId.get_mut<PlayerComponent>();

	dimension2du baseSize = dimension2du(960, 540);
	player->rootHUD = guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	HUDElement* selectHUD = new HUDActiveSelection(player->rootHUD);
	HUDElement* resourceHUD = new HUDResources(player->rootHUD, playerId);
	HUDElement* speedHUD = new HUDVelocityBar(player->rootHUD);
	player->HUD.push_back(selectHUD);
	player->HUD.push_back(resourceHUD);
	player->HUD.push_back(speedHUD);
	return true;
}

void initializeAI(flecs::entity id, AI_TYPE type, f32 reactSpeed, f32 damageTolerance)
{
	auto ai = id.get_mut<AIComponent>();
	ai->AIType = type;
	ai->reactionSpeed = reactSpeed;
	ai->damageTolerance = damageTolerance;
	ai->timeSinceLastStateCheck = 10.f / (f32)(std::rand() % 100); //This is designed to make it so that the AI don't all check at the same time for framerate purposes

}

void initializeDefaultAI(flecs::entity id)
{
	initializeAI(id, AI_TYPE_DEFAULT, AI_DEFAULT_REACTION_TIME, AI_DEFAULT_DAMAGE_TOLERANCE);
}

flecs::entity explode(vector3df position, f32 duration, f32 scale, f32 radius, f32 damage, f32 force)
{
	flecs::entity id = game_world->entity();
	auto exp = id.get_mut<ExplosionComponent>();
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
	exp->explosion->setMaterialTexture(0, stateController->assets.getTextureAsset("defaultExplosion"));
	exp->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	exp->light = smgr->addLightSceneNode(0, position, SColorf(1.f, .9f, .1f));

	exp->force = force;
	exp->damage = damage;
	exp->radius = radius;

	return id;
}

ShipInstance getEndScenarioData()
{
	ShipInstance inst;
	auto hp = gameController->playerEntity.get<HealthComponent>();
	inst.hp = *hp;

	auto ship = gameController->playerEntity.get<ShipComponent>();
	inst.ship = *ship;
	for (u32 i = 0; i < ship->hardpointCount; ++i) {
		if (!ship->weapons[i].is_alive()) { //there's no weapon here
			inst.weps[i] = stateController->weaponData[0]->weaponComponent; //add the no-weapon component
			continue;
		}

		auto wep = ship->weapons[i].get_mut<WeaponInfoComponent>();
		if (!wep->usesAmmunition) continue;

		if (wep->clip < wep->maxClip) { //if the clip is partially spent just reload the damn thing
			wep->clip = wep->maxClip;
			if (wep->ammunition >= wep->maxClip) {
				wep->ammunition -= wep->maxClip;
			}
			else {
				wep->ammunition = 0; 
			}
		}
		inst.weps[i] = *wep;
	}
	if (!ship->physWeapon.is_alive()) {
		inst.physWep = stateController->physWeaponData[0]->weaponComponent;
	}
	else {
		auto phys = ship->physWeapon.get<WeaponInfoComponent>();
		inst.physWep = *phys;
	}
	//todo: make it so that it would also grab the health / ammo of wingmen
	return inst;
}

ShipInstance newShipInstance()
{
	ShipInstance ship;
	ship.ship = stateController->shipData[0]->shipComponent;
	ship.hp.health = 100.f;
	ship.hp.maxHealth = 100.f;

	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		ship.weps[i] = stateController->weaponData[0]->weaponComponent;
	}
	ship.physWep = stateController->physWeaponData[0]->weaponComponent;

	return ship;
}

void initNewCampaign()
{
	stateController->campaign = Campaign();

	for (u32 i = 0; i < NUM_SCENARIO_OPTIONS; ++i) {
		stateController->campaign.possibleScenarios[i] = randomScenario();
	}
	stateController->campaign.currentScenario = randomScenario();
	stateController->campaign.currentScenario.detectionChance = 0;

	ShipInstance defaultShip = newShipInstance();

	defaultShip.ship = stateController->shipData[0]->shipComponent;
	defaultShip.weps[0] = stateController->weaponData[3]->weaponComponent;
	defaultShip.weps[1] = stateController->weaponData[3]->weaponComponent;
	defaultShip.physWep = stateController->physWeaponData[1]->weaponComponent;

	stateController->campaign.playerShip = defaultShip;

	stateController->campaign.availableWeapons.push_back(stateController->weaponData[0]->weaponComponent);
	stateController->campaign.availablePhysWeapons.push_back(stateController->physWeaponData[0]->weaponComponent);
	stateController->campaign.availablePhysWeapons.push_back(stateController->physWeaponData[2]->weaponComponent);
}

ShipInstance randomShipInstance()
{
	ShipInstance inst = newShipInstance();
	u32 id = std::rand() % stateController->shipData.size();
	inst.ship = stateController->shipData[id]->shipComponent;

	inst.hp.health = (f32)(std::rand() % (u32)(inst.hp.maxHealth));

	return inst;
}
WeaponInfoComponent randomWeapon()
{
	u32 id = std::rand() % stateController->weaponData.size();
	if (id == 0) id += 1;
	WeaponInfoComponent wep = stateController->weaponData[id]->weaponComponent;
	if (wep.usesAmmunition) {
		u32 clips = wep.maxAmmunition / wep.maxClip;
		wep.ammunition = wep.maxClip * (std::rand() % clips);
	}
	return wep;
}
