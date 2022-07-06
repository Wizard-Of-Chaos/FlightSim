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

ISoundSource* Assets::getWepFireSound(u32 id, bool phys)
{
	if (phys) return physWeaponFireSounds[id];
	return weaponFireSounds[id];
}
bool Assets::setWepFireSound(u32 id, ISoundSource* sound, bool phys)
{
	if (phys) physWeaponFireSounds[id] = sound;
	else weaponFireSounds[id] = sound;
	return true;
}
ISoundSource* Assets::getWepHitSound(u32 id, bool phys)
{
	if (phys) return physWeaponImpactSounds[id];
	return weaponFireSounds[id];
}
bool Assets::setWepHitSound(u32 id, ISoundSource * sound, bool phys)
{
	if (phys) physWeaponImpactSounds[id] = sound;
	else weaponImpactSounds[id] = sound;
	return true;
}
bool Assets::setWeaponFireSound(u32 id, ISoundSource* sound)
{
	return setWepFireSound(id, sound);
}
bool Assets::setWeaponImpactSound(u32 id, ISoundSource* sound)
{
	return setWepHitSound(id, sound);
}

ISoundSource* Assets::getWeaponFireSound(u32 id)
{
	return getWepFireSound(id);
}
ISoundSource* Assets::getWeaponImpactSound(u32 id)
{
	return getWepHitSound(id);
}

bool Assets::setPhysWeaponFireSound(u32 id, ISoundSource* sound)
{
	return setWepFireSound(id, sound, true);
}
bool Assets::setPhysWeaponImpactSound(u32 id, ISoundSource* sound)
{
	return setWepHitSound(id, sound, true);
}
ISoundSource* Assets::getPhysWeaponFireSound(u32 id)
{
	return getWepFireSound(id, true);
}
ISoundSource* Assets::getPhysWeaponImpactSound(u32 id)
{
	return getWepHitSound(id, true);
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