#pragma once

#ifndef BASEFLIGHTHEADER_H
#define BASEFLIGHTHEADER_H

/*
* This header holds all of the basic includes that are needed for the project.
* When adding new files, this shoudl be the first thing you include.
*/
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>
#include <irrKlang.h>
#include <flecs.h>
using namespace irrklang;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")

#if _DEBUG
#pragma comment(lib, "BulletDynamics_Debug.lib")
#pragma comment(lib, "BulletCollision_Debug.lib")
#pragma comment(lib, "LinearMath_Debug.lib")
#pragma comment(lib, "BulletWorldImporter_Debug.lib")
#pragma comment(lib, "BulletFileLoader_Debug.lib")
#else 
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "LinearMath.lib")
#pragma comment(lib, "BulletWorldImporter.lib")
#pragma comment(lib, "BulletFileLoader.lib")
#endif 

#pragma comment(lib, "irrKlang.lib")
#endif

class GameStateController;
class GameController;
class GuiController;
class SceneManager;
class BulletPhysicsWorld;

/*
* These are global variables used from throughout the program to track what exactly is going on.
*/

extern GameStateController* stateController;
extern GameController* gameController;
extern GuiController* guiController;

extern IrrlichtDevice* device;
extern IVideoDriver* driver;
extern ISceneManager* smgr;
extern IGUIEnvironment* guienv;
extern ISoundEngine* soundEngine;
extern BulletPhysicsWorld* bWorld;
extern flecs::world* game_world;

const flecs::id_t INVALID_ENTITY_ID = 0;
#define INVALID_ENTITY flecs::entity(game_world->get_world(), INVALID_ENTITY_ID)

#endif