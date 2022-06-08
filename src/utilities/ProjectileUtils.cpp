#include "ProjectileUtils.h"
#include "GameController.h"
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

flecs::entity createProjectileEntity(vector3df spawnPos, vector3df direction, flecs::entity weaponId)
{
	if (!weaponId.has<WeaponInfoComponent>() || weaponId.has_relation(flecs::ChildOf)) {
		std::cout << "Cannot fire! ";
		if (!weaponId.has<WeaponInfoComponent>()) std::cout << "Weapon info ";
		if (!weaponId.has_relation(flecs::ChildOf)) std::cout << "Parent info ";
		std::cout << "is NULL\n";
		return INVALID_ENTITY;
	}
	auto wepInfo = weaponId.get_mut<WeaponInfoComponent>();

	auto shipRBC = weaponId.get_object(flecs::ChildOf).get<BulletRigidBodyComponent>();

	auto projectileEntity = game_world->entity();

	auto projectileInfo = addProjectileInfo(wepInfo, spawnPos);
	auto projInfoC = projectileEntity.get_mut<ProjectileInfoComponent>(projectileEntity);
	*projInfoC = projectileInfo;

	projectileEntity.add<FiredBy>(weaponId);

	btVector3 initialForce(0, 0, 0);
	vector3df initialDir = direction;
	btVector3 initVelocity = shipRBC->rigidBody.getLinearVelocity();
	btQuaternion initRot = shipRBC->rigidBody.getOrientation();

	if (projectileInfo.type == WEP_PLASMA) {
		initialForce += irrVecToBt(initialDir) * projectileInfo.speed;
		createPlasmaProjectile(projectileEntity, direction, spawnPos);
	}
	else if (projectileInfo.type == WEP_KINETIC) {
		auto kin = weaponId.get<KineticInfoComponent>();
		initialDir = adjustAccuracy(direction, kin->accuracy);
		initialForce += irrVecToBt(initialDir) * projectileInfo.speed;
		createKineticProjectile(projectileEntity, initialDir, wepInfo->spawnPosition);
	}
	else if (projectileInfo.type == WEP_MISSILE) {
		createMissileProjectile(projectileEntity, initialDir, spawnPos);
	}
	else if (projectileInfo.type == WEP_PHYS_IMPULSE || projectileInfo.type == WEP_PHYS_BOLAS) {
		initialForce += irrVecToBt(initialDir) * projectileInfo.speed;
		createPlasmaProjectile(projectileEntity, initialDir, spawnPos);
	}
	auto rbc = addProjectileRBC(projectileEntity, initialForce, initVelocity, spawnPos, initRot);

	if (wepInfo->type == WEP_KINETIC) {
		auto kin = weaponId.get<KineticInfoComponent>();
		for (u32 i = 1; i < kin->projectilesPerShot; ++i) {
			vector3df dir = adjustAccuracy(direction, kin->accuracy);
			btVector3 force = irrVecToBt(dir) * projectileInfo.speed;
			flecs::entity newId = game_world->entity();
			newId.add<ProjectileInfoComponent>(projectileInfo);
			auto newRBC = addProjectileRBC(newId, force, initVelocity, spawnPos, initRot);
			createKineticProjectile(newId, dir, spawnPos);
			bWorld->addRigidBody(&newRBC->rigidBody);
		}
	}

	bWorld->addRigidBody(&rbc->rigidBody);
	return projectileEntity;
}

void createKineticProjectile(flecs::entity projId, vector3df dir, vector3df spawn)
{
	auto wepComp = projId.get_object<FiredBy>().get<WeaponInfoComponent>();

	IrrlichtComponent irr;

	irr.node = smgr->addLightSceneNode(0, spawn, SColorf(.8f, .8f, .1f, .5f), 5.f);
	irr.name = "bullet";
	ISceneNode* bill = smgr->addBillboardSceneNode(irr.node, dimension2d<f32>(1.f, 1.f));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, wepComp->particle);
	irr.node->setID(ID_IsNotSelectable);
	irr.node->setName(idToStr(projId).c_str());
	bill->setID(ID_IsNotSelectable);

	projId.add<IrrlichtComponent>(irr);
}

void createPlasmaProjectile(flecs::entity projId, vector3df dir, vector3df spawn)
{
	auto wepComp = projId.get_object<FiredBy>().get<WeaponInfoComponent>();

	IrrlichtComponent irr;

	irr.node = smgr->addLightSceneNode(0, spawn, SColorf(.8f, .2f, .2f), 30.f);
	irr.name = "plasma ball";
	ISceneNode* bill = smgr->addBillboardSceneNode(irr.node, dimension2d<f32>(3.f, 3.f));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, wepComp->particle);

	irr.node->setID(ID_IsNotSelectable);
	irr.node->setName(idToStr(projId).c_str());
	bill->setID(ID_IsNotSelectable);

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, irr.node);
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

	projId.add<IrrlichtComponent>(irr);
}

