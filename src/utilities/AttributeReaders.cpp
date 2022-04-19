#include "AttributeReaders.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include <iostream>

vector3df strToVec(std::string str) //Turns a string to a vector, splitting on a , character
{
	std::string xstr, ystr, zstr;

	std::stringstream split(str);
	std::getline(split, xstr, ',');
	std::getline(split, ystr, ',');
	std::getline(split, zstr, ',');
	return vector3df(std::stof(xstr), std::stof(ystr), std::stof(zstr));
}

u32 loadShipData(std::string path, gvReader& in)
{
	std::cout << "Reading ship in from: " << path << "... ";
	in.read(path);
	if (in.lines.empty()) {
		std::cout << "Could not read " << path << "!\n";
		return -1;
	}
	in.readLinesToValues();
	u32 id = std::stoi(in.values["id"]);
	std::string name = in.values["name"];
	ShipData* data = new ShipData;

	data->id = id;
	data->name = name;
	data->description = in.values["description"];

	std::string meshpath = "models/" + in.values["model"];
	std::string texpath = "models/" + in.values["texture"];
	std::string normpath = "models/" + in.values["norm"];
	data->shipMesh = meshpath;
	data->shipTexture = texpath;
	data->shipNorm = normpath;
	/*
	data->shipNorm = driver->getTexture(normpath.c_str());
	if (data->shipNorm) {
		driver->makeNormalMapTexture(data->shipNorm, 7.f);
		IMesh* tmesh = smgr->getMeshManipulator()->createMeshWithTangents(data->shipMesh); //drop this somewhere
		smgr->getMeshCache()->removeMesh(data->shipMesh);
		data->shipMesh = tmesh;
	}
	*/

	std::string jetpath = "effects/" + in.values["jet"];
	std::string enginepath = "effects/" + in.values["engine"];

	data->engineTexture = enginepath;
	data->jetTexture = enginepath;

	data->shipComponent.forwardThrust = std::stof(in.values["forwardThrust"]);
	data->shipComponent.brakeThrust = std::stof(in.values["brakeThrust"]);
	data->shipComponent.strafeThrust = std::stof(in.values["strafeThrust"]);
	data->shipComponent.pitchThrust = std::stof(in.values["pitchThrust"]);
	data->shipComponent.yawThrust = std::stof(in.values["yawThrust"]);
	data->shipComponent.rollThrust = std::stof(in.values["rollThrust"]);

	data->shipComponent.hardpointCount = std::stoi(in.values["hardpointCount"]);

	std::string val;
	for (unsigned int i = 0; i < data->shipComponent.hardpointCount; ++i) {
		val = "hardpoint" + std::to_string(i);
		data->shipComponent.hardpoints[i] = strToVec(in.values[val]);
		data->shipComponent.weapons[i] = INVALID_ENTITY;
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "upJetPos" + std::to_string(i);
		data->shipComponent.upJetPos[i] = strToVec(in.values[val]);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "downJetPos" + std::to_string(i);
		data->shipComponent.downJetPos[i] = strToVec(in.values[val]);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "leftJetPos" + std::to_string(i);
		data->shipComponent.leftJetPos[i] = strToVec(in.values[val]);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "rightJetPos" + std::to_string(i);
		data->shipComponent.rightJetPos[i] = strToVec(in.values[val]);
	}
	for (u32 i = 0; i < 2; ++i) {
		val = "reverseJetPos" + std::to_string(i);
		data->shipComponent.reverseJetPos[i] = strToVec(in.values[val]);
	}
	data->shipComponent.engineJetPos = strToVec(in.values["engineJetPos"]);
	data->shipComponent.physWeaponHardpoint = strToVec(in.values["physWeaponHardpoint"]);

	data->shipComponent.velocityTolerance = std::stof(in.values["velocityTolerance"]);
	data->shipComponent.linearMaxVelocity = std::stof(in.values["linearMaxVelocity"]);
	data->shipComponent.angularMaxVelocity = std::stof(in.values["angularMaxVelocity"]);
	data->shipComponent.afterburnerThrust = std::stof(in.values["afterburnerThrust"]);
	data->shipComponent.afterburnerFuel = std::stof(in.values["afterburnerFuel"]);
	data->shipComponent.maxAfterburnerFuel = data->shipComponent.afterburnerFuel;
	data->shipComponent.afterburnerFuelEfficiency = std::stof(in.values["afterburnerFuelEfficiency"]);

	data->shipComponent.afterburnerOn = false;
	data->shipComponent.safetyOverride = false;

	stateController->shipData[id] = data;
	std::cout << "Done.\n";
	return id;
}

