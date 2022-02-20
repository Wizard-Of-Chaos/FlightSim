#pragma once
#ifndef GAMEDEFAULTS_H
#define GAMEDEFAULTS_H

#include "BaseHeader.h"

struct GameDefaults
{
	IMesh* defaultShipMesh;
	IMesh* defaultWeaponMesh;
	IMesh* defaultObstacleMesh;
	ITexture* defaultShipTexture;
	ITexture* defaultProjectileTexture;
	ITexture* defaultCrosshairTexture;
	ITexture* defaultSelectionTexture;
	ITexture* defaultContactTexture;
	ITexture* defaultContactMarkerTexture;
	ITexture* defaultHealthBarTexture;
};

#endif