void createMissileProjectile(flecs::entity projId, vector3df dir, vector3df spawn)
{
	auto wepId = projId.get_object<FiredBy>();
	auto wepComp = wepId.get<WeaponInfoComponent>();
	auto missComp = wepId.get<MissileInfoComponent>();

	auto sensors = wepId.get_object(flecs::ChildOf).get<SensorComponent>();

	if (!sensors) return; // need a similar check for AI component

	if (sensors->targetContact == INVALID_ENTITY) {
		std::cout << "No entity is currently selected!\n";
	}

	IrrlichtComponent irr;

	auto irrship = wepId.get_object(flecs::ChildOf).get<IrrlichtComponent>();

	vector3df rot = irrship->node->getRotation();
	irr.node = smgr->addMeshSceneNode(missComp->missileMesh, 0, ID_IsNotSelectable, spawn, rot, vector3df(.2f, .2f, .2f));
	irr.name = "missile";
	irr.node->setName(idToStr(projId).c_str());

	MissileProjectileComponent missile;

	if (missComp->timeToLock <= sensors->timeSelected) {
		missile.target = sensors->targetContact;
	} else {
		std::cout << "Sensors unavailable or lock-time not established!\n";
		missile.target = INVALID_ENTITY;
	}
	if (wepId.get_object(flecs::ChildOf).has<AIComponent>()) {
		//make the AI able to fire missiles
	}

	missile.maxVelocity = missComp->maxMissileVelocity;
	missile.rotThrust = missComp->missileRotThrust;

	projId.add<MissileProjectileComponent>(missile);

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, irr.node);
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

	projId.add<IrrlichtComponent>(irr);
}

void destroyProjectile(flecs::entity projectile)
{
	if (projectile.has<BulletRigidBodyComponent>()) {
		auto rbc = projectile.get_mut<BulletRigidBodyComponent>();
		bWorld->removeRigidBody(&rbc->rigidBody); //removes the rigid body from the bullet physics
	}

	if (projectile.has<IrrlichtComponent>()) {
		auto irrComp = projectile.get_mut<IrrlichtComponent>();
		irrComp->node->removeAll(); //removes the children of the node (like particles, billboards etc)
		irrComp->node->remove(); // removes the node
	}
	projectile.destruct(); //bye bye your life goodbye
}

flecs::entity createProjectileImpactEffect(vector3df position, f32 duration)
{
	auto id = game_world->entity();

	id.add<ExplosionComponent>();
	auto explodeinfo = id.get_mut<ExplosionComponent>();

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

void missileGoTo(flecs::entity id, f32 dt)
{
	if (!id.is_alive()) return;
	if (!id.has<MissileProjectileComponent>() || !id.has<ProjectileInfoComponent>() || !id.has<BulletRigidBodyComponent>()) return;

	auto rbc = id.get_mut<BulletRigidBodyComponent>();
	auto miss = id.get_mut<MissileProjectileComponent>();
	auto proj = id.get_mut<ProjectileInfoComponent>();

	btRigidBody* body = &rbc->rigidBody;

	btVector3 torque(0, 0, 0);
	btVector3 force(0, 0, 0);
	btVector3 forward = getRigidBodyForward(body);

	force += forward * proj->speed;
	body->applyCentralImpulse(force * dt);

	if(!miss->target.is_alive()) return;

	auto targetirr = miss->target.get<IrrlichtComponent>();

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

BulletRigidBodyComponent* addProjectileRBC(flecs::entity id, btVector3& initForce, btVector3& initVelocity, vector3df& spawn, btQuaternion& initRot)
{
	f32 mass = .1f;
	auto rbc = id.get_mut<BulletRigidBodyComponent>();

	btTransform transform = btTransform();
	transform.setIdentity();

	transform.setRotation(initRot);
	transform.setOrigin(irrVecToBt(spawn));

	auto motionState = new btDefaultMotionState(transform);

	rbc->sphere = btSphereShape(.5f);
	btVector3 localInertia;

	rbc->sphere.calculateLocalInertia(mass, localInertia);
	rbc->rigidBody = btRigidBody(mass, motionState, &rbc->sphere, localInertia);
	setIdOnBtObject(&rbc->rigidBody, id);

	rbc->rigidBody.setLinearVelocity(initVelocity);
	rbc->rigidBody.applyCentralImpulse(initForce);
	return rbc;
}

ProjectileInfoComponent addProjectileInfo(WeaponInfoComponent* wepInfo, vector3df spawnPos)
{
	ProjectileInfoComponent projectileInfo;
	projectileInfo.type = wepInfo->type;
	projectileInfo.dmgtype = wepInfo->dmgtype;
	projectileInfo.speed = wepInfo->projectileSpeed;
	projectileInfo.startPos = spawnPos;
	projectileInfo.lifetime = wepInfo->lifetime;
	projectileInfo.currentLifetime = 0.f;
	projectileInfo.damage = wepInfo->damage;

	return projectileInfo;
}