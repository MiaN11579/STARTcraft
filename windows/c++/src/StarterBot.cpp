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
    const int mineralTotal = BWAPI::Broodwar->self()->gatheredMinerals();
    const int gasCash = BWAPI::Broodwar->self()->gas();
    const int gasTotal = BWAPI::Broodwar->self()->gatheredGas();
    const int supplyUsed = BWAPI::Broodwar->self()->supplyUsed();
    const int supplyTotal = BWAPI::Broodwar->self()->supplyTotal();
    // Update our MapTools information
    m_mapTools.onFrame();

    if (workersBuilt < 25)
    {
        // i put the iff loop in because the function kept making workers after it reached 24.
        trainAdditionalWorkers();
    }

    if (barracksBuilt >= 1)
    {
        if (medCounter >= 3)
        {
            trainMedic();
        }
        else
        {
            trainMarine();
        }
    }

    // continously send any idle workers to mine minerals
    sendIdleWorkersToMinerals();

    if (refineryBuilt >= 1)
    {
        // i put this here so that the workers that would be sent to gather
        // gas will still gather minerals while the refinery is being built.
        sendIdleWorkersToRefineries();
    }

    if (workersBuilt >= 5)
    {
        scout();
    }

    if (workersBuilt >= 8)
    {
        build(supply, 1, m_builder1);
    }

    if (workersBuilt >= 10)
    {
        build(barrack, 1, m_builder2);
    }

    if (workersBuilt >= 12)
    {
        build(refinery, 1, m_refinery1);
    }

    if (barracksBuilt >= 1)
    {
        build(barrack, 2, m_builder3);
    }

    if (barracksBuilt >= 2)
    {
        build(engeneering, 1, m_builder4);
        build(barrack, 3, m_builder2);
    }

    if (supplyBuilt >= 1)
    {
        build(academy, 1, m_builder1);
        build(supply, 2, m_builder2);
    }

    if (barracksBuilt >= 3)
    {
        build(barrack, 4, m_builder3);
    }

    if (barracksBuilt >= 4)
    {
        build(barrack, 5, m_builder4);
    }

    if (academyBuilt >= 1)
    {
    }

    if (supplyBuilt >= 2)
    {
        build(supply, 3, m_builder2);
    }

    if (supplyBuilt >= 3)
    {
        build(supply, 4, m_builder3);
    }

    if (supplyBuilt >= 4 && (supplyUsed == supplyTotal - 4))
    {
        build(supply, 5, m_builder4);
    }

    if (supplyBuilt >= 5 && (supplyUsed == supplyTotal - 4))
    {
        build(supply, 6, m_builder1);
    }

    if (supplyBuilt >= 6 && (supplyUsed == supplyTotal - 4))
    {
        build(supply, 7, m_builder2);
    }

    if (supplyBuilt >= 7 && (supplyUsed == supplyTotal - 4))
    {
        build(supply, 8, m_builder3);
    }

    if (supplyBuilt >= 8 && (supplyUsed == supplyTotal - 4))
    {
        build(supply, 9, m_builder4);
    }

    if (marinesBuilt >= 10)
    {
        BWAPI::Broodwar->printf("Started an attack");
        marinesBuilt = 0;
        attack();
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

// Send 3 workers to collect gas
void StarterBot::sendIdleWorkersToRefineries()
{
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Find a refinery
        if (unit->getType() == refinery)
        {
            auto command = m_refinery1->getLastCommand();
            if (command.getTarget() != unit) // Check if worker is already assigned to refinery
            {
                m_refinery1->rightClick(unit);
                m_refinery2->rightClick(unit);
                m_refinery3->rightClick(unit);
            }
            return;
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

// Train more medics in barracks
void StarterBot::trainMedic()
{
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Check the unit type, if it is an idle barrack, train more marine
        if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks && unit->isCompleted() && !unit->isTraining())
        {
            bool train = unit->train(medic);
            if (train)
            {
                BWAPI::Broodwar->printf("Started training %s", medic.getName().c_str());
            }
        }
    }
}

// Train more marines in barracks
void StarterBot::trainMarine()
{
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Check the unit type, if it is an idle barrack, train more marine
        if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks && unit->isCompleted() && !unit->isTraining())
        {
            bool train = unit->train(marine);
            if (train)
            {
                BWAPI::Broodwar->printf("Started training %s", marine.getName().c_str());
            }
        }
    }
}

