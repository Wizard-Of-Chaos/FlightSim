#include "SoundSystem.h"
#include "GameController.h"

void soundSystem(flecs::iter it)
{
	auto soundIt = gameController->sounds.begin();
	while(soundIt != gameController->sounds.end()) {
		flecs::entity ent(game_world->get_world(), soundIt->id);
		if (!ent.is_alive()) {
			soundIt = gameController->sounds.erase(soundIt);
			continue;
		}
		auto irr = ent.get<IrrlichtComponent>();

		if (!irr || !soundIt->sound) {
			soundIt = gameController->sounds.erase(soundIt); //I want to re-work the std library so instead of "erase" it's "whip". Whip it! Whip it good!
			continue;
		} else if (soundIt->sound->isFinished()) {
			soundIt->sound->drop();
			soundIt = gameController->sounds.erase(soundIt);
			continue;
		}
		soundIt->sound->setPosition(irr->node->getAbsolutePosition());
		++soundIt;
	}
}