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
//Loads the ship data from the game state controller onto the given entity.
//Includes an Irrlicht component andg6 a ShipComponent.
bool loadShip(u32 id, EntityId entity, SceneManager* manager);
//Loads the weapon data from the game state controller onto the given entity.
//Includes a WeaponInfoComponent, an Irrlicht component, a ParentComponent, and whatever other components are necessary (e.g., a MissileInfoComponent).
bool loadWeapon(u32 id, EntityId weaponEntity, EntityId shipEntity, SceneManager* manager);

//Pulls the ship data from the given .gdat file and saves it in the game state controller.
bool loadShipData(std::string path, GameStateController* cont, gvReader& in);
//Pulls the weapon data from the given .gdat file and saves it in the game state controller.
bool loadWeaponData(std::string path, GameStateController* cont, gvReader& in);

btConvexHullShape createCollisionShapeFromMesh(IMesh* mesh);
#endif 