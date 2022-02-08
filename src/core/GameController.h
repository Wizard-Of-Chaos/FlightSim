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

enum SELECTIONS
{
	ID_IsNotSelectable = 0,
	ID_IsSelectable = 1 << 0,
	ID_IsAvoidable = 1 << 1
};

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

		GameController(GameStateController* controller);
		void init();
		void close();

		void initDefaultScene();
		void update();

	private:

		SceneManager sceneECS; 
		u32 then;

		f32 accumulator = 0.0f;
		f32 dt = 0.005f;
		f32 t = 0.0f;
};


#endif