#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include "BaseHeader.h"
#include "GvReader.h"
#include "InputComponent.h"
#include <unordered_map>
#include <vector>

const extern std::map <std::string, E_DRIVER_TYPE> drivers;

class GameStateController;

/*
* Holds the configuration for the game - things like fullscreen, the resolution, filtering, shadows, and other
* graphics stuff.
*/
struct VideoConfig
{
	VideoConfig();
	void loadConfig(std::string filename);
	void saveConfig(std::string filename);
	E_DRIVER_TYPE driver;
	bool fullscreen;
	bool useScreenRes;
	int resX;
	int resY;
	bool vsync;
	bool bilinearFiltering;
	bool anisotropicFiltering;
	bool trilinearFiltering;
	bool shadows;
	bool antialiasing;
	bool stencilBuffer;
};

struct KeyConfig
{
	KeyConfig();
	void loadConfig(std::string filename);
	void saveConfig(std::string filename);
	std::unordered_map<INPUT, EKEY_CODE> keybinds;
};

enum class PARTICLE_LEVEL
{
	OFF,
	LOW,
	HIGH
};

struct GameConfig
{
	GameConfig();
	void loadConfig(std::string filename);
	void saveConfig(std::string filename);
	PARTICLE_LEVEL particleLevel;
	bool spaceFriction;
	bool flightAssist;

};
#endif 