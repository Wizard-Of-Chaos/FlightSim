#include "Shaders.h"
#include "GameController.h"

#include "GameStateController.h"
matrix4 getWorldViewProj()
{
	matrix4 out;
	out = driver->getTransform(ETS_PROJECTION);
	out *= driver->getTransform(ETS_VIEW);
	out *= driver->getTransform(ETS_WORLD);
	return out;
}

void ShieldShaderCb::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	invWorld = driver->getTransform(ETS_WORLD);
	invWorld.makeInverse();

	services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
	worldViewProj = getWorldViewProj();

	services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
	vector3df pos = ship->getAbsolutePosition();
	services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);

	SColorf col(0.f, .5f, 1.f, 0.f);
	services->setVertexShaderConstant("mLightColor", reinterpret_cast<f32*>(&col), 4);
	transposeWorld = driver->getTransform(ETS_WORLD);
	transposeWorld = transposeWorld.getTransposed();
	services->setVertexShaderConstant("mTransWorld", transposeWorld.pointer(), 16);
}

void ambientShaderCb::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	worldViewProj = getWorldViewProj();
	services->setVertexShaderConstant("worldViewProj", worldViewProj.pointer(), 16);

	worldInvTranspose = driver->getTransform(ETS_WORLD);
	worldInvTranspose.makeInverse();
	worldInvTranspose = worldInvTranspose.getTransposed();
	services->setVertexShaderConstant("worldInvTranspose", worldInvTranspose.pointer(), 16);
}