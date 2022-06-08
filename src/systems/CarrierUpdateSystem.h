#pragma once
#ifndef CARRIERUPDATESYSTEM_H
#define CARRIERUPDATESYSTEM_H
#include "BaseHeader.h"
#include "CarrierComponent.h"
#include "IrrlichtComponent.h"
#include "FactionComponent.h"

void carrierUpdateSystem(flecs::iter it, CarrierComponent* carr, IrrlichtComponent* irr, FactionComponent* fac);

#endif 