void StarterBot::attack()
{
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Check the unit type, if it is an idle worker, then we want to send it somewhere
        if ((unit->getType() == marine || unit->getType() == medic) && unit->isIdle())
        {
            auto command = unit->getLastCommand();
            if (command.getTargetPosition() == enemyBase)
            {
                continue;
            }
            unit->attack(enemyBase);
        }
    }
}

void StarterBot::scout()
{
    if (enemyFound == false)
    {
        // code for sending the scout around the map.
        auto &startLocations = BWAPI::Broodwar->getStartLocations();
        for (BWAPI::TilePosition tp : startLocations)
        {
            if (BWAPI::Broodwar->isExplored(tp))
            {
                continue;
            }
            BWAPI::Position pos(tp);
            auto command = m_scout->getLastCommand();
            if (command.getTargetPosition() == pos)
            {
                return;
            }
            m_scout->move(pos);
            return;
        }
        // if we find an enemy unit, mark the location and return scout to base.
        BWAPI::Unit pos_enemy = m_scout->getClosestUnit();
        if (BWAPI::Broodwar->self()->isEnemy(pos_enemy->getPlayer()) && pos_enemy->getType().isBuilding())
        {
            enemyBase = pos_enemy->getPosition();
            BWAPI::Broodwar->printf("Enemy Found");
            enemyFound = true;
            m_scout->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
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
    if (typeOwned >= required)
    {
        return;
    }
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
    if (unit->getType() == marine && unit->isCompleted())
    {
    }
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
        if (workersBuilt == 5)
        {
            m_scout = unit;
            BWAPI::Broodwar->printf("scout role assigned");
        }
        if (workersBuilt == 6)
        {
            m_builder1 = unit;
            BWAPI::Broodwar->printf("builder1 role assigned");
        }
        if (workersBuilt == 7)
        {
            m_builder2 = unit;
            BWAPI::Broodwar->printf("builder2 role assigned");
        }
        if (workersBuilt == 8)
        {
            m_refinery1 = unit;
            BWAPI::Broodwar->printf("refinery1 role assigned");
        }
        if (workersBuilt == 9)
        {
            m_refinery2 = unit;
            BWAPI::Broodwar->printf("refinery2 role assigned");
        }
        if (workersBuilt == 10)
        {
            m_refinery3 = unit;
            BWAPI::Broodwar->printf("refinery3 role assigned");
        }
        if (workersBuilt == 11)
        {
            m_builder3 = unit;
            BWAPI::Broodwar->printf("builder3 role assigned");
        }
        if (workersBuilt == 12)
        {
            m_builder4 = unit;
            BWAPI::Broodwar->printf("builder4 role assigned");
        }
    }
    else if (unit->getType() == marine && unit->isCompleted())
    {
        marinesBuilt++;
        medCounter++;
        BWAPI::Broodwar->printf("marines: %d\n", marinesBuilt);
    }
    else if (unit->getType() == medic && unit->isCompleted())
    {
        medicsBuilt++;
        medCounter = 0;
        BWAPI::Broodwar->printf("medics: %d\n", medicsBuilt);
    }
    else if (unit->getType() == barrack && unit->isCompleted())
    {
        barracksBuilt++;
        BWAPI::Broodwar->printf("barrack: %d\n", barracksBuilt);
    }
    else if (unit->getType() == supply && unit->isCompleted())
    {
        supplyBuilt++;
    }
    else if (unit->getType() == refinery && unit->isCompleted())
    {
        refineryBuilt++;
    }
    else if (unit->getType() == academy && unit->isCompleted())
    {
        m_academy = unit;
        academyBuilt++;
    }
    else if (unit->getType() == engeneering && unit->isCompleted())
    {
        m_engeneering = unit;
        engeneeringBuilt++;
    }
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