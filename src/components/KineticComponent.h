#pragma once
#ifndef KINETICCOMPONENT_H
#define KINETICCOMPONENT_H
#include "BaseHeader.h"

/*
* Kinetic info components hold some extra information on kinetic weapons. Namely, how accurate they are, and how many bullets
* they fire per shot. The machine gun is more accurate than the shotgun, but the shotgun has more pellets.
* 
* I don't know why I specified that. That should be obvious.
*/
struct KineticInfoComponent
{
	u32 projectilesPerShot;
	f32 accuracy;
};

#endif 