#pragma once
#ifndef ATTRIBUTEREADERS_H
#define ATTRIBUTEREADERS_H
#include "BaseHeader.h"
#include "GvReader.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "LoadoutData.h"


class SceneManager;
class GameController;
class GameStateController;

/*
* These functions are used to load various chunks of data using the GvReader class. Can load ships, weapons, and other stuff
* from .gdat files.
*/

//Reads a vector from a string in the manner of: 40,50,-20
vector3df strToVec(std::string str);
//Creates a shipcomponent and an irrlicht component on the given entity.
//TODO: add on the rigid body loader
bool loadShip(std::string name, EntityId id, SceneManager* manager);
//Creates a weapon entity with a WeaponInfoComponent and an IrrlichtComponent.
bool loadWeapon(std::string name, EntityId weaponId, EntityId shipId, SceneManager* manager);

bool loadShipData(std::string path, GameStateController* cont, gvReader& in);
bool loadWeaponData(std::string path, GameStateController* cont, gvReader& in);
#endif 