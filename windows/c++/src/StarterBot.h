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
	BWAPI::Unit m_academy = nullptr;
	BWAPI::Unit m_engeneering = nullptr;

	const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
	const BWAPI::UnitType supply = BWAPI::Broodwar->self()->getRace().getSupplyProvider();
	const BWAPI::UnitType refinery = BWAPI::Broodwar->self()->getRace().getRefinery();
	const BWAPI::UnitType marine = BWAPI::UnitTypes::Terran_Marine;
	const BWAPI::UnitType medic = BWAPI::UnitTypes::Terran_Medic;
	const BWAPI::UnitType barrack = BWAPI::UnitTypes::Terran_Barracks;
	const BWAPI::UnitType academy = BWAPI::UnitTypes::Terran_Academy;
	const BWAPI::UnitType engeneering = BWAPI::UnitTypes::Terran_Engineering_Bay;

	int workersBuilt = 0;
	int marinesBuilt = 0;
	int medicsBuilt = 0;
	int supplyBuilt = 0;
	int refineryBuilt = 0;
	int barracksBuilt = 0;
	int academyBuilt = 0;
	int engeneeringBuilt = 0;

	int medCounter = 0;

	bool enemyFound = false;
	BWAPI::Position enemyBase;

public:

	StarterBot();

	// helper functions to get you started with bot programming and learn the API
	void sendIdleWorkersToMinerals();
	void attack();
	void freeBuilder();
	void sendIdleWorkersToRefineries();
	void trainAdditionalWorkers();
	void trainAdditionalMarines();
	void trainMarine();
	void trainAdditionalMedics();
	void trainMedic();
	void buildAdditionalSupply();
	void scout();
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