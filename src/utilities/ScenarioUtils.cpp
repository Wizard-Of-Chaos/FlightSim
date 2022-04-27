#include "ScenarioUtils.h"
#include "GameStateController.h"
#include "GameController.h"
#include "SceneManager.h"

Scenario randomScenario()
{
	gvReader in;
	in.read("attributes/scenariodesc.gdat");
	in.readLinesToValues();
	SCENARIO_TYPE type = static_cast<SCENARIO_TYPE>(std::rand() % SCENARIO_MAX_TYPES);
	if (type == SCENARIO_MAX_TYPES) type = SCENARIO_KILL_HOSTILES;
	SCENARIO_ENVIRONMENT env = static_cast<SCENARIO_ENVIRONMENT>(std::rand() % SCENENV_MAX_ENVIRONMENTS);
	if (env == SCENENV_MAX_ENVIRONMENTS) env = SCENENV_ASTEROID_FIELD;

	//env = SCENENV_GAS_FIELD; //remove later when other environments are implemented

	std::string location = scenarioEnvStrings.at(env);
	std::string description = in.values[location];
	description += "\n";
	description += in.values[scenarioStrings.at(type)];
	u32 objCount = std::rand() % (stateController->campaign.currentDifficulty * 3) + 1;

	vector3df player(0, 0, -50);
	vector3df enemy(10, 20, 80);
	Scenario scen(type, env, objCount, player, enemy);

	if(env != SCENENV_EMPTY) setObstaclePositions(scen);

	/*
	std::cout << location << std::endl;
	std::cout << description << std::endl;
	std::cout << "type: " << type << ", env: " << env << ", objective count: " << objCount << std::endl;
	*/
	scen.location = location;
	scen.description = description;
	return scen;
}

void buildScenario(Scenario& scenario)
{
	ISceneNode* n = smgr->addLightSceneNode(0, vector3df(0, 10000, 0),
		SColor(200, 255, 180, 180), 50000.f);
	n->setID(ID_IsNotSelectable);

	setScenarioType(scenario);
	setEnvironment(scenario);

	ITexture* sky = driver->getTexture("effects/starskybox.png");
	n = smgr->addSkyBoxSceneNode(sky, sky, sky, sky, sky, sky);
	n->setID(ID_IsNotSelectable);

}

void setEnvironment(Scenario& scenario)
{
	std::cout << "Setting environment... \n";
	switch (scenario.environment) {
	case SCENENV_ASTEROID_FIELD:
		buildAsteroidField(scenario);
		break;
	case SCENENV_GAS_FIELD:
		buildGasField(scenario);
		break;
	default:
		std::cout << "No valid environment! \n";
		break;
	}
}

void setScenarioType(Scenario& scenario)
{
	std::cout << "Setting up scenario type... \n";
	switch (scenario.type) {
	case SCENARIO_KILL_HOSTILES:
		setKillHostilesScenario(scenario);
		break;
	default:
		std::cout << "No valid type! Defaulting to kill hostiles. \n";
		setKillHostilesScenario(scenario);
		break;
	}
}

void setObstaclePositions(Scenario& scenario)
{
	std::cout << "Setting obstacle positions... ";
	for (u32 i = 0; i < 1000; ++i) {
		vector3df pos = getPointInSphere(vector3df(0, 0, 0), 5000.f);
		scenario.obstaclePositions.push_back(pos);
	}
	std::cout << "Done.\n";
}

void buildAsteroidField(Scenario& scenario) 
{
	std::cout << "Building asteroid field... ";
	for (u32 i = 0; i < scenario.obstaclePositions.size(); ++i) {
		u32 scale = std::rand() % 100;
		f32 mass = (f32)scale / 5.f;
		//EntityId gas = createGasCloud(obstaclePositions[i], vector3df(scale, scale, scale));
		EntityId rock = createAsteroid(scenario.obstaclePositions[i], randomRotationVector(), vector3df(scale, scale, scale), mass);
	}
	std::cout << "Done.\n";

	//auto cloud = createGasCloud(vector3df(100, 0, 0), vector3df(10, 10, 10));
}

void buildGasField(Scenario& scenario) 
{
	std::cout << "Building gas field... ";
	for (u32 i = 0; i < scenario.obstaclePositions.size(); ++i) {
		u32 scale = std::rand() % 30;
		EntityId gas = createGasCloud(scenario.obstaclePositions[i], vector3df(scale, scale, scale));
	}
	std::cout << "Done. \n";
}

void setKillHostilesScenario(Scenario& scenario)
{
	EntityId player = createPlayerShipFromLoadout(scenario.playerStartPos, vector3df(0, 0, 0));
	initializePlayerFaction(player);
	std::cout << "Culling obstacle positions... ";
	for (u32 i = 0; i < scenario.obstaclePositions.size(); ++i) {
		vector3df pos = scenario.obstaclePositions[i];
		f32 radius = 75.f;
		if (isPointInSphere(pos, scenario.playerStartPos, radius)) {
			scenario.obstaclePositions.erase(scenario.obstaclePositions.begin() + i);
			continue;
		}
		if (isPointInSphere(pos, scenario.enemyStartPos, radius)) {
			scenario.obstaclePositions.erase(scenario.obstaclePositions.begin() + i);
		}
	}
	std::cout << "Done. Obstacles remaining: " << scenario.obstaclePositions.size() << std::endl;

	std::cout << "Setting up hostiles... ";
	for (u32 i = 0; i < scenario.objectiveCount; ++i) {
		vector3df pos = getPointInSphere(scenario.enemyStartPos, 25.f);
		EntityId enemy = createDefaultAIShip(pos, vector3df(0, 180, 0)); //todo: create AI ship generator that pulls from loaded ships
		scenario.objectives[i] = enemy;
	}
	std::cout << "Done. \n";
}