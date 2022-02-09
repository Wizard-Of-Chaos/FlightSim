#pragma once

#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <tuple>
#include <vector>

typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
typedef uint64_t EntityId;
const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 400;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

extern int componentCounter;

template <class T>
int getId() {
	static int componentId = componentCounter++;
	return componentId;
}

EntityId createEntityId(EntityIndex index, EntityVersion version);
EntityIndex getEntityIndex(EntityId id);
EntityVersion getEntityVersion(EntityId id);
bool isEntityValid(EntityId id);

#define INVALID_ENTITY createEntityId(EntityIndex(-1), 0)


struct ComponentPool {
	ComponentPool(size_t elementsize) {
		elementSize = elementsize;
		poolData = new char[elementsize * MAX_ENTITIES];
	}

	~ComponentPool() {
		delete[] poolData;
	}

	void* get(size_t index) {
		return poolData + index * elementSize;
	}

	char* poolData{ nullptr };
	size_t elementSize{ 0 };
};

struct Scene {
	struct EntityDesc {
		EntityId id;
		ComponentMask mask;
	};

	std::vector<EntityDesc> entities;
	std::vector<EntityIndex> freeEntities;
	std::vector<ComponentPool*> componentPools;
	std::vector<std::tuple<EntityId, int, unsigned int>> priorityAccumulator;

	bool entityInUse(EntityId cachedId);

	EntityId newEntity();

	template<typename T>
	T* assign(EntityId id) {
		if (entities[getEntityIndex(id)].id != id)
			return nullptr;

		int componentId = getId<T>();
		if (componentPools.size() <= componentId) {
			componentPools.resize(componentId + 1, nullptr);
		}
		if (componentPools[componentId] == nullptr) {
			componentPools[componentId] = new ComponentPool(sizeof(T));
		}

		T* component = new (componentPools[componentId]->get(getEntityIndex(id))) T();
		entities[getEntityIndex(id)].mask.set(componentId);
		return component;
	}

	template<typename T>
	T* get(EntityId id) {
		if (entities[getEntityIndex(id)].id != id)
			return nullptr;
		int componentId = getId<T>();
		if (!entities[getEntityIndex(id)].mask.test(componentId))
			return nullptr;
		T* component = static_cast<T*>(componentPools[componentId]->get(getEntityIndex(id)));
		return component;
	}

	template<typename T>
	void remove(EntityId id) {
		if (entities[getEntityIndex(id)].id != id)
			return;
		int componentId = getId<T>();
		entities[getEntityIndex(id)].mask.reset(componentId);
	}

	void destroyEntity(EntityId id) {
		EntityId newId = createEntityId(EntityIndex(-1), getEntityVersion(id) + 1);
		entities[getEntityIndex(id)].id = newId;
		entities[getEntityIndex(id)].mask.reset();
		freeEntities.push_back(getEntityIndex(id));
	}
};

template<typename ...ComponentTypes>
struct SceneView
{
	//variables
	Scene* scene{ nullptr };
	ComponentMask componentMask;
	bool all{ false };

	SceneView(Scene& scene) : scene(&scene) {
		if (sizeof...(ComponentTypes) == 0) {
			all = true;
		} else {
			int componentIds[] = { 0, getId<ComponentTypes>() ... }; //visual studio does not like this line
			for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++) componentMask.set(componentIds[i]);
		}
	}
	struct Iterator
	{
		EntityIndex index;
		Scene* scene;
		ComponentMask mask;
		bool all{ false };

		Iterator(Scene* scene, EntityIndex index, ComponentMask mask, bool all) : scene(scene), index(index), mask(mask), all(all) {}

		EntityId operator*() const { return scene->entities[index].id; }
		bool operator==(const Iterator& other) const { return index == other.index || index == scene->entities.size(); }
		bool operator!=(const Iterator& other) const { return index != other.index && index != scene->entities.size(); }
		bool ValidIndex() { return isEntityValid(scene->entities[index].id) && (all || mask == (mask & scene->entities[index].mask)); }
		Iterator& operator++() {
			do { index++; } 
			while (index < scene->entities.size() && !ValidIndex()); 
			return *this;
		}
	};

	const Iterator begin() const {
		unsigned int firstIndex = 0;
		while (firstIndex < scene->entities.size() &&
			(componentMask != (componentMask & scene->entities[firstIndex].mask) || !isEntityValid(scene->entities[firstIndex].id))) {
			firstIndex++;
		}
		return Iterator(scene, firstIndex, componentMask, all);
	}
	const Iterator end() const {
		return Iterator(scene, EntityIndex(scene->entities.size()), componentMask, all);
	}
};

#endif