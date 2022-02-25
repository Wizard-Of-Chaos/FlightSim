#pragma once
#ifndef GAMEDEFAULTS_H
#define GAMEDEFAULTS_H

#include "BaseHeader.h"

//Holds all the default textures and sounds and meshes for the game that can be used as placeholders if you can't load your custom mesh.
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
	ITexture* defaultJetTexture;
	ITexture* defaultEngineJetTexture;
	ISoundSource* defaultLaserSound;
	ISoundSource* defaultEngineSoundLoop;
	ISoundSource* defaultMusic;
	ISoundSource* defaultJetSoundLoop;
};

#endif