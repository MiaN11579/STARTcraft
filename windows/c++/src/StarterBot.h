#pragma once

#include "MapTools.h"

#include <BWAPI.h>

class StarterBot
{
	MapTools m_mapTools;
	//unit pointers
	BWAPI::Unit m_scout = nullptr;
	BWAPI::Unit m_builder1 = nullptr;
	BWAPI::Unit m_builder2 = nullptr;
	BWAPI::Unit m_builder3 = nullptr;
	BWAPI::Unit m_builder4 = nullptr;
	BWAPI::Unit m_refinery1 = nullptr;
	BWAPI::Unit m_refinery2 = nullptr;
	BWAPI::Unit m_refinery3 = nullptr;
	BWAPI::Unit m_refinery = nullptr;
	BWAPI::Unit m_academy = nullptr;
	BWAPI::Unit m_engeneering = nullptr;
	BWAPI::Unit m_barracks1 = nullptr;
	BWAPI::Unit m_barracks2 = nullptr;
	BWAPI::Unit m_factory1 = nullptr;
	BWAPI::Unit m_factory2 = nullptr;
	BWAPI::Unit m_bunker = nullptr;

	const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
	const BWAPI::UnitType supply = BWAPI::Broodwar->self()->getRace().getSupplyProvider();
	const BWAPI::UnitType refinery = BWAPI::Broodwar->self()->getRace().getRefinery();
	const BWAPI::UnitType marine = BWAPI::UnitTypes::Terran_Marine;
	const BWAPI::UnitType medic = BWAPI::UnitTypes::Terran_Medic;
	const BWAPI::UnitType firebat = BWAPI::UnitTypes::Terran_Firebat;
	const BWAPI::UnitType vulture = BWAPI::UnitTypes::Terran_Vulture;
	const BWAPI::UnitType tank = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
	const BWAPI::UnitType barrack = BWAPI::UnitTypes::Terran_Barracks;
	const BWAPI::UnitType factory = BWAPI::UnitTypes::Terran_Factory;
	const BWAPI::UnitType machine = BWAPI::UnitTypes::Terran_Machine_Shop;
	const BWAPI::UnitType academy = BWAPI::UnitTypes::Terran_Academy;
	const BWAPI::UnitType engeneering = BWAPI::UnitTypes::Terran_Engineering_Bay;
	const BWAPI::UnitType bunker = BWAPI::UnitTypes::Terran_Bunker;


	int workerPrice = workerType.mineralPrice();
	int marinePrice = marine.mineralPrice();
	int medicMinPrice = medic.mineralPrice();
	int medicGasPrice = medic.gasPrice();
	int firebatMinPrice = firebat.mineralPrice();
	int firebatGasPrice = firebat.gasPrice();
	int vulturePrice = vulture.mineralPrice();
	int tankMinPrice = tank.mineralPrice();
	int tankGasPrice = tank.gasPrice();
	int supplyPrice = supply.mineralPrice();
	int refineryPrice = refinery.mineralPrice();
	int barracksPrice = barrack.mineralPrice();
	int bunkerPrice = bunker.mineralPrice();
	int academyMinPrice = academy.mineralPrice();
	int academyGasPrice = academy.gasPrice();
	int engeneeringPrice = engeneering.mineralPrice();
	int factoryMinPrice = factory.mineralPrice();
	int factoryGasPrice = factory.gasPrice();
	int machineMinPrice = machine.mineralPrice();
	int machineGasPrice = machine.gasPrice();
	int weaponUpMinPrice = BWAPI::UpgradeTypes::Terran_Infantry_Weapons.mineralPrice();
	int weaponUpGasPrice = BWAPI::UpgradeTypes::Terran_Infantry_Weapons.gasPrice();

	int mineralCash = BWAPI::Broodwar->self()->minerals();
	int mineralTotal = BWAPI::Broodwar->self()->gatheredMinerals();
	int gasCash = BWAPI::Broodwar->self()->gas();
	int gasTotal = BWAPI::Broodwar->self()->gatheredGas();
	int supplyUsed = BWAPI::Broodwar->self()->supplyUsed();
	int supplyTotal = BWAPI::Broodwar->self()->supplyTotal();

	//boolean flags used for the build order
	bool worker5 = false;
	bool worker6 = false;
	bool worker7 = false;
	bool worker8 = false;
	bool worker9 = false;
	bool worker10 = false;
	bool worker11 = false;
	bool worker12 = false;
	bool worker13 = false;
	bool worker14 = false;

	bool refineryStart = false;
	bool bunkerFilled = false;
	bool mineralsExhausted = false;

	int workersBuilt = 0;
	int marinesBuilt = 0;
	int medicsBuilt = 0;
	int firebatsBuilt = 0;
	int vulturesBuilt = 0;
	int tanksBuilt = 0;
	int supplyBuilt = 0;
	int refineryBuilt = 0;
	int barracksBuilt = 0;
	int academyBuilt = 0;
	int engeneeringBuilt = 0;
	int factoryBuilt = 0;
	int machineBuilt = 0;
	int bunkersBuilt = 0;

	int medCounter = 0;
	bool enemyFound = false;
	int attackWave = 0;
	int attackCount = 20;
	BWAPI::Position enemyBase;
	BWAPI::Position playerBase = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	BWAPI::Position chokepoint = playerBase;
	BWAPI::Position bunkerPos = playerBase;
	std::deque<BWAPI::Unit> enemyBuildings;

public:

	StarterBot();

	// helper functions to get you started with bot programming and learn the API
	void sendIdleWorkersToMinerals();
	void attack();
	void sendIdleWorkersToRefineries();
	void trainAdditionalWorkers();
	void train(BWAPI::UnitType type);
	void factoryTrain(BWAPI::UnitType type);
	void build(BWAPI::UnitType type, int required, BWAPI::Unit builder);
	void buildBunker();
	BWAPI::Unit getenemyInRegion();
	void scout();
	void assignMarinesToBunker();
	void drawDebugInformation();

	// functions that are triggered by various BWAPI events from main.cpp
	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);
};