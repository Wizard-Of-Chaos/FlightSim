#include "ProjectileUtils.h"
#include "GameController.h"
#include "SceneManager.h"

EntityId createProjectileEntity(SceneManager* manager, vector3df spawnPos, vector3df direction, EntityId weaponId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto wepInfo = scene->get<WeaponInfoComponent>(weaponId);

	if (!wepInfo) return INVALID_ENTITY;

	auto projectileEntity = scene->newEntity();

	auto parent = scene->assign<ParentComponent>(projectileEntity);
	parent->parentId = weaponId;

	auto projectileInfo = scene->assign<ProjectileInfoComponent>(projectileEntity);
	projectileInfo->type = wepInfo->type;
	projectileInfo->speed = wepInfo->projectileSpeed;
	projectileInfo->startPos = spawnPos;
	projectileInfo->range = wepInfo->range;
	projectileInfo->damage = wepInfo->damage;

	switch (projectileInfo->type) {
	case WEP_PLASMA:
		createPlasmaProjectile(manager, projectileEntity, direction, spawnPos);
		break;
	case WEP_GRAPPLE:
		break;
	case WEP_MISSILE:
		createMissileProjectile(manager, projectileEntity, direction, spawnPos);
		break;
	}
	auto rigidBodyInfo = scene->assign<BulletRigidBodyComponent>(projectileEntity);
	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(irrVecToBt(spawnPos));

	auto motionState = new btDefaultMotionState(transform);

	auto shape = new btSphereShape(.5f);
	btVector3 localInertia;
	f32 mass = .1f;
	shape->calculateLocalInertia(mass, localInertia);
	rigidBodyInfo->rigidBody = btRigidBody(mass, motionState, shape, localInertia);

	rigidBodyInfo->rigidBody.applyCentralImpulse(irrVecToBt(direction) * projectileInfo->speed);

	rigidBodyInfo->rigidBody.setUserIndex(getEntityIndex(projectileEntity));
	rigidBodyInfo->rigidBody.setUserIndex2(getEntityVersion(projectileEntity));
	rigidBodyInfo->rigidBody.setUserIndex3(1);

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

void createMissileProjectile(SceneManager* manager, EntityId projId, vector3df dir, vector3df spawn)
{
	auto irr = manager->scene.assign<IrrlichtComponent>(projId);

	auto wepParent = manager->scene.get<ParentComponent>(projId);
	auto irrp = manager->scene.get<IrrlichtComponent>(wepParent->parentId);
	irr->node = manager->controller->smgr->addMeshSceneNode(manager->defaults.defaultWeaponMesh, 0, ID_IsNotSelectable, spawn, irrp->node->getRotation(), vector3df(.2f, .2f, .2f));
	irr->name = "missile";
	irr->node->setName(idToStr(projId).c_str());

	auto missile = manager->scene.assign<MissileComponent>(projId); //nothing to do with this yet; need to load missile stats

	auto shipParent = manager->scene.get<ParentComponent>(wepParent->parentId);

	auto pc = manager->scene.get<PlayerComponent>(shipParent->parentId);
	if (pc) {
		missile->target = pc->activeSelection;
	}
	auto ai = manager->scene.get<AIComponent>(shipParent->parentId);
	if (ai) {
		//set the target to whatever the AI is targeting
	}
	//hardcoded missile stuff for now
	missile->forwardThrust = 100.f;
	missile->maxVelocity = 130.f;
	missile->rotThrust = 90.f;
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

void missileGoTo(btRigidBody* body, MissileComponent* miss, btVector3 dest, f32 dt)
{
	btVector3 force(0, 0, 0);
	btVector3 torque(0, 0, 0);
	btVector3 forward = getRigidBodyForward(body);

	btVector3 path = dest - body->getCenterOfMassPosition();
	btVector3 dir = path.normalized();

	btScalar angle = forward.angle(dir);
	btVector3 angVel = body->getAngularVelocity();

	if (angle <= angVel.length()) {
		btVector3 angNorm = velocitySafeNormalize(angVel);
		if (angNorm.length2() <= 0.00001f) torque += -angNorm;
		else torque += -angNorm * miss->rotThrust;
	}
	else {
		btVector3 right = getRigidBodyRight(body);
		btVector3 left = getRigidBodyLeft(body);
		btVector3 up = getRigidBodyUp(body);
		btVector3 down = getRigidBodyDown(body);
		if (right.dot(dir) > left.dot(dir)) {
			torque += up * miss->rotThrust;
		}
		else {
			torque += down * miss->rotThrust;
		}
		if (up.dot(dir) > down.dot(dir)) {
			torque += left * miss->rotThrust;
		}
		else {
			torque += right * miss->rotThrust;
		}
	}
	if (angle * RADTODEG >= 90 || body->getLinearVelocity().length() < miss->maxVelocity) {
		force += forward * miss->forwardThrust;
	}

	body->applyCentralImpulse(force * dt);
	body->applyTorqueImpulse(torque * dt);
}