#include "Shaders.h"
#include "GameController.h"
#include "GameStateController.h"

void ShieldShaderCb::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	matrix4 invWorld = driver->getTransform(ETS_WORLD);
	invWorld.makeInverse();

	services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
	matrix4 worldViewProj;
	worldViewProj = driver->getTransform(ETS_PROJECTION);
	worldViewProj *= driver->getTransform(ETS_VIEW);
	worldViewProj *= driver->getTransform(ETS_WORLD);

	services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
	vector3df pos = ship->getAbsolutePosition();
	services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);

	SColorf col(0.f, .5f, 1.f, 0.f);
	services->setVertexShaderConstant("mLightColor", reinterpret_cast<f32*>(&col), 4);
	matrix4 world = driver->getTransform(ETS_WORLD);
	world = world.getTransposed();
	services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);
}