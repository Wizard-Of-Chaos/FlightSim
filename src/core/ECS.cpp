#include "ECS.h"
#include <iostream>

int componentCounter = 0;

EntityId Scene::newEntity() { //The only function that should ACTUALLY be called anywhere else
	if (!freeEntities.empty()) {
		EntityIndex newIndex = freeEntities.back();
		freeEntities.pop_back();
		EntityId newId = createEntityId(newIndex, getEntityVersion(entities[newIndex].id));
		entities[newIndex].id = newId;
		return entities[newIndex].id;
	}
	entities.push_back({ createEntityId(EntityIndex(entities.size()), 0), ComponentMask() });
	return entities.back().id;
}

EntityId createEntityId(EntityIndex index, EntityVersion version) {
	return ((EntityId)index << 32) | ((EntityId)version);
}
EntityIndex getEntityIndex(EntityId id) {
	return id >> 32;
}
EntityVersion getEntityVersion(EntityId id) {
	return (EntityVersion)id;
}
bool isEntityValid(EntityId id) {
	return (id >> 32) != EntityIndex(-1);
}

bool Scene::entityInUse(EntityId cachedId) {
	if (!isEntityValid(cachedId)) {
		return false;
	}
	if (entities[getEntityIndex(cachedId)].id == cachedId) {
		return true;
	} return false;
}

void parentUpdate(Scene& scene)
{
	for (auto id : SceneView<ParentComponent>(scene)) {
		auto parent = scene.get<ParentComponent>(id);
		if (!scene.entityInUse(parent->parentId)) {
			parent->parentId = INVALID_ENTITY;
		}
	}
}