void loadAmmoData(WeaponData* data, gvReader& in)
{
	data->weaponComponent.maxAmmunition = std::stoi(in.values["maxAmmunition"]);
	data->weaponComponent.ammunition = data->weaponComponent.maxAmmunition;
	data->weaponComponent.maxClip = std::stoi(in.values["clip"]);
	data->weaponComponent.reloadTime = std::stof(in.values["reloadTime"]);
	data->weaponComponent.clip = data->weaponComponent.clip;

}

u32 loadWeaponData(std::string path, gvReader& in)
{
	std::cout << "Reading weapon in from: " << path << "... ";
	in.read(path);
	if (in.lines.empty()) {
		std::cout << "Could not read " << path << "!\n";
	}
	in.readLinesToValues();

	u32 id = std::stoi(in.values["id"]);
	std::string name = in.values["name"];
	WEAPON_TYPE type = (WEAPON_TYPE)std::stoi(in.values["type"]);
	DAMAGE_TYPE dmgtype = (DAMAGE_TYPE)std::stoi(in.values["dmgtype"]);
	WeaponData* data = new WeaponData;
	if (type == WEP_KINETIC) {
		delete data;
		data = new KineticData;
	}
	else if (type == WEP_MISSILE) {
		delete data;
		data = new MissileData;
	}
	data->id = id;
	data->name = name;
	data->description = in.values["description"];
	std::string meshpath = "models/" + in.values["model"];
	std::string texpath = "models/" + in.values["texture"];
	std::string normpath = "models/" + in.values["norm"];

	data->weaponMesh = meshpath;
	data->weaponTexture = texpath;
	data->weaponNorm = normpath;

	/*
	if (data->weaponNorm) {
		driver->makeNormalMapTexture(data->weaponNorm, 7.f);
		IMesh* tmesh = smgr->getMeshManipulator()->createMeshWithTangents(data->weaponMesh); //drop this somewhere
		smgr->getMeshCache()->removeMesh(data->weaponMesh);
		data->weaponMesh = tmesh;
	}
	*/
	std::string effectpath = "effects/" + in.values["particle"];
	data->weaponEffect = effectpath;
	data->weaponComponent.usesAmmunition = false;
	data->weaponComponent.maxAmmunition = 0;
	data->weaponComponent.ammunition = 0;
	data->weaponComponent.maxClip = 0;
	data->weaponComponent.reloadTime = 0;
	data->weaponComponent.clip = 0;
	data->weaponComponent.timeReloading = 0;

	if (type == WEP_MISSILE) {
		data->weaponComponent.usesAmmunition = true;
		std::string misspath = "models/" + in.values["missilemodel"];
		std::string misstexpath = "models/" + in.values["missiletexture"];
		MissileData* miss = (MissileData*)data;

		miss->missileMesh = misspath;
		miss->missileTexture = misstexpath;
		miss->missileComponent.maxMissileVelocity = std::stof(in.values["maxMissileVelocity"]);
		miss->missileComponent.missileRotThrust = std::stof(in.values["missileRotThrust"]);
		miss->missileComponent.timeToLock = std::stof(in.values["timeToLock"]);
		loadAmmoData(data, in);
	}
	if (type == WEP_KINETIC) {
		data->weaponComponent.usesAmmunition = true;
		KineticData* kin = (KineticData*)data;

		loadAmmoData(data, in);
		kin->kineticComponent.accuracy = std::stof(in.values["accuracy"]);
		kin->kineticComponent.projectilesPerShot = std::stoi(in.values["projectilesPerShot"]);
	}

	data->weaponComponent.isFiring = false;
	data->weaponComponent.type = type;
	data->weaponComponent.dmgtype = dmgtype;
	data->weaponComponent.firingSpeed = std::stof(in.values["firingSpeed"]);
	data->weaponComponent.projectileSpeed = std::stof(in.values["projectileSpeed"]);
	data->weaponComponent.damage = std::stof(in.values["damage"]);
	data->weaponComponent.range = std::stof(in.values["range"]);
	data->weaponComponent.timeSinceLastShot = 0.f;

	bool phys = std::stoi(in.values["phys"]);
	if (id == 0) {
		stateController->weaponData[id] = data;
		stateController->physWeaponData[id] = data;
	} else if (phys) {
		stateController->physWeaponData[id] = data;
	} else {
		stateController->weaponData[id] = data;
	}
	std::cout << "Done.\n";
	return id;
}

