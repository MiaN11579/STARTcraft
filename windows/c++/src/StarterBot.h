#pragma once

#include "MapTools.h"

#include <BWAPI.h>

class StarterBot
{
	MapTools m_mapTools;
	//scout unit
	BWAPI::Unit m_scout = nullptr;
	BWAPI::Unit m_builder1 = nullptr;
	BWAPI::Unit m_builder2 = nullptr;
	BWAPI::Unit m_refinery1 = nullptr;
	BWAPI::Unit m_refinery2 = nullptr;
	BWAPI::Unit m_refinery3 = nullptr; 

	const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
	const BWAPI::UnitType supply = BWAPI::Broodwar->self()->getRace().getSupplyProvider();
	const BWAPI::UnitType refinery = BWAPI::Broodwar->self()->getRace().getRefinery();
	const BWAPI::UnitType barrack = BWAPI::UnitTypes::Terran_Barracks;
	const BWAPI::UnitType factory = BWAPI::UnitTypes::Terran_Factory;
	const BWAPI::UnitType starport = BWAPI::UnitTypes::Terran_Starport;
	const BWAPI::UnitType controlTower = BWAPI::UnitTypes::Terran_Control_Tower;
	const BWAPI::UnitType sciFacility = BWAPI::UnitTypes::Terran_Science_Facility;
	const BWAPI::UnitType academy = BWAPI::UnitTypes::Terran_Academy;
	const BWAPI::UnitType nucSilo = BWAPI::UnitTypes::Terran_Nuclear_Silo;

	int workersBuilt = 0;
	int supplyBuilt = 0;
	int refineryBuilt = 0;
	int barracksBuilt = 0;
	int factoryBuilt = 0;
	int starportBuilt = 0;
	int controlTowerBuilt = 0;
	int sciFacilityBuilt = 0;
	int nucSiloBuilt = 0;

	BWAPI::Position enemyBase;

public:

	StarterBot();

	// helper functions to get you started with bot programming and learn the API
	void sendIdleWorkersToMinerals();
	void trainAdditionalWorkers();
	void trainAdditionalMarines();
	void buildAdditionalSupply();
	void attackWithMarines();
	void build(BWAPI::UnitType type, int required, BWAPI::Unit builder);
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