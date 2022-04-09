#include "BaseHeader.h"
#include "GameStateController.h"
#include "GvReader.h"
#include "Config.h"

int main()
{
	VideoConfig config;
	config.loadConfig("cfg/videoconfig.gdat");
	config.saveConfig("cfg/videoconfig.gdat");
	dimension2du res(config.resX, config.resY);
	if (config.useScreenRes) {
		IrrlichtDevice* nullDev = createDevice(EDT_NULL); //Used to get the current screen res if needed
		res = nullDev->getVideoModeList()->getDesktopResolution();
		nullDev->drop();
	}
	IrrlichtDevice* device = createDevice(config.driver, res, 32, config.fullscreen, config.stencilBuffer, config.vsync, 0);
	GameStateController controller(device, config);
	controller.videoConfig = config;
	controller.mainLoop();
	device->drop();
	return 0;

}