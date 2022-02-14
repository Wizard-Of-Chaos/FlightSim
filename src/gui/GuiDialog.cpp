#include "GuiDialog.h"
#include "GuiController.h"

GuiDialog::GuiDialog(GuiController* controller) 
{ 
	guiController = controller;
	baseSize = dimension2du(960, 540);
	root = guiController->guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));
}

void GuiDialog::show() 
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), guiController->driver->getScreenSize()));
	root->setVisible(true);
};
void GuiDialog::hide() 
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), guiController->driver->getScreenSize()));
	root->setVisible(false); 
};