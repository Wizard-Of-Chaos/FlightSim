#include "ProjectileUtils.h"
#include "GameController.h"
#include "SceneManager.h"
#include "GameStateController.h"

#include <iostream>

vector3df adjustAccuracy(vector3df dir, f32 accuracy)
{
	f32 xspread, yspread, zspread;
	xspread = (f32)(std::rand() % 10) - 5.f;
	yspread = (f32)(std::rand() % 10) - 5.f;
	zspread = (f32)(std::rand() % 10) - 5.f;

	xspread /= (accuracy * 100.f);
	yspread /= (accuracy * 100.f);
	zspread /= (accuracy * 100.f);
	vector3df newDir = dir;
	newDir.X += xspread;
	newDir.Y += yspread;
	newDir.Z += zspread;
	return newDir;
}

EntityId createProjectileEntity(vector3df spawnPos, vector3df direction, EntityId weaponId)
{
	Scene* scene = &sceneManager->scene;

	auto wepInfo = scene->get<WeaponInfoComponent>(weaponId);
	auto shipParent = scene->get<ParentComponent>(weaponId);

	if (!wepInfo || !shipParent) {
		std::cout << "Cannot fire! ";
		if (!wepInfo) std::cout << "Weapon info ";
		if (!shipParent) std::cout << "Parent info ";
		std::cout << "is NULL\n";
		return INVALID_ENTITY;
	}

	auto shipRBC = scene->get<BulletRigidBodyComponent>(shipParent->parentId);

	auto projectileEntity = scene->newEntity();

	auto projectileInfo = addProjectileInfo(projectileEntity, wepInfo, spawnPos);
	addProjectileParent(projectileEntity, weaponId);

	btVector3 initialForce(0, 0, 0);
	vector3df initialDir = direction;
	btVector3 initVelocity = shipRBC->rigidBody.getLinearVelocity();
	btQuaternion initRot = shipRBC->rigidBody.getOrientation();

	if (projectileInfo->type == WEP_PLASMA) {
		initialForce += irrVecToBt(initialDir) * projectileInfo->speed;
		createPlasmaProjectile(projectileEntity, direction, spawnPos);
	}
	else if (projectileInfo->type == WEP_KINETIC) {
		auto kin = scene->get<KineticInfoComponent>(weaponId);
		initialDir = adjustAccuracy(direction, kin->accuracy);
		initialForce += irrVecToBt(initialDir) * projectileInfo->speed;
		createKineticProjectile(projectileEntity, initialDir, wepInfo->spawnPosition);
	}
	else if (projectileInfo->type == WEP_MISSILE) {
		createMissileProjectile(projectileEntity, sceneManager->scene.get<MissileInfoComponent>(weaponId), initialDir, spawnPos);
	}
	else if (projectileInfo->type == WEP_PHYS_IMPULSE) {
		initialForce += irrVecToBt(initialDir) * projectileInfo->speed;
		createPlasmaProjectile(projectileEntity, initialDir, spawnPos);
	}
	auto rbc = addProjectileRBC(projectileEntity, initialForce, initVelocity, spawnPos, initRot);

	if (wepInfo->type == WEP_KINETIC) {
		auto kin = scene->get<KineticInfoComponent>(weaponId);
		for (u32 i = 1; i < kin->projectilesPerShot; ++i) {
			vector3df dir = adjustAccuracy(direction, kin->accuracy);
			btVector3 force = irrVecToBt(dir) * projectileInfo->speed;
			EntityId newId = scene->newEntity();
			addProjectileInfo(newId, wepInfo, spawnPos);
			addProjectileParent(newId, weaponId);
			auto newRBC = addProjectileRBC(newId, force, initVelocity, spawnPos, initRot);
			createKineticProjectile(newId, dir, spawnPos);
			bWorld->addRigidBody(&newRBC->rigidBody);
		}
	}

	bWorld->addRigidBody(&rbc->rigidBody);
	return projectileEntity;
}

void createKineticProjectile(EntityId projId, vector3df dir, vector3df spawn)
{
	auto irr = sceneManager->scene.assign<IrrlichtComponent>(projId);
	auto wepPar = sceneManager->scene.get<ParentComponent>(projId);
	auto wepComp = sceneManager->scene.get<WeaponInfoComponent>(wepPar->parentId);

	irr->node = smgr->addLightSceneNode(0, spawn, SColorf(.8f, .8f, .1f, .5f), 5.f);
	irr->name = "bullet";
	ISceneNode* bill = smgr->addBillboardSceneNode(irr->node, dimension2d<f32>(1.f, 1.f));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, wepComp->particle);
	irr->node->setID(ID_IsNotSelectable);
	irr->node->setName(idToStr(projId).c_str());
	bill->setID(ID_IsNotSelectable);
}

