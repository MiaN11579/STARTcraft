#include "StarterBot.h"
#include "Tools.h"
#include "MapTools.h"
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

StarterBot::StarterBot()
{
}

// Called when the bot starts!
void StarterBot::onStart()
{
    // Set our BWAPI options here
    BWAPI::Broodwar->setLocalSpeed(5);
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
    mineralCash = BWAPI::Broodwar->self()->minerals();
    mineralTotal = BWAPI::Broodwar->self()->gatheredMinerals();
    gasCash = BWAPI::Broodwar->self()->gas();
    gasTotal = BWAPI::Broodwar->self()->gatheredGas();
    supplyUsed = BWAPI::Broodwar->self()->supplyUsed() / 2;
    supplyTotal = BWAPI::Broodwar->self()->supplyTotal() / 2;
    // Update our MapTools information
    m_mapTools.onFrame();
    const BWAPI::Unit myDepot = Tools::GetDepot();
    sendIdleWorkersToMinerals();
    // BWAPI::Broodwar->printf("supplyUsed is %d\n", supplyUsed);
    // BWAPI::Broodwar->printf("supplyTotal is %d\n", supplyTotal);

    //  once we have enough workers to start with, send a a worker to scout.
    if (supplyUsed >= 8 && m_scout && (enemyFound == false || chokepoint == playerBase))
    {
        scout();
    }
    if (m_scout && enemyFound == true && chokepoint != playerBase && bunkersBuilt == 0 && bunkerPrice <= mineralCash)
    {
        buildBunker();
    }
    if (bunkersBuilt >= 1 && m_bunker && bunkerFilled == false)
    {
        assignMarinesToBunker();
    }

    // start of the build order.
    if (supplyUsed >= 4 && worker5 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
        worker5 = true;
        BWAPI::Broodwar->printf("worker 5 started");
    }

    else if (supplyUsed >= 5 && worker6 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
        worker6 = true;
        BWAPI::Broodwar->printf("worker 6 started");
    }

    else if (supplyUsed >= 6 && worker7 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
        worker7 = true;
        BWAPI::Broodwar->printf("worker 7 started");
    }

    else if (supplyUsed >= 7 && worker8 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
        worker8 = true;
        BWAPI::Broodwar->printf("worker 8 started");
    }

    else if (supplyUsed >= 8 && worker9 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
        worker9 = true;
        BWAPI::Broodwar->printf("worker 9 started");
    }

    else if (supplyUsed >= 9 && supplyPrice <= mineralCash && m_builder1 && worker10 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        build(supply, 1, m_builder1);
        myDepot->train(workerType);
        worker10 = true;
        BWAPI::Broodwar->printf("worker 10 started");
    }

    else if (supplyUsed >= 10 && worker11 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash && supplyBuilt == 1)
    {
        myDepot->train(workerType);
        worker11 = true;
        BWAPI::Broodwar->printf("worker 11 started");
    }

    if (supplyUsed >= 11)
    {
        if (barracksPrice <= mineralCash && m_builder2)
        {
            build(barrack, 1, m_builder2);
        }
        if (worker12 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
        {
            myDepot->train(workerType);
            worker12 = true;
            BWAPI::Broodwar->printf("worker 12 started");
        }
    }

    if (supplyUsed >= 12 && worker13 == false && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
        worker13 = true;
        BWAPI::Broodwar->printf("worker 13 started");
    }

    if (supplyUsed >= 13 && barracksPrice <= mineralCash && m_builder3)
    {
        build(barrack, 2, m_builder3);
    }

    if (supplyUsed >= 15 && supplyPrice <= mineralCash && m_builder4)
    {
        build(supply, 2, m_builder4);
    }

    if (supplyUsed >= 16 && refineryPrice <= mineralCash && m_refinery1)
    {
        build(refinery, 1, m_refinery1);
    }

    if (supplyUsed >= 17 && supplyPrice <= mineralCash && m_builder1)
    {
        build(supply, 3, m_builder1);
    }

    if (supplyUsed >= 20 && academyMinPrice <= mineralCash && academyGasPrice <= gasCash && m_builder2)
    {
        build(academy, 1, m_builder2);
    }

    if (supplyUsed >= 23 && supplyPrice <= mineralCash && m_builder3)
    {
        build(supply, 4, m_builder3);
    }

    if (supplyUsed >= 25 && supplyPrice <= mineralCash && m_builder4 && m_builder1)
    {
        build(supply, 5, m_builder4);
        build(engeneering, 1, m_builder1);
    }

    if (supplyUsed >= 28 && supplyPrice <= mineralCash && m_builder2)
    {
        build(barrack, 3, m_builder2);
    }

    if (supplyUsed >= 60)
    {
        build(supply, 7, m_builder4);
    }

    //switch from using supply to measure when to build things.
    if (barracksBuilt >= 3 && (factoryMinPrice + supplyPrice) <= mineralCash && factoryGasPrice <= gasCash && m_builder3 && m_builder4)
    {
        build(supply, 6, m_builder3);
        build(factory, 1, m_builder4);
    }

    if (factoryBuilt >= 1 && (factoryMinPrice + supplyPrice) <= mineralCash && factoryGasPrice <= gasCash && m_builder1 && m_builder2)
    {
        build(supply, 6, m_builder1);
        build(factory, 2, m_builder2);
    }

    if (factoryBuilt >= 2 && (supplyPrice + machineMinPrice) <= mineralCash && machineGasPrice <= gasCash && m_builder4 && m_factory1)
    {
        m_factory1->buildAddon(machine);
        build(supply, 7, m_builder3);
    }

    if (machineBuilt >= 1 && (supplyPrice + machineMinPrice) <= mineralCash && machineGasPrice <= gasCash && m_builder4 && m_factory2)
    {
        m_factory2->buildAddon(machine);
        build(supply, 8, m_builder4);
    }
    // End of the build order.

    // if the refinery has been built, send the designated workers to collect gas.
    if (refineryBuilt == 1 && m_refinery1 && m_refinery2 && m_refinery3 && m_refinery && refineryStart == false)
    {
        sendIdleWorkersToRefineries();
    }

    if (barracksBuilt >= 1)
    {
        if (academyBuilt >= 1 && medicsBuilt <= (marinesBuilt / 4))
        {
            train(medic);
        }
        if (attackWave < 2) // train marines for the first two attack waves
        {
            train(marine);
        }
        else
        {
            train(firebat);
        }
        attack();
    }

    if (engeneeringBuilt >= 1 && m_engeneering)
    {
        m_engeneering->upgrade(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
    }

    if (academyBuilt >= 1 && barracksBuilt >= 3 && m_academy && m_engeneering)
    {
        m_academy->upgrade(BWAPI::UpgradeTypes::Caduceus_Reactor);
        m_engeneering->upgrade(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
    }
    if (factoryBuilt >= 1)
    {
        if (machineBuilt >= 1 && tankMinPrice <= mineralCash && tankGasPrice <= gasCash)
        {
            factoryTrain(tank);
        }
        else
        {
            factoryTrain(vulture);
        }
    }

    if (workersBuilt <= 23 && myDepot && !myDepot->isTraining() && workerPrice <= mineralCash)
    {
        myDepot->train(workerType);
    }
}

// Send our idle workers to mine minerals so they don't just stand there
void StarterBot::sendIdleWorkersToMinerals()
{
    // Let's send all of our starting workers to the closest mineral to them
    // First we need to loop over all of the units that we (BWAPI::Broodwar->self()) own
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        // Check the unit type, if it is an idle worker, then we want to send it somewhere
        if (unit->getType().isWorker() && unit->isIdle())
        {
            // Get the closest mineral to this worker unit
            BWAPI::Unit closestMineral = Tools::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());

            // If a valid mineral was found, right click it with the unit in order to start harvesting
            if (closestMineral)
            {
                mineralsExhausted = false;
                unit->rightClick(closestMineral);
            }
            else
            {
                mineralsExhausted = true;
            }
        }
    }
}

// Send 3 workers to collect gas
void StarterBot::sendIdleWorkersToRefineries()
{
    m_refinery1->rightClick(m_refinery);
    m_refinery2->rightClick(m_refinery);
    m_refinery3->rightClick(m_refinery);
    refineryStart = true;
}

void StarterBot::attack()
{

    // initialize random seed
    srand((unsigned int)time(0));

    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        // Check the unit type, if it is an idle marine or medic, send it to attack
        if ((unit->getType() == marine || unit->getType() == medic || unit->getType() == firebat) || unit->getType() == vulture || unit->getType() == tank) // if this is a combat unit
        {
            if (unit->getDistance(playerBase) < 1000) // if unit is at player base
            {
                if (medCounter >= attackCount || mineralsExhausted == true) 
                {
                    unit->attack(enemyBase); // attack the enemy base
                }
            }
            else if (unit->getDistance(playerBase) > 1000 && unit->isIdle()) // if unit is outside player base and not doing anything
            {
                if (enemyBuildings.size() > 3) // if there are enemy units
                {
                    BWAPI::Broodwar->printf("Attack next enemy!");
                    unit->attack(enemyBuildings[0]);
                }
                else // if no enemy buildings in sight, search for one
                {
                    // generate random coordinates within 1000 radius of the enemy base
                    int x = rand() % 2000 - 1000;
                    int y = rand() % 2000 - 1000;
                    BWAPI::Position pos = BWAPI::Position(enemyBase.x + x, enemyBase.y + y).makeValid();
                    BWAPI::Broodwar->printf("Finding next enemy at %d, %d", pos.x, pos.y);
                    // scout around and attack anything on its way
                    unit->attack(pos);
                }
            }
        }
    }
    if (medCounter >= attackCount) // reset count
    {
        BWAPI::Broodwar->printf("Launch attack! Units: %d", attackCount);
        if (attackWave < 2)
        {
            attackWave++;
        }
        else if (attackCount < 35)
        {
            attackCount += 5;
        }
        medCounter = 0;
    }
}

