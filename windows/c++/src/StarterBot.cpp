#include "StarterBot.h"
#include "Tools.h"
#include "MapTools.h"

StarterBot::StarterBot()
{
}

// Called when the bot starts!
void StarterBot::onStart()
{
    // Set our BWAPI options here
    BWAPI::Broodwar->setLocalSpeed(10);
    BWAPI::Broodwar->setFrameSkip(0);

    // Enable the flag that tells BWAPI top let users enter input while bot plays
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

    // Call MapTools OnStart
    m_mapTools.onStart();
}

// Called whenever the game ends and tells you if you won or not
void StarterBot::onEnd(bool isWinner)
{
    std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called on each frame of the game
void StarterBot::onFrame()
{
    const int workersOwned = Tools::CountUnitsOfType(workerType, BWAPI::Broodwar->self()->getUnits());
    // get various things which will be used later on.
    const int mineralCash = BWAPI::Broodwar->self()->minerals();
    const int supplyUsed = BWAPI::Broodwar->self()->supplyUsed();
    // Update our MapTools information
    m_mapTools.onFrame();

    // keep building workers untill we reach 24
    trainAdditionalWorkers();
    // continously train more marines once we have a barracks
    if (barracksBuilt >= 1)
    {
        trainAdditionalMarines();
    }
    // continously send any idle workers to mine minerals
    sendIdleWorkersToMinerals();

    if (workersBuilt >= 6)
    {
        if (!m_scout || !m_builder1 || !m_builder2 || !m_refinery1 || !m_refinery2 || !m_refinery3)
        {
            int counter = 0;
            // For each unit that we own
            for (auto &unit : BWAPI::Broodwar->self()->getUnits())
            {
                // if the unit is of the correct type, and it actually has been constructed, return it
                if (unit->getType() == workerType && unit->isCompleted())
                {
                    if (counter == 0)
                        m_scout = unit;
                    else if (counter == 1)
                        m_builder1 = unit;
                    else if (counter == 2)
                        m_builder2 = unit;
                    else if (counter == 3)
                        m_refinery1 = unit;
                    else if (counter == 4)
                        m_refinery2 = unit;
                    else if (counter == 5)
                        m_refinery3 = unit;
                }
                if (counter >= 5)
                {
                    break;
                }
                counter++;
            }
        }
    }

    if (workersBuilt >= 7)
    {
        if (mineralCash >= 101)
        {
            build(supply, 1, m_builder1);
        }
        build(barrack, 1, m_builder2);
    }

    if (workersBuilt >= 8)
    {
        build(barrack, 2, m_builder1);
        build(supply, 2, m_builder2);
    }

    if (workersBuilt >= 10)
    {
        // code for sending the scout around the map.
        auto &startLocations = BWAPI::Broodwar->getStartLocations();
        for (BWAPI::TilePosition tp : startLocations)
        {
            if (BWAPI::Broodwar->isExplored(tp))
            {
                BWAPI::TilePosition pos(tp);
                continue;
            }
            BWAPI::Position pos(tp);
            m_scout->move(pos);
            break;
        }
        build(supply, 3, m_builder1);
    }

    if (supplyBuilt >= 2)
    {
        build(refinery, 3, m_builder2);
    }

    if (refineryBuilt >= 1)
    {
        build(supply, 3, m_builder1);
        // BWAPI::Unit closestRefinery1 = Tools::GetClosestUnitTo(m_refinery1, BWAPI::Broodwar->getGeysers());
        // BWAPI::Unit closestRefinery2 = Tools::GetClosestUnitTo(m_refinery2, BWAPI::Broodwar->getGeysers());
        // BWAPI::Unit closestRefinery3 = Tools::GetClosestUnitTo(m_refinery3, BWAPI::Broodwar->getGeysers());
        // if (closestRefinery1) { m_refinery1->rightClick(closestRefinery1); }
        // if (closestRefinery2) { m_refinery1->rightClick(closestRefinery2); }
        // if (closestRefinery3) { m_refinery1->rightClick(closestRefinery3); }
    }
    if (barracksBuilt >= 2)
    {
        build(factory, 1, m_builder2);
    }

    // Draw unit health bars, which brood war unfortunately does not do
    // Tools::DrawUnitHealthBars();

    // Draw some relevent information to the screen to help us debug the bot
    // drawDebugInformation();
}

// Send our idle workers to mine minerals so they don't just stand there
void StarterBot::sendIdleWorkersToMinerals()
{
    // Let's send all of our starting workers to the closest mineral to them
    // First we need to loop over all of the units that we (BWAPI::Broodwar->self()) own
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Check the unit type, if it is an idle worker, then we want to send it somewhere
        if (unit->getType().isWorker() && unit->isIdle())
        {
            // Get the closest mineral to this worker unit
            BWAPI::Unit closestMineral = Tools::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());

            // If a valid mineral was found, right click it with the unit in order to start harvesting
            if (closestMineral)
            {
                unit->rightClick(closestMineral);
            }
        }
    }
}

// Train more workers so we can gather more income
void StarterBot::trainAdditionalWorkers()
{
    const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
    const int workersWanted = 24;
    const int workersOwned = Tools::CountUnitsOfType(workerType, BWAPI::Broodwar->self()->getUnits());
    if (workersOwned < workersWanted)
    {
        // get the unit pointer to my depot
        const BWAPI::Unit myDepot = Tools::GetDepot();

        // if we have a valid depot unit and it's currently not training something, train a worker
        // there is no reason for a bot to ever use the unit queueing system, it just wastes resources
        if (myDepot && !myDepot->isTraining())
        {
            myDepot->train(workerType);
        }
    }
}