void createPlasmaProjectile(EntityId projId, vector3df dir, vector3df spawn)
{
	auto irr = sceneManager->scene.assign<IrrlichtComponent>(projId);
	auto wepPar = sceneManager->scene.get<ParentComponent>(projId);
	auto wepComp = sceneManager->scene.get<WeaponInfoComponent>(wepPar->parentId);

	//this needs to be abstracted out to creating different types of node, for now it's just the laser with a crappy particle
	irr->node = smgr->addLightSceneNode(0, spawn, SColorf(.8f, .2f, .2f), 30.f);
	irr->name = "plasma ball";
	ISceneNode* bill = smgr->addBillboardSceneNode(irr->node, dimension2d<f32>(3.f, 3.f));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, wepComp->particle);

	irr->node->setID(ID_IsNotSelectable);
	irr->node->setName(idToStr(projId).c_str());
	bill->setID(ID_IsNotSelectable);

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, irr->node);
	ps->setID(ID_IsNotSelectable);
	IParticleEmitter* em = ps->createSphereEmitter(ps->getPosition(), .5f, //spawn point and radius
		(-dir * .01f), 30, 60, //direction, emit rate min/max
		SColor(0, 100, 50, 50), SColor(0, 255, 100, 100), 500, 2000, 0, //min / max color, shortest lifetime, longest lifetime, angle
		dimension2df(.1f, .1f), dimension2df(1.f, 1.f)); //min / max size
	ps->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, wepComp->particle);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
}

void createMissileProjectile(EntityId projId, MissileInfoComponent* missInfo, vector3df dir, vector3df spawn)
{
	auto wepParent = sceneManager->scene.get<ParentComponent>(projId);
	auto wepComp = sceneManager->scene.get<WeaponInfoComponent>(wepParent->parentId);
	auto missComp = sceneManager->scene.get<MissileInfoComponent>(wepParent->parentId);

	auto shipParent = sceneManager->scene.get<ParentComponent>(wepParent->parentId);
	auto sensors = sceneManager->scene.get<SensorComponent>(shipParent->parentId);
	if (!sensors) return; // need a similar check for AI component
	if (sensors && sensors->targetContact == INVALID_ENTITY) {
		std::cout << "No entity is currently selected!\n";
	}

	auto irr = sceneManager->scene.assign<IrrlichtComponent>(projId);

	auto irrship = sceneManager->scene.get<IrrlichtComponent>(shipParent->parentId);
	vector3df rot = irrship->node->getRotation();
	irr->node = smgr->addMeshSceneNode(missComp->missileMesh, 0, ID_IsNotSelectable, spawn, rot, vector3df(.2f, .2f, .2f));
	irr->name = "missile";
	irr->node->setName(idToStr(projId).c_str());

	auto missile = sceneManager->scene.assign<MissileProjectileComponent>(projId);
	if (sensors && missInfo->timeToLock <= sensors->timeSelected) {
		missile->target = sensors->targetContact;
	} else {
		std::cout << "Sensors unavailable or lock-time not established!\n";
		missile->target = INVALID_ENTITY;
	}
	auto ai = sceneManager->scene.get<AIComponent>(shipParent->parentId);
	if (ai) {
		//set the target to whatever the AI is targeting
	}
	//hardcoded missile stuff for now
	missile->maxVelocity = missInfo->maxMissileVelocity;
	missile->rotThrust = missInfo->missileRotThrust;

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, irr->node);
	ps->setID(ID_IsNotSelectable);
	IParticleEmitter* em = ps->createSphereEmitter(ps->getPosition(), .5f, //spawn point and radius
		(-dir * .00001f), 50, 100, //direction, emit rate min/max
		SColor(0, 100, 100, 100), SColor(0, 255, 200, 200), 500, 2000, 0, //min / max color, shortest lifetime, longest lifetime, angle
		dimension2df(.1f, .1f), dimension2df(1.f, 1.f)); //min / max size
	ps->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, wepComp->particle);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
}

