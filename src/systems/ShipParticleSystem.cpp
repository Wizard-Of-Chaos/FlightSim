#include "ShipParticleSystem.h"

void turnOnJets(IParticleEmitter* jet1, IParticleEmitter* jet2)
{
	jet1->setMaxParticlesPerSecond(800);
	jet1->setMinParticlesPerSecond(500);
	jet2->setMaxParticlesPerSecond(800);
	jet2->setMinParticlesPerSecond(500);
}

void turnOffJets(IParticleEmitter* jet1, IParticleEmitter* jet2)
{
	jet1->setMaxParticlesPerSecond(0);
	jet1->setMinParticlesPerSecond(0);
	jet2->setMaxParticlesPerSecond(0);
	jet2->setMinParticlesPerSecond(0);
}

void shipParticleSystem(Scene& scene, f32 dt)
{
	for (auto entityId : SceneView<ShipComponent>(scene)) {
		auto ship = scene.get<ShipComponent>(entityId);
		auto irr = scene.get<IrrlichtComponent>(entityId);

		IParticleEmitter* up1 = ship->upJetEmit[0]->getEmitter();
		IParticleEmitter* up2 = ship->upJetEmit[1]->getEmitter();
		IParticleEmitter* down1 = ship->downJetEmit[0]->getEmitter();
		IParticleEmitter* down2 = ship->downJetEmit[1]->getEmitter();
		IParticleEmitter* left1 = ship->leftJetEmit[0]->getEmitter();
		IParticleEmitter* left2 = ship->leftJetEmit[1]->getEmitter();
		IParticleEmitter* right1 = ship->rightJetEmit[0]->getEmitter();
		IParticleEmitter* right2 = ship->rightJetEmit[1]->getEmitter();

		up1->setDirection(getNodeUp(irr->node) * .02f);
		up2->setDirection(getNodeUp(irr->node) * .02f);
		down1->setDirection(getNodeDown(irr->node) * .02f);
		down2->setDirection(getNodeDown(irr->node) * .02f);
		left1->setDirection(getNodeLeft(irr->node) * .02f);
		left2->setDirection(getNodeLeft(irr->node) * .02f);
		right1->setDirection(getNodeRight(irr->node) * .02f);
		right2->setDirection(getNodeRight(irr->node) * .02f);

		turnOffJets(up1, up2);
		turnOffJets(down1, down2);
		turnOffJets(left1, left2);
		turnOffJets(right1, right2);

		if (ship->moves[SHIP_STRAFE_DOWN]) {
			turnOnJets(up1, up2);
		}
		if (ship->moves[SHIP_STRAFE_UP]) {
			turnOnJets(down1, down2);
		}
		if (ship->moves[SHIP_STRAFE_LEFT]) {
			turnOnJets(right1, right2);
		}
		if (ship->moves[SHIP_STRAFE_RIGHT]) {
			turnOnJets(left1, left2);
		}

		for (u32 i = 0; i < SHIP_MAX_MOVEMENTS; ++i) {
			ship->moves[i] = false;
		}
	}
}