#include "ProjectileUtils.h"
#include "GameController.h"
#include "SceneManager.h"
#include <iostream>

EntityId createProjectileEntity(SceneManager* manager, vector3df spawnPos, vector3df direction, EntityId weaponId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

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

	auto parent = scene->assign<ParentComponent>(projectileEntity);
	parent->parentId = weaponId;

	auto projectileInfo = scene->assign<ProjectileInfoComponent>(projectileEntity);
	projectileInfo->type = wepInfo->type;
	projectileInfo->speed = wepInfo->projectileSpeed;
	projectileInfo->startPos = spawnPos;
	projectileInfo->range = wepInfo->range;
	projectileInfo->damage = wepInfo->damage;

	f32 mass = .1f;

	auto rigidBodyInfo = scene->assign<BulletRigidBodyComponent>(projectileEntity);
	btTransform transform = btTransform();
	transform.setIdentity();

	transform.setRotation(shipRBC->rigidBody.getOrientation());
	transform.setOrigin(irrVecToBt(spawnPos));

	auto motionState = new btDefaultMotionState(transform);

	auto shape = new btSphereShape(.5f);
	btVector3 localInertia;

	shape->calculateLocalInertia(mass, localInertia);
	rigidBodyInfo->rigidBody = btRigidBody(mass, motionState, shape, localInertia);

	if (projectileInfo->type == WEP_PLASMA) rigidBodyInfo->rigidBody.applyCentralImpulse(irrVecToBt(direction) * projectileInfo->speed);

	rigidBodyInfo->rigidBody.setUserIndex(getEntityIndex(projectileEntity));
	rigidBodyInfo->rigidBody.setUserIndex2(getEntityVersion(projectileEntity));
	rigidBodyInfo->rigidBody.setUserIndex3(1);

	switch (projectileInfo->type) {
	case WEP_PLASMA:
		createPlasmaProjectile(manager, projectileEntity, direction, spawnPos);
		break;
	case WEP_GRAPPLE:
		break;
	case WEP_MISSILE:
		mass = .3f;
		auto missInfo = manager->scene.get<MissileInfoComponent>(weaponId);
		createMissileProjectile(manager, projectileEntity, missInfo, direction, spawnPos);
		break;
	}

	manager->bulletWorld->addRigidBody(&rigidBodyInfo->rigidBody);

	return projectileEntity;
}

void createPlasmaProjectile(SceneManager* manager, EntityId projId, vector3df dir, vector3df spawn)
{
	auto irr = manager->scene.assign<IrrlichtComponent>(projId);

	ISceneManager* smgr = manager->controller->smgr;
	//this needs to be abstracted out to creating different types of node, for now it's just the laser with a crappy particle
	irr->node = smgr->addLightSceneNode(0, spawn, SColorf(.8f, .2f, .2f), 30.f);
	irr->name = "plasma ball";
	ISceneNode* bill = smgr->addBillboardSceneNode(irr->node, dimension2d<f32>(3.f, 3.f));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, manager->defaults.defaultProjectileTexture);

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
	ps->setMaterialTexture(0, manager->defaults.defaultProjectileTexture);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
}

void createMissileProjectile(SceneManager* manager, EntityId projId, MissileInfoComponent* missInfo, vector3df dir, vector3df spawn)
{
	auto wepParent = manager->scene.get<ParentComponent>(projId);
	auto shipParent = manager->scene.get<ParentComponent>(wepParent->parentId);
	auto sensors = manager->scene.get<SensorComponent>(shipParent->parentId);
	if (!sensors) return; // need a similar check for AI component
	if (sensors && sensors->targetContact == INVALID_ENTITY) {
		std::cout << "No entity is currently selected!\n";
	}
	ISceneManager* smgr = manager->controller->smgr;

	auto irr = manager->scene.assign<IrrlichtComponent>(projId);

	auto irrship = manager->scene.get<IrrlichtComponent>(shipParent->parentId);
	vector3df rot = irrship->node->getRotation();
	irr->node = manager->controller->smgr->addMeshSceneNode(manager->defaults.defaultMissileMesh, 0, ID_IsNotSelectable, spawn, rot, vector3df(.2f, .2f, .2f));
	irr->name = "missile";
	irr->node->setName(idToStr(projId).c_str());

	auto missile = manager->scene.assign<MissileProjectileComponent>(projId);
	if (sensors) {
		missile->target = sensors->targetContact;

	}
	auto ai = manager->scene.get<AIComponent>(shipParent->parentId);
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
	ps->setMaterialTexture(0, manager->defaults.defaultJetTexture);
	ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
}

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

EntityId projectileImpact(SceneManager* manager, vector3df position, f32 duration)
{
	EntityId id = manager->scene.newEntity();
	auto explodeinfo = manager->scene.assign<ExplosionComponent>(id);
	explodeinfo->duration = duration;
	explodeinfo->lifetime = 0;
	explodeinfo->explosion = manager->controller->smgr->addParticleSystemSceneNode(true, 0, ID_IsNotSelectable, position);
	auto em = explodeinfo->explosion->createSphereEmitter(vector3df(0, 0, 0), .2f, vector3df(0.01f, 0.f, 0.f), 50, 100, SColor(0, 255, 255, 255), SColor(0, 255, 255, 255),
		50, 100, 360, dimension2df(1.f, 1.f), dimension2df(1.5f, 1.5f));
	explodeinfo->explosion->setEmitter(em);
	em->drop();
	IParticleAffector* paf = explodeinfo->explosion->createFadeOutParticleAffector(SColor(0, 0, 0, 0), 100);
	explodeinfo->explosion->addAffector(paf);
	paf->drop();
	explodeinfo->explosion->setMaterialFlag(EMF_LIGHTING, false);
	explodeinfo->explosion->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	explodeinfo->explosion->setMaterialTexture(0, manager->defaults.defaultProjectileTexture);
	explodeinfo->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	return id;
}

void missileGoTo(SceneManager* manager, EntityId id, f32 dt)
{
	auto miss = manager->scene.get<MissileProjectileComponent>(id);
	auto proj = manager->scene.get<ProjectileInfoComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	if (!miss || !proj || !rbc) return;
	btRigidBody* body = &rbc->rigidBody;

	btVector3 torque(0, 0, 0);
	btVector3 force(0, 0, 0);
	btVector3 forward = getRigidBodyForward(body);

	force += forward * proj->speed;
	body->applyCentralImpulse(force * dt);

	if(!manager->scene.entityInUse(miss->target)) return;

	auto targetirr = manager->scene.get<IrrlichtComponent>(miss->target);

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