// Train combat units in barracks
void StarterBot::train(BWAPI::UnitType type)
{
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        // Check the unit type, if it is an idle barrack, train more marine
        if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks && !unit->isTraining() && type.mineralPrice() <= mineralCash)
        {
            bool train = unit->train(type);
            if (train)
            {
                // BWAPI::Broodwar->printf("Started training %s", type.getName().c_str());
            }
        }
    }
}

void StarterBot::factoryTrain(BWAPI::UnitType type)
{
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        // Check the unit type, if it is an idle barrack, train more marine
        if (unit->getType() == BWAPI::UnitTypes::Terran_Factory && !unit->isTraining() && type.mineralPrice() <= mineralCash)
        {
            bool train = unit->train(type);
            if (train)
            {
                // BWAPI::Broodwar->printf("Started training %s", type.getName().c_str());
            }
        }
    }
}

void StarterBot::scout()
{
    if (enemyFound == false)
    {
        // if we find an enemy unit, mark the location and return scout to base.
        BWAPI::Unit pos_enemy = m_scout->getClosestUnit();
        if (BWAPI::Broodwar->self()->isEnemy(pos_enemy->getPlayer())) // if closest unit is an enemy
        {
            enemyBase = m_scout->getLastCommand().getTargetPosition(); // set enemy base as scout's last command position
            enemyFound = true;
            BWAPI::Broodwar->printf("Enemy Found: %d, %d", enemyBase.x, enemyBase.y);
            m_scout->move(playerBase); // move scout back to player base
            return;
        }
        // code for sending the scout around the map.
        auto& startLocations = BWAPI::Broodwar->getStartLocations();
        for (BWAPI::TilePosition tp : startLocations) // for each start location
        {
            BWAPI::TilePosition tp1 = BWAPI::TilePosition(tp.x + 5, tp.y + 5);
            BWAPI::TilePosition tp2 = BWAPI::TilePosition(tp.x - 5, tp.y + 5);
            BWAPI::TilePosition tp3 = BWAPI::TilePosition(tp.x + 5, tp.y - 5);
            BWAPI::TilePosition tp4 = BWAPI::TilePosition(tp.x - 5, tp.y - 5);
            std::deque<BWAPI::TilePosition> surroundings = { tp, tp1, tp2, tp3, tp4 }; // add all the surrounding tiles to the deque
            for (BWAPI::TilePosition tile : surroundings)
            {
                if (!BWAPI::Broodwar->isExplored(tile))
                {
                    BWAPI::Position pos(tile);
                    BWAPI::Broodwar->drawCircleMap(pos, 32, BWAPI::Colors::Red, true);
                    auto command = m_scout->getLastCommand();
                    if (command.getTargetPosition() == pos)
                    {
                        return;
                    }
                    m_scout->move(pos);
                    return;
                }
            }
        }
    }
    else if (chokepoint == playerBase) // if chokepoint has not been set
    {
        BWAPI::Broodwar->drawCircleMap(enemyBase, 32, BWAPI::Colors::Red, true);
        int distance = m_scout->getDistance(playerBase);
        if (distance <= 700)
        {
            chokepoint = m_scout->getPosition(); // set chokepoint to the position of the scout
            BWAPI::Broodwar->printf("Chokepoint x: %d, Chokepoint y: %d", chokepoint.x, chokepoint.y);
        }
        if (distance <= 750)
        {
            bunkerPos = m_scout->getPosition();
        }
    }
}

