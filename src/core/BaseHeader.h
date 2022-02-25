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

#include <irrKlang.h>
using namespace irrklang;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")

#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "LinearMath.lib")

#pragma comment(lib, "irrKlang.lib")
#endif

#endif