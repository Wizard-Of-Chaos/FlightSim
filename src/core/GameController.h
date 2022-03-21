#pragma once

#ifndef FLIGHTEVENTRECEIVER_H
#define FLIGHTEVENTRECEIVER_H
#include "BaseHeader.h"

#include "ECS.h"
#include "SceneManager.h"
#include "IrrlichtComponent.h"
#include "InputComponent.h"
#include "GameFunctions.h"

class GameStateController;

//The different types of ID an irrlicht node might have. Used to determine whether or not a player can
//"select" the object (or whether or not an AI can avoid the object). Tack on more if you need them.
enum SELECTIONS
{
	ID_IsNotSelectable = 0,
	ID_IsSelectable = 1 << 0,
	ID_IsAvoidable = 1 << 1
};

//An extension of the bullet physics world with a helpful function that effectively deletes the world. Otherwise, it's the same.
class BulletPhysicsWorld : public btDiscreteDynamicsWorld
{
	public:
		BulletPhysicsWorld(btDispatcher* dispatcher, btBroadphaseInterface* broadphasePairCache, btSequentialImpulseConstraintSolver* solver, btCollisionConfiguration* collisionConfiguration) :
			btDiscreteDynamicsWorld(dispatcher, broadphasePairCache, solver, collisionConfiguration)
		{
		}
		void clearObjects()
		{
			getBroadphase()->~btBroadphaseInterface();
			new(getBroadphase())btDbvtBroadphase();
			m_collisionObjects.clear();
			m_nonStaticRigidBodies.clear();
			m_sortedConstraints.clear();
			m_constraints.clear();
			m_actions.clear();
			m_predictiveManifolds.clear();
		}
};

class btDebugRenderer : public btIDebugDraw
{
public:
	void setDriver(IVideoDriver* ndriver) { driver = ndriver; }
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		matrix4 id;
		id.makeIdentity();
		driver->setTransform(ETS_WORLD, id);
		driver->setMaterial(SMaterial());
		driver->draw3DLine(btVecToIrr(from), btVecToIrr(to), SColor(255,255,255,255));
	}
	virtual void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar dist, int lifeTime, const btVector3& color)
	{

	}
	virtual void reportErrorWarning(const char* warning) {
		printf(warning);
	}
	virtual void draw3dText(const btVector3& location, const char* text) {

	}
	virtual void setDebugMode(int nmode) { mode = nmode; }
	virtual int getDebugMode() const { return mode; }
private:
	IVideoDriver* driver;
	int mode;
};
/*
* The GameController class holds all the necessary information about what's actually going on in the game. It has pointers to the
* various drivers for the game (the irrlicht device, video driver, Irrlicht scene manager, ECS manager, sound engine, etc) and handles
* updates for the game. It is the Keeper of the DT (delta time) which determines how frequently scene and physics updates are called.
* 
* The init() function is used to initialize the game (set up the physics world, irrlicht stuff, some other things), and the close() function
* gets rid of those (such as when you return to the game menus). Many things include the GameController as a pointer - such as the SceneManager class.
*/
class GameController
{
	public:
		bool OnEvent(const SEvent& event);

		IrrlichtDevice* device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;
		BulletPhysicsWorld* bWorld;
		ISoundEngine* soundEngine;
		
		GameStateController* stateController;

		btBroadphaseInterface* broadPhase;
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		
		btDebugRenderer rend;

		GameController(GameStateController* controller);
		void init();
		void close();

		void initDefaultScene();
		void update();

		void clearPlayerHUD();
	private:
		SceneManager sceneECS; 
		u32 then;
		f32 accumulator = 0.0f;
		f32 dt = 0.005f;
		f32 t = 0.0f;

		bool open;
};


#endif