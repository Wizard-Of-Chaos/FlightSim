#pragma once
#ifndef OBJECTIVECOMPONENT_H
#define OBJECTIVECOMPONENT_H
#include "BaseHeader.h"

enum OBJECTIVE_TYPE
{
	OBJ_DESTROY,
	OBJ_GO_TO,
	OBJ_COLLECT,
	OBJ_MOVE_TO_OTHER_OBJ,
};

struct ObjectiveComponent
{
	OBJECTIVE_TYPE type;
};

#endif