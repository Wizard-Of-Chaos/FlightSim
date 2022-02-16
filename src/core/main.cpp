#include "BaseHeader.h"
#include "GameStateController.h"
#include "GvReader.h"
#include "Config.h"

int main()
{
	GameConfig config;
	config.loadConfig("gameconfig.gdat");
	config.saveConfig("gameconfig.gdat");
	dimension2du res(config.resX, config.resY);
	if (config.useScreenRes) {
		IrrlichtDevice* nullDev = createDevice(EDT_NULL);
		res = nullDev->getVideoModeList()->getDesktopResolution();
		nullDev->drop();
	}
	IrrlichtDevice* device = createDevice(config.driver, res, 32, config.fullscreen, config.stencilBuffer, config.vsync, 0);
	GameStateController controller(device);
	controller.mainLoop();
	device->drop();
	return 0;

}