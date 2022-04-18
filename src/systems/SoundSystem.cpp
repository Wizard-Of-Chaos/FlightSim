#include "SoundSystem.h"
#include "GameController.h"

void soundSystem(f32 dt)
{
	auto it = gameController->sounds.begin();
	while(it != gameController->sounds.end()) {
		if (!sceneManager->scene.entityInUse(it->id)) {
			it = gameController->sounds.erase(it);
			continue;
		}
		auto irr = sceneManager->scene.get<IrrlichtComponent>(it->id);
		if (!irr || !it->sound) {
			it = gameController->sounds.erase(it); //I want to re-work the std library so instead of "erase" it's "whip". Whip it! Whip it good!
			continue;
		} else if (it->sound->isFinished()) {
			it->sound->drop();
			it = gameController->sounds.erase(it);
			continue;
		}
		it->sound->setPosition(irr->node->getAbsolutePosition());
		++it;
	}
}