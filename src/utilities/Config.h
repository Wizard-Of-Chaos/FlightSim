#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include "BaseHeader.h"
#include "GvReader.h"

const extern std::map <std::string, E_DRIVER_TYPE> drivers;

struct GameConfig
{
	GameConfig();
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

#endif 