#include "GameAssets.h"
#include "AttributeReaders.h"
#include <iostream>

IMesh* Assets::getMeshAsset(std::string name) 
{
	if (meshAssets[name]) {
		return meshAssets[name];
	}
	meshAssets[name] = smgr->getMesh(fileLocations[name].c_str());
	return meshAssets[name];
}

ITexture* Assets::getTextureAsset(std::string name) 
{
	if (textureAssets[name]) {
		return textureAssets[name];
	}
	textureAssets[name] = driver->getTexture(fileLocations[name].c_str());
	return textureAssets[name];
}

ITexture* Assets::getHUDAsset(std::string name) 
{
	if (HUDAssets[name]) {
		return HUDAssets[name];
	}
	HUDAssets[name] = driver->getTexture(fileLocations[name].c_str());
	return HUDAssets[name];
}

ISoundSource* Assets::getSoundAsset(std::string name)
{
	if (soundAssets[name]) {
		return soundAssets[name];
	}
	soundAssets[name] = soundEngine->getSoundSource(fileLocations[name].c_str());
	return soundAssets[name];
}

btConvexHullShape Assets::getHullAsset(std::string name) 
{
	if (hullAssets.find(name) != hullAssets.end()) {
		return hullAssets[name];
	}

	btConvexHullShape hull;
	if (!loadHull(fileLocations[name], hull)) {
		hull = createCollisionShapeFromMesh(hullMeshes[name]);
		saveHull(fileLocations[name], hull);
	}
	hullAssets[name] = hull;
	return hull;
}

IGUIFont* Assets::getFontAsset(std::string name) 
{
	if (fontAssets[name]) {
		return fontAssets[name];
	}

	fontAssets[name] = guienv->getFont(fileLocations[name].c_str());
	return fontAssets[name];
}

void Assets::clearLoadedGameAssets() 
{
	for (auto [name, mesh] : meshAssets) {
		if(mesh)smgr->getMeshCache()->removeMesh(mesh);
	}
	for (auto [name, tex] : textureAssets) {
		if(tex)driver->removeTexture(tex);
	}
	for (auto [name, tex] : HUDAssets) {
		if(tex)driver->removeTexture(tex);
	}
	meshAssets.clear();
	HUDAssets.clear();
	textureAssets.clear();
	hullAssets.clear();
}

void Assets::setFilenames() 
{
	fileLocations.clear();
	gvReader in;
	in.read("attributes/filelocations.gdat");
	in.readLinesToValues();
	for (auto [name, location] : in.values) {
		fileLocations[name] = location;
	}
}