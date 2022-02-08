#include "GameFunctions.h"
#include "SceneManager.h"
#include "GameController.h"
#include "IrrlichtUtils.h"

//Convenience functions to swap back and forth between irrlicht and bullet vectors.
vector3df bulletVectorToIrrlicht(btVector3 vec)
{
	return vector3df(vec.x(), vec.y(), vec.z());
}

btVector3 irrlichtVectorToBullet(vector3df vec)
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
	manager->defaults.defaultShipMesh = manager->controller->smgr->getMesh("models/tux/Tux.obj");
	manager->defaults.defaultShipTexture = manager->controller->driver->getTexture("models/tux/BulletShipTex.png");
	manager->defaults.defaultWeaponMesh = manager->controller->smgr->getMesh("models/wazer/wazer.obj");
	manager->defaults.defaultObstacleMesh = manager->controller->smgr->getMesh("models/asteroid/Asteroid.obj");
	manager->defaults.defaultProjectileTexture = manager->controller->driver->getTexture("effects/particlered.bmp");
	manager->defaults.defaultCrosshairTexture = manager->controller->driver->getTexture("hud/crosshair.png");
	manager->defaults.defaultSelectionTexture = manager->controller->driver->getTexture("hud/selection.png");
}

//Creates a default ship at the given position in Irrlicht space. Returns the entity ID.
EntityId createDefaultShip(SceneManager* manager, vector3df position)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	IMeshSceneNode* playerNode = smgr->addMeshSceneNode(manager->defaults.defaultShipMesh, 0, -1, position);
	playerNode->setMaterialTexture(0, manager->defaults.defaultShipTexture);

	auto shipEntity = scene->newEntity();
	playerNode->setName(idToStr(shipEntity).c_str());
	playerNode->setID(ID_IsSelectable);
	auto irrComponent = scene->assign<IrrlichtComponent>(shipEntity);

	irrComponent->node = playerNode;
	irrComponent->name = "Default Ship";

	auto shipComponent = scene->assign<ShipComponent>(shipEntity);
	shipComponent->forwardThrust = DEFAULT_FORWARD_THRUST;
	shipComponent->strafeThrust = DEFAULT_STRAFE_THRUST;
	shipComponent->pitchThrust = DEFAULT_PITCH_THRUST;
	shipComponent->yawThrust = DEFAULT_YAW_THRUST;
	shipComponent->rollThrust = DEFAULT_ROLL_THRUST;

	shipComponent->hardpointCount = 2;
	shipComponent->hardpoints[0] = vector3df(2.4816f, .25f, -6.0088f);
	shipComponent->hardpoints[1] = vector3df(-2.4816f, .25f, -6.0088f);


	for (unsigned int i = 0; i < shipComponent->hardpointCount; ++i) {
		initializeDefaultWeapon(manager, shipEntity, i);
	}

	return shipEntity;
}

//Creates a default obstacle at the given position (an asteroid). Returns the Entity ID. Does not create hitbox or health by default.
EntityId createDefaultObstacle(SceneManager* manager, vector3df position)
{
	ISceneManager* smgr = manager->controller->smgr;
	Scene* scene = &manager->scene;

	auto roidEntity = scene->newEntity();
	auto irrComp = scene->assign<IrrlichtComponent>(roidEntity);
	irrComp->node = smgr->addMeshSceneNode(manager->defaults.defaultObstacleMesh);
	irrComp->node->setID(ID_IsSelectable);
	irrComp->node->setPosition(position);
	irrComp->node->setName(idToStr(roidEntity).c_str());
	irrComp->name = "Asteroid";

	auto healthComp = scene->assign<HealthComponent>(roidEntity);
	healthComp->health = DEFAULT_MAX_HEALTH;
	healthComp->maxHealth = DEFAULT_MAX_HEALTH;

	return roidEntity;
}

