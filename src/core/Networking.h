#pragma once

#ifndef NETWORKING_H
#define NETWORKING_H
#include "BaseHeader.h"
#include "BulletRigidBodyComponent.h"
#include <algorithm>
#include <stdexcept>
#include <assert.h>

const unsigned int MAX_DATA_SIZE = 8000; // in bytes

typedef uint32_t SequenceNumber;

struct PacketData {
	PacketData();
	//EntityId entityId;
	int componentId;
	// size of this buffer is determined by its component, gonna need to keep a mapping of componentId's -> buffer sizes
	char componentData[];
};

struct Packet {
	Packet();
	SequenceNumber sequenceNumber;
	// TODO: need to send input too
	
	unsigned int numEntries;
	// data[i] should be static_cast'd to PacketData
	char data[MAX_DATA_SIZE];
};

//void sortAccumulator(std::vector<std::tuple<EntityId, int, unsigned int>>& priorityAccumulator);
#endif