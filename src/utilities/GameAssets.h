#pragma once
#ifndef GAMEASSETS_H
#define GAMEASSETS_H

#include "BaseHeader.h"
#include <unordered_map>
//Holds all the default textures and sounds and meshes for the game that can be used as placeholders if you can't load your custom mesh.

class Assets
{
	private:
		std::unordered_map<std::string, std::string> fileLocations;
		std::unordered_map<std::string, IMesh*> meshAssets;
		std::unordered_map<std::string, ITexture*> HUDAssets;
		std::unordered_map<std::string, ITexture*> textureAssets;
		std::unordered_map<std::string, ISoundSource*> soundAssets;
		std::unordered_map<std::string, btConvexHullShape> hullAssets;
		std::unordered_map<std::string, IMesh*> hullMeshes;
		std::unordered_map<std::string, IGUIFont*> fontAssets;

	public:
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