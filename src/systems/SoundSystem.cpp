#include "SoundSystem.h"
#include "GameController.h"

void soundSystem(f32 dt)
{
	for (auto it = gameController->sounds.begin(); it != gameController->sounds.end(); ++it) {
		auto irr = sceneManager->scene.get<IrrlichtComponent>(it->id);
		if (!irr || !it->sound) {
			gameController->sounds.erase(it); //I want to re-work the std library so instead of "erase" it's "whip". Whip it! Whip it good!
			continue;
		} else if (it->sound->isFinished()) {
			gameController->sounds.erase(it);
			continue;
		}
		it->sound->setPosition(irr->node->getAbsolutePosition());
	}
}