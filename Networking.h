#pragma once

#ifndef NETWORKING_H
#define NETWORKING_H
#include "BaseHeader.h"
#include "ECS.h"
#include "BulletRigidBodyComponent.h"
#include <algorithm>
#include <stdexcept>

const unsigned int MAX_DATA_SIZE = 8000; // in bytes

typedef uint32_t SequenceNumber;

struct PacketData {
	PacketData();
	EntityId entityId;
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

struct BitWriter {
	uint32_t* buffer;
	int wordIndex;
	uint64_t scratch;
	int scratchBits;

	void writeBits(uint32_t thingToWrite, short numBits);

};

struct BitReader {
	uint32_t* buffer;
	int wordIndex = 0;
	uint64_t scratch = 0;
	int scratchBits = 0;
	int numBitsRead = 0;
	int totalBits = 0;

	void read(short numBits, uint32_t* to);

};

Packet* buildPacket(Scene& scene);
bool accumulatorPred(const std::tuple<EntityId, int, unsigned int> left, const std::tuple<EntityId, int, unsigned int> right);
void sortAccumulator(std::vector<std::tuple<EntityId, int, unsigned int>>& priorityAccumulator);
bool serializeBulletRigidBodyComponent(EntityId entityId, char* buffer, Scene& scene);
bool serializeHealthComponent(EntityId entityId, char* buffer, Scene& scene);


#endif