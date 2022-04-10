#include "Config.h"
const std::map <std::string, E_DRIVER_TYPE> drivers =
{
	{"Direct3D9", EDT_DIRECT3D9},
	{"Direct3D8", EDT_DIRECT3D8},
	{"OpenGL", EDT_OPENGL}
};

VideoConfig::VideoConfig() //default configuration
{
	driver = EDT_DIRECT3D9;
	fullscreen = true;
	useScreenRes = true;
	resX = 960;
	resY = 540;
	vsync = true;
	bilinearFiltering = false;
	trilinearFiltering = true;
	anisotropicFiltering = true;
	shadows = true;
	antialiasing = true;
	stencilBuffer = true;
}

void VideoConfig::loadConfig(std::string filename)
{
	gvReader in;
	in.read(filename);
	in.readLinesToValues();
	if (in.lines.empty()) return;

	driver = drivers.at(in.values["driver"]);
	fullscreen = std::stoi(in.values["fullscreen"]);
	useScreenRes = std::stoi(in.values["useScreenRes"]);
	resX = std::stoi(in.values["resX"]);
	resY = std::stoi(in.values["resY"]);
	vsync = std::stoi(in.values["vsync"]);
	bilinearFiltering = std::stoi(in.values["bilinearFiltering"]);
	trilinearFiltering = std::stoi(in.values["trilinearFiltering"]);
	anisotropicFiltering = std::stoi(in.values["anisotropicFiltering"]);
	shadows = std::stoi(in.values["shadows"]);
	antialiasing = std::stoi(in.values["antialiasing"]);
	stencilBuffer = std::stoi(in.values["stencilBuffer"]);

}

void VideoConfig::saveConfig(std::string filename)
{
	gvReader out;
	std::string driverstr;
	for (auto pair : drivers) {
		if (pair.second == driver) {
			driverstr = pair.first;
			break;
		}
	}
	out.values["driver"] = driverstr;
	out.values["fullscreen"] = std::to_string(fullscreen);
	out.values["useScreenRes"] = std::to_string(useScreenRes);
	out.values["resX"] = std::to_string(resX);
	out.values["resY"] = std::to_string(resY);
	out.values["vsync"] = std::to_string(vsync);
	out.values["bilinearFiltering"] = std::to_string(bilinearFiltering);
	out.values["trilinearFiltering"] = std::to_string(trilinearFiltering);
	out.values["anisotropicFiltering"] = std::to_string(anisotropicFiltering);
	out.values["shadows"] = std::to_string(shadows);
	out.values["antialiasing"] = std::to_string(antialiasing);
	out.values["stencilBuffer"] = std::to_string(stencilBuffer);

	out.readValuesToLines();
	out.write(filename);
}

KeyConfig::KeyConfig()
{

}

void KeyConfig::loadConfig(std::string filename)
{

}

void KeyConfig::saveConfig(std::string filename)
{

}

GameConfig::GameConfig()
{
	particleLevel = PARTICLE_LEVEL::HIGH;
	spaceFriction = false;
	flightAssist = true;
}

void GameConfig::loadConfig(std::string filename)
{
	gvReader in;
	in.read(filename);
	in.readLinesToValues();
	if (in.lines.empty()) return;

	particleLevel = (PARTICLE_LEVEL)std::stoi(in.values["particleLevel"]);
	spaceFriction = std::stoi(in.values["spaceFriction"]);
	flightAssist = std::stoi(in.values["flightAssist"]);
}

void GameConfig::saveConfig(std::string filename)
{
	gvReader out;
	out.values["particleLevel"] = std::to_string((int)particleLevel);
	out.values["spaceFriction"] = std::to_string(spaceFriction);
	out.values["flightAssist"] = std::to_string(flightAssist);

	out.readValuesToLines();
	out.write(filename);
}