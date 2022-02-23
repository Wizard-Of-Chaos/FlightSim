#include "AttributeReaders.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

vector3df strToVec(std::string str)
{
	std::string xstr, ystr, zstr;

	std::stringstream split(str);
	std::getline(split, xstr, ',');
	std::getline(split, ystr, ',');
	std::getline(split, zstr, ',');
	return vector3df(std::stof(xstr), std::stof(ystr), std::stof(zstr));
}

bool loadShip(std::string path, EntityId id, SceneManager* manager)
{
	std::cout << "Reading ship in from: " << path << std::endl;
	gvReader in;
	in.read(path);
	if (in.lines.empty()) return false;
	in.readLinesToValues();

	auto ship = manager->scene.assign<ShipComponent>(id);
	auto irr = manager->scene.assign<IrrlichtComponent>(id);
	if (!ship || !irr) return false;

	ISceneManager* smgr = manager->controller->smgr;
	IVideoDriver* driver = manager->controller->driver;

	std::string meshpath = "models/" + in.values["model"];
	std::string texpath = "models/" + in.values["texture"];
	irr->name = in.values["name"];

	IMesh* shipmesh = smgr->getMesh(meshpath.c_str());
	ITexture* shiptex = driver->getTexture(meshpath.c_str());
	irr->node = smgr->addMeshSceneNode(shipmesh);
	irr->node->setMaterialTexture(0, shiptex);

	irr->node->setName(idToStr(id).c_str());
	irr->node->setID(ID_IsSelectable | ID_IsAvoidable);

	ship->forwardThrust = std::stof(in.values["forwardThrust"]);
	ship->brakeThrust = std::stof(in.values["brakeThrust"]);
	ship->strafeThrust = std::stof(in.values["strafeThrust"]);
	ship->pitchThrust = std::stof(in.values["pitchThrust"]);
	ship->yawThrust = std::stof(in.values["yawThrust"]);
	ship->rollThrust = std::stof(in.values["rollThrust"]);

	ship->hardpointCount = std::stoi(in.values["hardpointCount"]);
	std::string val;
	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		val = "hardpoint" + std::to_string(i);
		ship->hardpoints[i] = strToVec(in.values[val]);
		ship->weapons[i] = INVALID_ENTITY;
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "upJetPos" + std::to_string(i);
		ship->upJetPos[i] = strToVec(in.values[val]);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "downJetPos" + std::to_string(i);
		ship->downJetPos[i] = strToVec(in.values[val]);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "leftJetPos" + std::to_string(i);
		ship->leftJetPos[i] = strToVec(in.values[val]);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		val = "rightJetPos" + std::to_string(i);
		ship->rightJetPos[i] = strToVec(in.values[val]);
	}

	ship->curPitch = 0;
	ship->curYaw = 0;
	return true;
}

bool loadWeapon(std::string path, EntityId weaponId, SceneManager* manager)
{
	gvReader in;
	in.read(path);
	if (in.lines.empty()) return false;
	in.readLinesToValues();

	auto wep = manager->scene.assign<WeaponInfoComponent>(weaponId);
	auto irr = manager->scene.assign<IrrlichtComponent>(weaponId);
	if (!wep || !irr) return false;

	ISceneManager* smgr = manager->controller->smgr;
	IVideoDriver* driver = manager->controller->driver;

	std::string meshpath = "models/" + in.values["model"];
	std::string texpath = "models/" + in.values["texture"];
	irr->name = in.values["name"];

	IMesh* wepmesh = smgr->getMesh(meshpath.c_str());
	ITexture* weptex = driver->getTexture(meshpath.c_str());
	irr->node = smgr->addMeshSceneNode(wepmesh);

	irr->node->setName(idToStr(weaponId).c_str());
	irr->node->setID(ID_IsNotSelectable);
	irr->node->setMaterialTexture(0, weptex);

	wep->isFiring = false;
	wep->type = (WEAPON_TYPE)std::stoi(in.values["type"]);
	wep->firingSpeed = std::stof(in.values["firingSpeed"]);
	wep->projectileSpeed = std::stof(in.values["projectileSpeed"]);
	wep->damage = std::stof(in.values["damage"]);
	wep->range = std::stof(in.values["range"]);
	wep->timeSinceLastShot = 0.f;

	return true;
}