//Creates a default projectile and kicks it in the given direction.
//Requires: A weapon ID with an associated weapon info component. Returns INVALID_ENTITY otherwise.
//Also initializes a sphere rigid body associated with the entity.
EntityId createProjectileEntity(SceneManager* manager, vector3df spawnPos, vector3df direction, EntityId weaponId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto wepInfo = scene->get<WeaponInfoComponent>(weaponId);

	if (!wepInfo) return INVALID_ENTITY;

	auto projectileEntity = scene->newEntity();
	auto projectileInfo = scene->assign<ProjectileInfoComponent>(projectileEntity);
	projectileInfo->type = wepInfo->type;
	projectileInfo->speed = wepInfo->projectileSpeed;
	projectileInfo->startPos = spawnPos;
	projectileInfo->range = wepInfo->range;
	projectileInfo->damage = wepInfo->damage;

	auto irrComp = scene->assign<IrrlichtComponent>(projectileEntity);
	//this needs to be abstracted out to creating different types of node, for now it's just the laser with a crappy particle
	irrComp->node = smgr->addLightSceneNode(0, spawnPos, SColorf(.8f, .2f, .2f), 30.f);
	ISceneNode* bill = smgr->addBillboardSceneNode(irrComp->node, dimension2d<f32>(3.f, 3.f));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, manager->defaults.defaultProjectileTexture);

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, irrComp->node);
	IParticleEmitter* em = ps->createSphereEmitter(ps->getPosition(), .5f, //spawn point and radius
		(-direction * .01f), 30, 60, //direction, emit rate min/max
		SColor(0, 100, 50, 50), SColor(0, 255, 100, 100), 500, 2000, 0, //min / max color, shortest lifetime, longest lifetime, angle
		dimension2df(.1f, .1f), dimension2df(1.f, 1.f)); //min / max size
	ps->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, manager->defaults.defaultProjectileTexture);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	auto rigidBodyInfo = scene->assign<BulletRigidBodyComponent>(projectileEntity);
	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(irrlichtVectorToBullet(spawnPos));

	auto motionState = new btDefaultMotionState(transform);

	auto shape = new btSphereShape(1.5f);
	btVector3 localInertia;
	f32 mass = .1f;
	shape->calculateLocalInertia(mass, localInertia);
	rigidBodyInfo->rigidBody = btRigidBody(mass, motionState, shape, localInertia);

	rigidBodyInfo->rigidBody.applyCentralImpulse(irrlichtVectorToBullet(direction) * projectileInfo->speed);

	rigidBodyInfo->rigidBody.setUserIndex(getEntityIndex(projectileEntity));
	rigidBodyInfo->rigidBody.setUserIndex2(getEntityVersion(projectileEntity));
	rigidBodyInfo->rigidBody.setUserIndex3(1);

	manager->bulletWorld->addRigidBody(&rigidBodyInfo->rigidBody);

	return projectileEntity;
}

//Destroys a given projectile. Removes it from irrlicht and bullet considerations and the entity list.
void destroyProjectile(SceneManager* manager, EntityId projectile)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto rigidBodyInfo = scene->get<BulletRigidBodyComponent>(projectile);
	if (rigidBodyInfo) {
		manager->bulletWorld->removeRigidBody(&rigidBodyInfo->rigidBody); //removes the rigid body from the bullet physics
	}

	auto irrComp = scene->get<IrrlichtComponent>(projectile);
	if (irrComp) {
		irrComp->node->removeAll(); //removes the children of the node (like particles, billboards etc)
		irrComp->node->remove(); // removes the node
	}

	scene->remove<BulletRigidBodyComponent>(projectile);
	scene->remove<IrrlichtComponent>(projectile);
	scene->remove<ProjectileInfoComponent>(projectile);
	scene->destroyEntity(projectile); //bye bye your life goodbye
}

