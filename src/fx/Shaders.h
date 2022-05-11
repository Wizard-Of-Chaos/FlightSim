#pragma once
#ifndef SHADERS_H
#define SHADERS_H
#include "BaseHeader.h"

class ShieldShaderCb : public IShaderConstantSetCallBack
{
	public:
		void setShip(ISceneNode* nship) { ship = nship; }
		virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	private:
		ISceneNode* ship;
};

#endif 