void StarterBot::assignMarinesToBunker()
{
    int count = 0;
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        if (unit->getType() == marine && count < 4)
        {
            unit->rightClick(m_bunker);
            count++;
        }
        if (count >= 4)
        {
            bunkerFilled = true;
            break;
        }
    }
}

// Build given type of building to given required number
void StarterBot::build(BWAPI::UnitType type, int required, BWAPI::Unit builder)
{
    const int typeOwned = Tools::CountUnitsOfType(type, BWAPI::Broodwar->self()->getUnits());
    // If we have a sufficient number of buildings, we don't need to do anything
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
        BWAPI::Broodwar->printf("Started Building %s %d", type.getName().c_str(), typeOwned + 1);
    }
}

void StarterBot::buildBunker()
{
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition(bunkerPos);

    int maxBuildRange = 12;
    BWAPI::TilePosition buildPos = BWAPI::Broodwar->getBuildLocation(bunker, desiredPos, maxBuildRange, false);
    const bool startedBuilding = m_scout->build(bunker, buildPos);
    if (startedBuilding)
    {
        BWAPI::Broodwar->printf("Started Building %s", bunker.getName().c_str());
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
        marinesBuilt--;
    }
    else if (unit->getType() == medic && unit->isCompleted())
    {
        medicsBuilt--;
    }
    else if (unit->getType() == firebat && unit->isCompleted())
    {
        firebatsBuilt--;
    }
    else if (unit->getType() == vulture && unit->isCompleted())
    {
        vulturesBuilt--;
    }
    else if (unit->getType() == tank && unit->isCompleted())
    {
        tanksBuilt--;
    }
    if (unit->getPlayer() == BWAPI::Broodwar->enemy() && unit->getType().isBuilding())
    {
        for (unsigned int i = 0; i < enemyBuildings.size(); i++)
        {
            if (enemyBuildings[i] == unit)
            {
                BWAPI::Broodwar->printf("Enemy building removed");
                enemyBuildings.erase(enemyBuildings.begin() + i);
                break;
            }
        }
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
    if (unit->getType() == workerType)
    {
        workersBuilt++;
        BWAPI::Broodwar->printf("worker: %d\n", workersBuilt);

        if (workersBuilt == 5)
        {
            m_builder1 = unit;
            BWAPI::Broodwar->printf("builder1 role assigned");
        }
        else if (workersBuilt == 6)
        {
            m_builder2 = unit;
            BWAPI::Broodwar->printf("builder2 role assigned");
        }
        else if (workersBuilt == 7)
        {
            m_builder3 = unit;
            BWAPI::Broodwar->printf("builder3 role assigned");
        }
        else if (workersBuilt == 8)
        {
            m_scout = unit;
            BWAPI::Broodwar->printf("scout role assigned");
        }
        else if (workersBuilt == 9)
        {
            m_builder4 = unit;
            BWAPI::Broodwar->printf("builder4 role assigned");
        }
        else if (workersBuilt == 10)
        {
            m_refinery1 = unit;
            BWAPI::Broodwar->printf("refinery1 role assigned");
        }
        else if (workersBuilt == 11)
        {
            m_refinery2 = unit;
            BWAPI::Broodwar->printf("refinery2 role assigned");
        }
        else if (workersBuilt == 12)
        {
            m_refinery3 = unit;
            BWAPI::Broodwar->printf("refinery3 role assigned");
        }
    }
    else if (unit->getType() == marine)
    {
        marinesBuilt++;
        medCounter++;
        unit->attack(chokepoint);
        BWAPI::Broodwar->printf("marines: %d\n", marinesBuilt);
    }
    else if (unit->getType() == medic)
    {
        medicsBuilt++;
        unit->attack(chokepoint);
        BWAPI::Broodwar->printf("medics: %d\n", medicsBuilt);
    }
    else if (unit->getType() == firebat)
    {
        firebatsBuilt++;
        medCounter++;
        unit->attack(chokepoint);
        BWAPI::Broodwar->printf("firebat: %d\n", firebatsBuilt);
    }
    else if (unit->getType() == vulture)
    {
        vulturesBuilt++;
        medCounter++;
        unit->attack(chokepoint);
        BWAPI::Broodwar->printf("raptor: %d\n", vulturesBuilt);
    }
    else if (unit->getType() == tank)
    {
        tanksBuilt++;
        medCounter++;
        unit->attack(chokepoint);
        BWAPI::Broodwar->printf("tank: %d\n", tanksBuilt);
    }
    else if (unit->getType() == barrack)
    {
        barracksBuilt++;
        if (barracksBuilt == 1)
        {
            m_barracks1 = unit;
        }
        else if (barracksBuilt == 2)
        {
            m_barracks2 = unit;
        }
        BWAPI::Broodwar->printf("barrack: %d\n", barracksBuilt);
    }
    else if (unit->getType() == factory)
    {
        factoryBuilt++;
        if (factoryBuilt == 1)
        {
            m_factory1 = unit;
        }
        else if (factoryBuilt == 2)
        {
            m_factory2 = unit;
        }
        BWAPI::Broodwar->printf("factory: %d\n", factoryBuilt);
    }
    else if (unit->getType() == supply)
    {
        supplyBuilt++;
    }
    else if (unit->getType() == refinery)
    {
        refineryBuilt++;
        m_refinery = unit;
        sendIdleWorkersToRefineries();
    }
    else if (unit->getType() == academy)
    {
        m_academy = unit;
        academyBuilt++;
    }
    else if (unit->getType() == engeneering)
    {
        m_engeneering = unit;
        engeneeringBuilt++;
    }
    else if (unit->getType() == bunker)
    {
        m_bunker = unit;
        bunkersBuilt++;
    }
    else if (unit->getType() == machine)
    {
        machineBuilt++;
    }
}

// Called whenever a unit appears, with a pointer to the destroyed unit
// This is usually triggered when units appear from fog of war and become visible
void StarterBot::onUnitShow(BWAPI::Unit unit)
{
    if (unit->getPlayer() == BWAPI::Broodwar->enemy() && unit->getType().isBuilding()) // if unit is an enemy building
    {
        for (unsigned int i = 0; i < enemyBuildings.size(); i++)
        {
            if (enemyBuildings[i] == unit)
            {
                return;
            }
        }
        BWAPI::Broodwar->printf("Enemy building added");
        enemyBuildings.push_back(unit);
    }
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