//Destroys a given object with a bullet component and an irrlicht component.
//Usually called to get rid of something that hit 0 health.
void destroyObject(SceneManager* manager, EntityId id)
{
	auto irrComp = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	for (auto playerId : SceneView<PlayerComponent>(manager->scene)) {
		auto player = manager->scene.get<PlayerComponent>(playerId);
		if (player->activeSelection == irrComp->node) {
			player->activeSelection = nullptr;
			player->selectionGui->setVisible(false);
		}
	}

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

//Adds a default weapon to the given ship at the given hardpoint.
//Requires: Irrlicht component, ship component. Returns false without that.
bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	auto shipComp = scene->get<ShipComponent>(shipId);

	if (!shipIrr || !shipComp) return false;

	auto wepEntity = scene->newEntity();
	auto wepInfo = scene->assign<WeaponInfoComponent>(wepEntity);
	wepInfo->isFiring = false;
	wepInfo->type = DEFAULT_WEAPON_TYPE;
	wepInfo->firingSpeed = DEFAULT_FIRING_SPEED;
	wepInfo->projectileSpeed = DEFAULT_PROJECTILE_SPEED;
	wepInfo->range = DEFAULT_WEAPON_RANGE;
	wepInfo->timeSinceLastShot = 0.f;
	wepInfo->damage = DEFAULT_WEAPON_DAMAGE;

	auto wepIrrComp = scene->assign<IrrlichtComponent>(wepEntity);
	wepIrrComp->node = smgr->addMeshSceneNode(manager->defaults.defaultWeaponMesh, shipIrr->node, -1, shipComp->hardpoints[hardpoint], vector3df(0, 0, 0), vector3df(.5f, .5f, .5f));
	wepIrrComp->node->setID(ID_IsNotSelectable);
	shipComp->weapons[hardpoint] = wepEntity;

	return true;
}

//Sets up a default hitbox (an axis-aligned bounding box) on the given object.
//Requires: Irrlicht component. Returns false otherwise.
bool initializeDefaultRigidBody(SceneManager* manager, EntityId objectId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto objIrr = scene->get<IrrlichtComponent>(objectId);
	
	if (!objIrr) return false;

	BulletRigidBodyComponent* rbc = scene->assign<BulletRigidBodyComponent>(objectId);

	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(irrlichtVectorToBullet(objIrr->node->getPosition()));
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

//Adds on a player component and an input component to a given ship.
//Requires: Irrlicht component. Returns false without that.
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
	auto playerCamera = scene->assign<PlayerComponent>(shipId);
	playerCamera->camera = camera;
	playerCamera->target = target;

	return true;
}

//Adds a health component to the given entity. 
//Doesn't require anything, technically, but you should really only use it on things what have rigid bodies and guns and stuff.
void initializeDefaultHealth(SceneManager* manager, EntityId objectId)
{
	Scene* scene = &manager->scene;

	auto healthComp = scene->assign<HealthComponent>(objectId);
	healthComp->health = 100.f;
	healthComp->maxHealth = 100.f;
}

//Sets up the HUD for the given player entity.
//Requires a player component; returns false otherwise.
bool initializeDefaultHUD(SceneManager* manager, EntityId playerId)
{
	Scene* scene = &manager->scene;
	auto player = scene->get<PlayerComponent>(playerId);
	if (!player) return false;
	player->activeSelection = nullptr;
	player->crosshairGui = manager->controller->guienv->addImage(manager->defaults.defaultCrosshairTexture, position2di(0, 0));
	player->selectionGui = manager->controller->guienv->addImage(manager->defaults.defaultSelectionTexture, position2di(0, 0));

	player->crosshairGui->setVisible(true);
	player->selectionGui->setVisible(false);

	return true;
}

//Creates a default AI ship at the given position. Returns the ID.
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
	ai->detectionRadius = AI_DEFAULT_DETECTION_RADIUS;
	ai->reactionSpeed = AI_DEFAULT_REACTION_TIME;
	ai->timeSinceLastStateCheck = 0;
	ai->nearbyHostiles = false;
	ai->closestContact = INVALID_ENTITY;

	return id;
}