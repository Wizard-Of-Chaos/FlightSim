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

u32 loadShipData(std::string path, GameStateController* cont, gvReader& in)
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

	data->shipMesh = cont->smgr->getMesh(meshpath.c_str());
	data->shipTexture = cont->driver->getTexture(texpath.c_str());
	data->shipNorm = cont->driver->getTexture(normpath.c_str());
	if (data->shipNorm) {
		cont->driver->makeNormalMapTexture(data->shipNorm, 7.f);
		data->shipMesh = cont->smgr->getMeshManipulator()->createMeshWithTangents(data->shipMesh); //drop this somewhere
	}
	//data->collisionShape = createCollisionShapeFromMesh(data->shipMesh);

	std::string jetpath = "effects/" + in.values["jet"];
	std::string enginepath = "effects/" + in.values["engine"];

	data->engineTexture = cont->driver->getTexture(enginepath.c_str());
	data->jetTexture = cont->driver->getTexture(jetpath.c_str());

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

	cont->shipData[id] = data;
	std::cout << "Done.\n";
	return id;
}

u32 loadWeaponData(std::string path, GameStateController* cont, gvReader& in)
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
	if (type == WEP_PLASMA) {
		//nothing for now
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

	data->weaponMesh = cont->smgr->getMesh(meshpath.c_str());
	data->weaponTexture = cont->driver->getTexture(meshpath.c_str());
	
	std::string effectpath = "effects/" + in.values["particle"];
	data->weaponEffect = cont->driver->getTexture(effectpath.c_str());

	
	if (type == WEP_MISSILE) {
		std::string misspath = "models/" + in.values["missilemodel"];
		std::string misstexpath = "models/" + in.values["missiletexture"];
		MissileData* miss = (MissileData*)data;
		miss->missileMesh = cont->smgr->getMesh(misspath.c_str());
		miss->missileTexture = cont->driver->getTexture(misstexpath.c_str());
		miss->missileComponent.maxMissileVelocity = std::stof(in.values["maxMissileVelocity"]);
		miss->missileComponent.missileRotThrust = std::stof(in.values["missileRotThrust"]);
		miss->missileComponent.timeToLock = std::stof(in.values["timeToLock"]);
		miss->missileComponent.missileMesh = miss->missileMesh;
	}

	data->weaponComponent.isFiring = false;
	data->weaponComponent.type = type;
	data->weaponComponent.dmgtype = dmgtype;
	data->weaponComponent.firingSpeed = std::stof(in.values["firingSpeed"]);
	data->weaponComponent.projectileSpeed = std::stof(in.values["projectileSpeed"]);
	data->weaponComponent.damage = std::stof(in.values["damage"]);
	data->weaponComponent.range = std::stof(in.values["range"]);
	data->weaponComponent.timeSinceLastShot = 0.f;
	data->weaponComponent.particle = data->weaponEffect;

	cont->weaponData[id] = data;
	std::cout << "Done.\n";
	return id;
}

bool loadShip(u32 id, EntityId entity, SceneManager* manager)
{
	ISceneManager* smgr = manager->controller->smgr;
	IVideoDriver* driver = manager->controller->driver;
	GameStateController* stateCtrl = manager->controller->stateController;

	ShipData* data = stateCtrl->shipData[id];
	if (!data) return false;

	auto ship = manager->scene.assign<ShipComponent>(entity);
	auto irr = manager->scene.assign<IrrlichtComponent>(entity);
	if (!irr || !ship) return false;
	*ship = data->shipComponent;

	irr->node = smgr->addMeshSceneNode(data->shipMesh);
	irr->node->setMaterialTexture(0, data->shipTexture);
	if (data->shipNorm) {
		irr->node->setMaterialTexture(1, data->shipNorm);
		irr->node->setMaterialType(EMT_PARALLAX_MAP_SOLID);
	}
	irr->node->setName(idToStr(entity).c_str());
	irr->node->setID(ID_IsSelectable | ID_IsAvoidable);

	return true;
}

bool loadWeapon(u32 id, EntityId weaponEntity, EntityId shipEntity, SceneManager* manager)
{
	ISceneManager* smgr = manager->controller->smgr;
	IVideoDriver* driver = manager->controller->driver;
	GameStateController* stateCtrl = manager->controller->stateController;

	WeaponData* data = stateCtrl->weaponData[id];
	if (!data) return false;

	auto wep = manager->scene.assign<WeaponInfoComponent>(weaponEntity);
	auto irr = manager->scene.assign<IrrlichtComponent>(weaponEntity);
	auto parent = manager->scene.assign<ParentComponent>(weaponEntity);
	if (!wep || !irr || !parent) return false;
	
	parent->parentId = shipEntity;
	
	irr->node = smgr->addMeshSceneNode(data->weaponMesh);
	irr->node->setMaterialTexture(0, data->weaponTexture);
	irr->node->setName(idToStr(weaponEntity).c_str());
	irr->node->setID(ID_IsNotSelectable);

	*wep = data->weaponComponent;
	if (data->weaponComponent.type == WEP_MISSILE) {
		auto miss = manager->scene.assign<MissileInfoComponent>(weaponEntity);
		MissileData* mdata = (MissileData*)data;
		*miss = mdata->missileComponent;
	}

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