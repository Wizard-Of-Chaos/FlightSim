#pragma once

#ifndef ECS_H
#define ECS_H

/*
* This file holds the guts of the Entity-Component system. Don't screw with it unless asking
* one of the people who actually wrote it first.
*/

#include <bitset>
#include <tuple>
#include <vector>

//Typedefs for the index, version, and the combination of the two
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

//Used to check whether or not an entity is valid - as in, whether it's a valid ID (not whether or not it's currently being used).
#define INVALID_ENTITY createEntityId(EntityIndex(-1), 0)

//Holds the memory for all components
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

//Holds the data for the entities and component pools.
struct Scene {
	struct EntityDesc {
		EntityId id;
		ComponentMask mask;
	};

	std::vector<EntityDesc> entities;
	std::vector<EntityIndex> freeEntities;
	std::vector<ComponentPool*> componentPools;
	std::vector<std::tuple<EntityId, int, unsigned int>> priorityAccumulator;

	//This function is used to check whether or not the ID you have is currently being used
	//by the Scene. If it's not, you should probably stop whatever it is you're doing.
	bool entityInUse(EntityId cachedId);

	EntityId newEntity();

	template<typename T>
	//Assigns the given component to the ID given.
	//Called in the manner of: auto irrlichtComp = scene.assign<IrrlichtComponent>(myEntityId);
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
	//Gets the component associated with this ID.
	//Called in the manner of: auto irrlichtComp = scene.get<IrrlichtComponent>(myEntityId);
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
	//Removes the component from the entity.
	//Called in the manner of: scene.remove<IrrlichtComponent>(myEntityId);
	void remove(EntityId id) {
		if (entities[getEntityIndex(id)].id != id)
			return;
		int componentId = getId<T>();
		entities[getEntityIndex(id)].mask.reset(componentId);
	}
	//Removes the entity from the scene and frees up its memory.
	//DOES NOT actually delete the information associated with any pointers the entity might have.
	//Do your own damn cleanup.
	//Called in the manner of: scene.destroyEntity(myEntityId);
	void destroyEntity(EntityId id) {
		EntityId newId = createEntityId(EntityIndex(-1), getEntityVersion(id) + 1);
		entities[getEntityIndex(id)].id = newId;
		entities[getEntityIndex(id)].mask.reset();
		freeEntities.push_back(getEntityIndex(id));
	}
};

template<typename ...ComponentTypes>
//This structure allows us to loop through a given scene, sorted by components. The constructor is used
//to be able to do it.
//Used like: for(EntityId id : SceneView<IrrlichtComponent, BulletRigidBodyComponent>(scene))
//That for loop will loop through all entities in the scene with the associated components. You can then
//get those components and adjust them.
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

struct ParentComponent
{
	EntityId parentId;
};

void parentUpdate(Scene& scene);

#endif