// Train more marines in barracks
void StarterBot::trainAdditionalMarines()
{
    const BWAPI::UnitType marineType = BWAPI::UnitTypes::Terran_Marine;
    const int marinesWanted = 10;
    const int marinesOwned = Tools::CountUnitsOfType(marineType, BWAPI::Broodwar->self()->getUnits());
    if (marinesOwned < marinesWanted)
    {
        const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
        for (auto &unit : myUnits)
        {
            // Check the unit type, if it is an idle barrack, train more marine
            if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks && unit->isCompleted() && !unit->isTraining())
            {
                BWAPI::Broodwar->printf("Started training %s", marineType.getName().c_str());
                unit->train(marineType);
            }
        }
    } 
    else 
    {
        attackWithMarines();
    }
}

void StarterBot::attackWithMarines()
{
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        if (unit->getType() == BWAPI::UnitTypes::Terran_Marine)
        {
            unit->attack(BWAPI::Position(BWAPI::Broodwar->enemy()->getStartLocation())); // not working rn
        }
    }
}

// Build more supply if we are going to run out soon
void StarterBot::buildAdditionalSupply()
{
    // Get the amount of supply supply we currently have unused
    const int unusedSupply = Tools::GetTotalSupply(true) - BWAPI::Broodwar->self()->supplyUsed();

    // If we have a sufficient amount of supply, we don't need to do anything
    if (unusedSupply >= 2)
    {
        return;
    }

    // Otherwise, we are going to build a supply provider
    const BWAPI::UnitType supplyProviderType = BWAPI::Broodwar->self()->getRace().getSupplyProvider();

    const bool startedBuilding = Tools::BuildBuilding(supplyProviderType);
    if (startedBuilding)
    {
        BWAPI::Broodwar->printf("Started Building %s", supplyProviderType.getName().c_str());
    }
}

// Build given type of building to given required number
void StarterBot::build(BWAPI::UnitType type, int required, BWAPI::Unit builder)
{
    const int typeOwned = Tools::CountUnitsOfType(type, BWAPI::Broodwar->self()->getUnits());
    // If we have a sufficient amount of supply, we don't need to do anything
    if (typeOwned >= required) { return; }
    // Get a location that we want to build the building next to
    BWAPI::TilePosition desiredPos = BWAPI::Broodwar->self()->getStartLocation();

    // Ask BWAPI for a building location near the desired position for the type
    int maxBuildRange = 64;
    BWAPI::TilePosition buildPos = BWAPI::Broodwar->getBuildLocation(type, desiredPos, maxBuildRange, false);
    const bool startedBuilding = builder->build(type, buildPos);
    if (startedBuilding)
    {
        BWAPI::Broodwar->printf("Started Building %s", type.getName().c_str());
    }
}

// Draw some relevent information to the screen to help us debug the bot
void StarterBot::drawDebugInformation()
{
    Tools::DrawUnitCommands();
    Tools::DrawUnitBoundingBoxes();
}

// Called whenever a unit is destroyed, with a pointer to the unit
void StarterBot::onUnitDestroy(BWAPI::Unit unit)
{
}

// Called whenever a unit is morphed, with a pointer to the unit
// Zerg units morph when they turn into other units
void StarterBot::onUnitMorph(BWAPI::Unit unit)
{
}

// Called whenever a text is sent to the game by a user
void StarterBot::onSendText(std::string text)
{
    if (text == "/map")
    {
        m_mapTools.toggleDraw();
    }
}

// Called whenever a unit is created, with a pointer to the destroyed unit
// Units are created in buildings like barracks before they are visible,
// so this will trigger when you issue the build command for most units
void StarterBot::onUnitCreate(BWAPI::Unit unit)
{
}

// Called whenever a unit finished construction, with a pointer to the unit
void StarterBot::onUnitComplete(BWAPI::Unit unit)
{
    // if the unit is of the correct type, and it actually has been constructed, add to it's count.
    if (unit->getType() == workerType && unit->isCompleted())
    {
        workersBuilt++;
        BWAPI::Broodwar->printf("worker: %d\n", workersBuilt);
    }
    else if (unit->getType() == supply && unit->isCompleted()) { supplyBuilt++; }
    else if (unit->getType() == barrack && unit->isCompleted())
    {
        barracksBuilt++;
        BWAPI::Broodwar->printf("barrack: %d\n", barracksBuilt);
    }
    else if (unit->getType() == refinery && unit->isCompleted()) { refineryBuilt++; }
    else if (unit->getType() == factory && unit->isCompleted()) { factoryBuilt++; }
    else if (unit->getType() == starport && unit->isCompleted()) { starportBuilt++; }
    else if (unit->getType() == controlTower && unit->isCompleted()) { controlTowerBuilt++; }
    else if (unit->getType() == sciFacility && unit->isCompleted()) { sciFacilityBuilt++; }
    else if (unit->getType() == nucSilo && unit->isCompleted()) { nucSiloBuilt++; }
}

// Called whenever a unit appears, with a pointer to the destroyed unit
// This is usually triggered when units appear from fog of war and become visible
void StarterBot::onUnitShow(BWAPI::Unit unit)
{
}

// Called whenever a unit gets hidden, with a pointer to the destroyed unit
// This is usually triggered when units enter the fog of war and are no longer visible
void StarterBot::onUnitHide(BWAPI::Unit unit)
{
}

// Called whenever a unit switches player control
// This usually happens when a dark archon takes control of a unit
void StarterBot::onUnitRenegade(BWAPI::Unit unit)
{
}