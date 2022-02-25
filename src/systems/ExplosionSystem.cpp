#include "SceneManager.h"
#include "ExplosionSystem.h"
#include <iostream>
void explosionSystem(SceneManager* manager, f32 dt)
{
	for (auto id : SceneView<ExplosionComponent>(manager->scene))
	{
		auto exp = manager->scene.get<ExplosionComponent>(id);
		exp->lifetime += dt;
		auto em = exp->explosion->getEmitter();
		if (exp->lifetime >= exp->duration) {
			exp->explosion->remove();
			manager->scene.destroyEntity(id);
		}
		f32 completion = (exp->duration - exp->lifetime) / exp->duration;
		em->setMinParticlesPerSecond((u32)(200 * completion));
		em->setMaxParticlesPerSecond((u32)(500 * completion));

	}
}