void destroyProjectile(EntityId projectile)
{
	Scene* scene = &sceneManager->scene;

	auto rigidBodyInfo = scene->get<BulletRigidBodyComponent>(projectile);
	if (rigidBodyInfo) {
		bWorld->removeRigidBody(&rigidBodyInfo->rigidBody); //removes the rigid body from the bullet physics
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

EntityId projectileImpact(vector3df position, f32 duration)
{
	EntityId id = sceneManager->scene.newEntity();
	auto explodeinfo = sceneManager->scene.assign<ExplosionComponent>(id);
	explodeinfo->duration = duration;
	explodeinfo->lifetime = 0;
	explodeinfo->explosion = smgr->addParticleSystemSceneNode(true, 0, ID_IsNotSelectable, position);
	auto em = explodeinfo->explosion->createSphereEmitter(vector3df(0, 0, 0), .2f, vector3df(0.01f, 0.f, 0.f), 50, 100, SColor(0, 255, 255, 255), SColor(0, 255, 255, 255),
		50, 100, 360, dimension2df(1.f, 1.f), dimension2df(1.5f, 1.5f));
	explodeinfo->explosion->setEmitter(em);
	em->drop();
	IParticleAffector* paf = explodeinfo->explosion->createFadeOutParticleAffector(SColor(0, 0, 0, 0), 100);
	explodeinfo->explosion->addAffector(paf);
	paf->drop();
	explodeinfo->explosion->setMaterialFlag(EMF_LIGHTING, false);
	explodeinfo->explosion->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	explodeinfo->explosion->setMaterialTexture(0, stateController->assets.getTextureAsset("defaultProjectileTexture"));
	explodeinfo->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	explodeinfo->light = nullptr;

	explodeinfo->damage = 0;
	explodeinfo->radius = 1.f;
	explodeinfo->force = 0;
	return id;
}

void missileGoTo(EntityId id, f32 dt)
{
	auto miss = sceneManager->scene.get<MissileProjectileComponent>(id);
	auto proj = sceneManager->scene.get<ProjectileInfoComponent>(id);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
	if (!miss || !proj || !rbc) return;
	btRigidBody* body = &rbc->rigidBody;

	btVector3 torque(0, 0, 0);
	btVector3 force(0, 0, 0);
	btVector3 forward = getRigidBodyForward(body);

	force += forward * proj->speed;
	body->applyCentralImpulse(force * dt);

	if(!sceneManager->scene.entityInUse(miss->target)) return;

	auto targetirr = sceneManager->scene.get<IrrlichtComponent>(miss->target);

	btVector3 path = irrVecToBt(targetirr->node->getPosition()) - body->getCenterOfMassPosition();
	btVector3 dir = path.normalized();

	//turn towards target
	btVector3 right = getRigidBodyRight(body);
	btVector3 left = getRigidBodyLeft(body);
	btVector3 up = getRigidBodyUp(body);
	btVector3 down = getRigidBodyDown(body);

	btScalar angle = forward.angle(dir);
	btVector3 ang = body->getAngularVelocity();

	if (angle <= ang.length()) {
		btVector3 angNorm = velocitySafeNormalize(ang);
		if (ang.length2() >= DEGENERATE_VECTOR_LENGTH) {
			torque += -ang;
		}
		else {
			btVector3 stopTorque = -angNorm * miss->rotThrust;
			torque += stopTorque;

		}
	} else {
		if (right.dot(dir) > left.dot(dir)) {
			torque += up * miss->rotThrust;
		} else {
			torque += down * miss->rotThrust;
		}
		if (up.dot(dir) > down.dot(dir)) {
			torque += left * miss->rotThrust;
		} else {
			torque += right * miss->rotThrust;
		}
	}

	body->applyTorqueImpulse(torque * dt);
}

BulletRigidBodyComponent* addProjectileRBC(EntityId id, btVector3& initForce, btVector3& initVelocity, vector3df& spawn, btQuaternion& initRot)
{
	f32 mass = .1f;
	auto rbc = sceneManager->scene.assign<BulletRigidBodyComponent>(id);
	btTransform transform = btTransform();
	transform.setIdentity();

	transform.setRotation(initRot);
	transform.setOrigin(irrVecToBt(spawn));

	auto motionState = new btDefaultMotionState(transform);

	rbc->sphere = btSphereShape(.5f);
	btVector3 localInertia;

	rbc->sphere.calculateLocalInertia(mass, localInertia);
	rbc->rigidBody = btRigidBody(mass, motionState, &rbc->sphere, localInertia);
	rbc->rigidBody.setUserIndex(getEntityIndex(id));
	rbc->rigidBody.setUserIndex2(getEntityVersion(id));
	rbc->rigidBody.setUserIndex3(1);
	rbc->rigidBody.setLinearVelocity(initVelocity);
	rbc->rigidBody.applyCentralImpulse(initForce);
	return rbc;
}

ParentComponent* addProjectileParent(EntityId id, EntityId parent)
{
	auto par = sceneManager->scene.assign<ParentComponent>(id);
	par->parentId = parent;
	return par;
}

ProjectileInfoComponent* addProjectileInfo(EntityId id, WeaponInfoComponent* wepInfo, vector3df spawnPos)
{
	auto projectileInfo = sceneManager->scene.assign<ProjectileInfoComponent>(id);
	projectileInfo->type = wepInfo->type;
	projectileInfo->dmgtype = wepInfo->dmgtype;
	projectileInfo->speed = wepInfo->projectileSpeed;
	projectileInfo->startPos = spawnPos;
	projectileInfo->range = wepInfo->range;
	projectileInfo->damage = wepInfo->damage;
	return projectileInfo;
}