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

	const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
	const BWAPI::UnitType supply = BWAPI::Broodwar->self()->getRace().getSupplyProvider();
	const BWAPI::UnitType refinery = BWAPI::Broodwar->self()->getRace().getRefinery();
	const BWAPI::UnitType marine = BWAPI::UnitTypes::Terran_Marine;
	const BWAPI::UnitType medic = BWAPI::UnitTypes::Terran_Medic;
	const BWAPI::UnitType firebat = BWAPI::UnitTypes::Terran_Firebat;
	const BWAPI::UnitType barrack = BWAPI::UnitTypes::Terran_Barracks;
	const BWAPI::UnitType academy = BWAPI::UnitTypes::Terran_Academy;
	const BWAPI::UnitType engeneering = BWAPI::UnitTypes::Terran_Engineering_Bay;

	int workerPrice = workerType.mineralPrice();
	int marinePrice = marine.mineralPrice();
	int medicMinPrice = medic.mineralPrice();
	int medicGasPrice = medic.gasPrice();
	int supplyPrice = supply.mineralPrice();
	int refineryPrice = refinery.mineralPrice();
	int barracksPrice = barrack.mineralPrice();
	int academyMinPrice = academy.mineralPrice();
	int academyGasPrice = academy.gasPrice();
	int engeneeringPrice = engeneering.mineralPrice();
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

	int workersBuilt = 0;
	int marinesBuilt = 0;
	int medicsBuilt = 0;
	int firebatsBuilt = 0;
	int supplyBuilt = 0;
	int refineryBuilt = 0;
	int barracksBuilt = 0;
	int academyBuilt = 0;
	int engeneeringBuilt = 0;
	int factoryBuilt = 0;

	int medCounter = 0;
	bool enemyFound = false;
	int attackWave = 0;
	BWAPI::Position enemyBase;
	BWAPI::Position playerBase = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	BWAPI::Position chokepoint = playerBase;
	std::deque<BWAPI::Unit> enemyBuildings;

public:

	StarterBot();

	// helper functions to get you started with bot programming and learn the API
	void sendIdleWorkersToMinerals();
	void attack();
	void sendIdleWorkersToRefineries();
	void trainAdditionalWorkers();
	void train(BWAPI::UnitType type);
	void build(BWAPI::UnitType type, int required, BWAPI::Unit builder);
	BWAPI::Unit getenemyInRegion();
	void scout();
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