bool loadShip(u32 id, EntityId entity)
{
	ShipData* data = stateController->shipData[id];
	if (!data) return false;

	auto ship = sceneManager->scene.assign<ShipComponent>(entity);
	auto irr = sceneManager->scene.assign<IrrlichtComponent>(entity);
	if (!irr || !ship) return false;
	*ship = data->shipComponent;

	ITexture* norm = driver->getTexture(data->shipNorm.c_str());

	if (norm) {
		irr->node = smgr->addMeshSceneNode(smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh(data->shipMesh.c_str())));
		driver->makeNormalMapTexture(norm, 7.f);
		irr->node->setMaterialTexture(1, norm);
		irr->node->setMaterialType(EMT_PARALLAX_MAP_SOLID);
	}
	else {
		irr->node = smgr->addMeshSceneNode(smgr->getMesh(data->shipMesh.c_str()));
	}
	irr->node->setMaterialTexture(0, driver->getTexture(data->shipTexture.c_str()));

	irr->node->setName(idToStr(entity).c_str());
	irr->node->setID(ID_IsSelectable | ID_IsAvoidable);

	return true;
}

bool loadWeapon(u32 id, EntityId weaponEntity, EntityId shipEntity, bool phys)
{
	WeaponData* data;
	if (phys) data = stateController->physWeaponData[id];
	else data = stateController->weaponData[id];

	if (!data) return false;

	auto wep = sceneManager->scene.assign<WeaponInfoComponent>(weaponEntity);
	auto irr = sceneManager->scene.assign<IrrlichtComponent>(weaponEntity);
	auto parent = sceneManager->scene.assign<ParentComponent>(weaponEntity);
	if (!wep || !irr || !parent) return false;
	
	parent->parentId = shipEntity;

	auto mesh = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh(data->weaponMesh.c_str()));
	irr->node = smgr->addMeshSceneNode(mesh);
	irr->node->setMaterialTexture(0, driver->getTexture(data->weaponTexture.c_str()));

	ITexture* norm = driver->getTexture(data->weaponNorm.c_str());
	if (norm) {
		driver->makeNormalMapTexture(norm, 7.f);
		irr->node->setMaterialTexture(1, norm);
		irr->node->setMaterialType(EMT_PARALLAX_MAP_SOLID);
	}

	irr->node->setName(idToStr(weaponEntity).c_str());
	irr->node->setID(ID_IsNotSelectable);
	wep->usesAmmunition = false;

	*wep = data->weaponComponent;
	if (data->weaponComponent.type == WEP_MISSILE) {
		auto miss = sceneManager->scene.assign<MissileInfoComponent>(weaponEntity);
		MissileData* mdata = (MissileData*)data;
		*miss = mdata->missileComponent;
		miss->missileMesh = smgr->getMesh(mdata->missileMesh.c_str());
		miss->missileTexture = driver->getTexture(mdata->missileTexture.c_str());
		wep->usesAmmunition = true;
	}
	if (data->weaponComponent.type == WEP_KINETIC) {
		auto kin = sceneManager->scene.assign<KineticInfoComponent>(weaponEntity);
		KineticData* kdata = (KineticData*)data;
		*kin = kdata->kineticComponent;
		wep->clip = wep->maxClip;
		wep->usesAmmunition = true; 
	}
	wep->particle = driver->getTexture(data->weaponEffect.c_str());

	return true; 
}

btConvexHullShape createCollisionShapeFromMesh(IMesh* mesh)
{
	IMeshBuffer* buf = mesh->getMeshBuffer((u32)0);
	S3DVertex* bufverts = (S3DVertex*)buf->getVertices();
	btConvexHullShape shape;

	for (u32 i = 0; i < buf->getVertexCount(); ++i) {
		vector3df pos = bufverts[i].Pos;
		shape.addPoint(btVector3(pos.X, pos.Y, pos.Z));
	}
	shape.setMargin(0);
	btShapeHull* hull = new btShapeHull(&shape);
	hull->buildHull(0);
	btConvexHullShape ret((const btScalar*)hull->getVertexPointer(), hull->numVertices(), sizeof(btVector3));
	delete hull;
	return ret;
}

bool saveHull(std::string path, btConvexHullShape& shape)
{
	btDefaultSerializer serializer;
	serializer.startSerialization();
	shape.serializeSingleShape(&serializer);
	serializer.finishSerialization();

	FILE* f = fopen(path.c_str(), "wb");
	if (!f) return false;

	fwrite(serializer.getBufferPointer(), serializer.getCurrentBufferSize(), 1, f);
	fclose(f);
	return true;
}
bool loadHull(std::string path, btConvexHullShape& shape)
{
	btBulletWorldImporter importer(0);
	if (!importer.loadFile(path.c_str())) {
		std::cout << "Could not load hull file! \n";
		return false;
	}
	if (!importer.getNumCollisionShapes()) {
		std::cout << "Hull file not properly formatted! \n";
		return false;
	}
	btConvexHullShape* coll = (btConvexHullShape*)importer.getCollisionShapeByIndex(0);
	shape = *coll;

	return true;
	
}