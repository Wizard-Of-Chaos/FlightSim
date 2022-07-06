#include "GameAssets.h"
#include "AttributeReaders.h"
#include <iostream>

bool Assets::setMeshAsset(std::string name, IMesh* mesh)
{
	meshAssets[name] = mesh;
	return true;
}

bool Assets::setTextureAsset(std::string name, ITexture* texture)
{
	textureAssets[name] = texture;
	return true;
}

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
	std::string shapeLoc = "attributes/hulls/" + name + ".bullet";
	if (!loadHull(shapeLoc, hull)) {
		std::cout << "Hull not found for " << name << ". Attempting to load from mesh... \n";
		if (meshAssets[name]) {
			hull = createCollisionShapeFromMesh(meshAssets[name]);
			saveHull(shapeLoc, hull);
			std::cout << "Hull created and saved to " << shapeLoc << ". \n";
			fileLocations[name] = shapeLoc;
		}
		else {
			std::cout << "No mesh found for " << name << "!\n";
		}
	}
	else {
		fileLocations[name] = shapeLoc;
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

bool Assets::setWeaponFireSound(u32 id, ISoundSource* sound)
{
	weaponFireSounds[id] = sound;
	return true;
}
bool Assets::setWeaponImpactSound(u32 id, ISoundSource* sound)
{
	weaponImpactSounds[id] = sound;
	return true;
}

ISoundSource* Assets::getWeaponFireSound(u32 id)
{
	return weaponFireSounds[id];
}
ISoundSource* Assets::getWeaponImpactSound(u32 id)
{
	return weaponImpactSounds[id];
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
	soundAssets.clear();
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