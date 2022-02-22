#pragma once
#ifndef ATTRIBUTEREADERS_H
#define ATTRIBUTEREADERS_H
#include "BaseHeader.h"
#include "GvReader.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"

class SceneManager;
class GameController;

//Reads a vector from a string in the manner of: 40,50,-20
vector3df strToVec(std::string str);
//Creates a shipcomponent and an irrlicht component on the given entity.
//TODO: add on the rigid body loader
bool loadShip(std::string path, EntityId id, SceneManager* manager);
//Creates a weapon entity with a WeaponInfoComponent and an IrrlichtComponent.
bool loadWeapon(std::string path, EntityId weaponId, SceneManager* manager);

#endif 