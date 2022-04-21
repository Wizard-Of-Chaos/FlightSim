#include "GuiDialog.h"
#include "GuiController.h"

GuiDialog::GuiDialog() 
{ 
	baseSize = dimension2du(960, 540);
	root = guienv->addImage(rect<s32>(position2di(0, 0), baseSize));
	IGUIImage* img = (IGUIImage*)root;
	img->setImage(driver->getTexture("ui/starbg.png"));
	img->setScaleImage(true);
	scaleAlign(img);

}

void GuiDialog::show() 
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
};
void GuiDialog::hide() 
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(false); 
};