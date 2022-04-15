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
	ITexture* defaultObstacleTexture;
	btConvexHullShape defaultObstacleHull;
	IMesh* defaultMissileMesh;

	ITexture* defaultShipTexture;
	ITexture* defaultProjectileTexture;
	ITexture* defaultCrosshairTexture;
	ITexture* defaultSelectionTexture;
	ITexture* defaultContactTexture;
	ITexture* defaultContactMarkerTexture;
	ITexture* defaultHealthBarTexture;
	ITexture* defaultVelocityBarTexture;
	ITexture* defaultFuelBarTexture;

	ITexture* defaultJetTexture;
	ITexture* defaultEngineJetTexture;
	ITexture* defaultExplosion;

	ITexture* defaultCloudTexture;

	ISoundSource* defaultLaserSound;
	ISoundSource* bonk;
	ISoundSource* defaultEngineSoundLoop;
	ISoundSource* defaultMusic;
	ISoundSource* defaultJetSoundLoop;
	ISoundSource* defaultGunSound;

	IGUIFont* defaultHUDFont;
};

struct loadedData
{
	std::vector<ShipComponent> loadedShips;
	std::vector<WeaponInfoComponent> loadedWeapons;
};

#endif