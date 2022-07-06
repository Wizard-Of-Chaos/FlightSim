#pragma once
#ifndef GAMEASSETS_H
#define GAMEASSETS_H

#include "BaseHeader.h"
#include <unordered_map>

//Holds all the default textures and sounds and meshes for the game that can be used as placeholders if you can't load your custom mesh.
//When you're loading something, you should set it up in here so it can get cleared when the game ends.

class Assets
{
	private:
		std::unordered_map<std::string, std::string> fileLocations;
		std::unordered_map<std::string, IMesh*> meshAssets;
		std::unordered_map<std::string, ITexture*> HUDAssets;
		std::unordered_map<std::string, ITexture*> textureAssets;
		std::unordered_map<std::string, ISoundSource*> soundAssets;
		std::unordered_map<std::string, btConvexHullShape> hullAssets;
		std::unordered_map<std::string, IGUIFont*> fontAssets;

		std::unordered_map<u32, ISoundSource*> weaponFireSounds;
		std::unordered_map<u32, ISoundSource*> weaponImpactSounds;

		std::unordered_map<u32, ISoundSource*> physWeaponFireSounds;
		std::unordered_map<u32, ISoundSource*> physWeaponImpactSounds;

		ISoundSource* getWepFireSound(u32 id, bool phys = false);
		bool setWepFireSound(u32 id, ISoundSource* sound, bool phys = false);
		ISoundSource* getWepHitSound(u32 id, bool phys = false);
		bool setWepHitSound(u32 id, ISoundSource* sound, bool phys = false);
	public:
		bool setWeaponFireSound(u32 id, ISoundSource* sound);
		bool setWeaponImpactSound(u32 id, ISoundSource* sound);
		bool setPhysWeaponFireSound(u32 id, ISoundSource* sound);
		bool setPhysWeaponImpactSound(u32 id, ISoundSource* sound);

		ISoundSource* getWeaponFireSound(u32 id);
		ISoundSource* getWeaponImpactSound(u32 id);
		ISoundSource* getPhysWeaponFireSound(u32 id);
		ISoundSource* getPhysWeaponImpactSound(u32 id);

		bool setMeshAsset(std::string name, IMesh* mesh);
		bool setTextureAsset(std::string name, ITexture* texture);

		IMesh* getMeshAsset(std::string name);
		ITexture* getTextureAsset(std::string name);
		ITexture* getHUDAsset(std::string name);
		ISoundSource* getSoundAsset(std::string name);
		btConvexHullShape getHullAsset(std::string name);
		IGUIFont* getFontAsset(std::string name);
		void setFilenames();
		void clearLoadedGameAssets();
};
#endif