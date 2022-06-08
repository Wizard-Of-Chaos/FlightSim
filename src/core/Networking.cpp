#include "Networking.h"


Packet::Packet() {
	sequenceNumber = 0;
	numEntries = 0;
}

PacketData::PacketData() {
	//entityId = 0;
	componentId = 0;
}

/// <summary>
/// Constructs a packet to be sent over the wire for the current scene
/// NOTE: make sure to free()/delete the created packet once you're done with it
/// </summary>
/// <param name="scene">scene to be serialized and sent over the wire</param>
/// <returns>a packet to be sent over the wire</returns>
/// 
/*
Packet* buildPacket(Scene& scene) {
	auto packet = new Packet();
	auto spaceRemaining = MAX_DATA_SIZE;

	for (auto& [entityId, componentId, priority] : scene.priorityAccumulator) {
		bool (*serializer)(EntityId, char*, Scene& scene) = nullptr;
		unsigned int payloadSize = 0;

		unsigned int componentPrio = 1;

		if (componentId == getId<BulletRigidBodyComponent>()) {
			// serializer = serializeBulletRigidBodyComponent;
			// TODO: calculate payload size
		}
		else if (componentId == getId<HealthComponent>()) {
			// serializer = serializeHealthComponent;
			// TODO: calculate payload size
		}
		else {
			throw std::invalid_argument("Component with componentId: " + std::to_string(componentId) + " is not a known serializable type");
		}

		// check if serialized component would fit in buffer
		// if yes, serialize it and place it in
		if (payloadSize + sizeof(PacketData) < spaceRemaining) {
			auto nextSlot = packet->data + (MAX_DATA_SIZE - spaceRemaining);
			auto packetData = new (nextSlot) PacketData();
			
			packetData->entityId = entityId;
			packetData->componentId = componentId;
			auto success = serializer(entityId, packetData->componentData, scene);

			if (!success) {
				// TODO: decide what to do when serialization fails
			}
			priority = componentPrio;
			spaceRemaining -= sizeof(PacketData) + payloadSize;
		}
		else {
			priority += componentPrio;
		}
	}

	sortAccumulator(scene.priorityAccumulator);

	return packet;
}

bool accumulatorPred(const std::tuple<EntityId, int, unsigned int> left, const std::tuple<EntityId, int, unsigned int> right) {
	auto prioLeft = std::get<2>(left);
	auto prioRight = std::get<2>(right);
	return prioLeft < prioRight;
}
void sortAccumulator(std::vector<std::tuple<EntityId, int, unsigned int>>& priorityAccumulator) {
	for (auto it = priorityAccumulator.begin(); it != priorityAccumulator.end(); it++) {
		auto const insertionPoint = std::lower_bound(priorityAccumulator.begin(), it, *it, accumulatorPred);

		std::rotate(insertionPoint, it, it + 1);
	}
}
*/