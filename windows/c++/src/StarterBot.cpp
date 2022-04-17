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
    BWAPI::Broodwar->setLocalSpeed(0);
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

    if (supplyUsed >= 25 && supplyPrice <= mineralCash && m_builder1 && m_builder2)
    {
        build(supply, 5, m_builder1);
        build(engeneering, 1, m_builder2);
    }

    if (supplyUsed >= 28 && supplyPrice <= mineralCash && m_builder3)
    {
        build(barrack, 3, m_builder3);
    }
    // End of the build order.

    // if the refinery has been built, send the designated workers to collect gas.
    if (refineryBuilt == 1 && m_refinery1 && m_refinery2 && m_refinery3 && refineryStart == false)
    {
        sendIdleWorkersToRefineries();
    }

    if (barracksBuilt >= 1)
    {
        if (academyBuilt >= 1 && medicsBuilt <= (marinesBuilt / 4))
        {
            train(medic);
        }
        if (attackWave < 2)
        {
            train(marine);
        }
        else
        {
            train(firebat);
        }
        attack();
    }

    if (engeneeringBuilt >= 1)
    {
        m_engeneering->upgrade(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
    }

    if (academyBuilt >= 1 && barracksBuilt >= 3)
    {
        m_academy->upgrade(BWAPI::UpgradeTypes::U_238_Shells);
        m_engeneering->upgrade(BWAPI::UpgradeTypes::Terran_Infantry_Armor);
    }

    /*
    // after the first barracks is built, but before the academy is built, just keep
    // pumping out marines form the barracks.
    if (barracksBuilt >= 1 && m_barracks1 && academyBuilt == 0 && !m_barracks1->isTraining() && supplyUsed < supplyTotal && marinePrice <= mineralCash)
    {
        m_barracks1->train(marine);
        if (barracksBuilt == 2 && m_barracks2 && !m_barracks2->isTraining() && supplyUsed < supplyTotal && marinePrice <= mineralCash)
        {
            m_barracks2->train(marine);
        }
    }
    // otherwise, we want to build marines and medics, with a 4/1 ratio
    else if (academyBuilt == 1 && supplyUsed <= supplyTotal)
    {
        if (medicsBuilt <= (marinesBuilt / 4) && medicMinPrice <= mineralCash && medicGasPrice <= gasCash)
        {
            if (!m_barracks1->isTraining())
            {
                m_barracks1->train(medic);
            }
            else if (!m_barracks2->isTraining())
            {
                m_barracks2->train(medic);
            }
        }
        else if (marinePrice <= mineralCash)
        {
            if (!m_barracks1->isTraining())
            {
                m_barracks1->train(marine);
            }
            else if (!m_barracks2->isTraining())
            {
                m_barracks2->train(marine);
            }
        }
    }*/
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
    m_refinery1->rightClick(m_refinery);
    m_refinery2->rightClick(m_refinery);
    m_refinery3->rightClick(m_refinery);
    refineryStart = true;
}

BWAPI::Unit StarterBot::getenemyInRegion()
{
    for (auto &unit : BWAPI::Broodwar->enemy()->getUnits())
    {
        // if it's an overlord, don't worry about it for defense, we don't care what they see
        if (unit->getType() == BWAPI::UnitTypes::Zerg_Overlord)
        {
            continue;
        }

        if (unit->getDistance(playerBase) < 800) // if the unit is close to our base
        {
            BWAPI::Broodwar->printf("Enemy in region");
            return unit;
        }
    }
    return nullptr;
}

void StarterBot::attack()
{
    BWAPI::Unit enemyInRegion = getenemyInRegion(); // check if there is an enemy in our region

    // initialize random seed
    srand((unsigned int)time(0));

    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Check the unit type, if it is an idle marine or medic, send it to attack
        if ((unit->getType() == marine || unit->getType() == medic || unit->getType() == firebat)) // if this is a combat unit
        {
            if (unit->getDistance(playerBase) < 800) // if unit is at player base
            {
                if (enemyInRegion != nullptr) // if base is under attack
                {
                    BWAPI::Broodwar->printf("Protect base!");
                    unit->attack(enemyInRegion); // attack the enemy in region
                }
                else if (medCounter >= 20) // if base is fine
                {
                    unit->attack(enemyBase); // attack the enemy base
                }
            }
            else if (unit->getDistance(playerBase) > 1000 && unit->isIdle()) // if unit is outside player base and not doing anything
            {
                if (enemyBuildings.empty()) // if there are enemy units
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
                    BWAPI::Broodwar->printf("%d, %d, Finding next enemy at %d, %d", x, y, pos.x, pos.y);
                    // scout around and attack anything on its way
                    unit->attack(pos);
                }
            }
        }
    }
    if (medCounter >= 20 && enemyInRegion == nullptr) // reset count and attack enemy if base is not under attack
    {
        BWAPI::Broodwar->printf("Launch attack!");
        if (attackWave < 2)
        {
            attackWave++;
        }
        medCounter = 0;
    }
}

// Train combat units in barracks
void StarterBot::train(BWAPI::UnitType type)
{
    const BWAPI::Unitset &myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto &unit : myUnits)
    {
        // Check the unit type, if it is an idle barrack, train more marine
        if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks && !unit->isTraining() && type.mineralPrice() <= mineralTotal)
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
        if (BWAPI::Broodwar->self()->isEnemy(pos_enemy->getPlayer()))
        {
            enemyBase = m_scout->getLastCommand().getTargetPosition();
            BWAPI::Broodwar->printf("Enemy Found, Enemy Base: %d, %d", enemyBase.x, enemyBase.y);
            enemyFound = true;
            m_scout->move(playerBase);
            return;
        }
        // code for sending the scout around the map.
        auto &startLocations = BWAPI::Broodwar->getStartLocations();
        for (BWAPI::TilePosition tp : startLocations) // for each start location
        {
            BWAPI::TilePosition tp1 = BWAPI::TilePosition(tp.x + 5, tp.y + 5);
            BWAPI::TilePosition tp2 = BWAPI::TilePosition(tp.x - 5, tp.y + 5);
            BWAPI::TilePosition tp3 = BWAPI::TilePosition(tp.x + 5, tp.y - 5);
            BWAPI::TilePosition tp4 = BWAPI::TilePosition(tp.x - 5, tp.y - 5);
            std::deque<BWAPI::TilePosition> surroundings = {tp, tp1, tp2, tp3, tp4}; // add all the surrounding tiles to the deque
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
        if (distance <= 600)
        {
            chokepoint = m_scout->getPosition(); // set chokepoint to the position of the scout
            BWAPI::Broodwar->printf("Chokepoint x: %d, Chokepoint y: %d", chokepoint.x, chokepoint.y);
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
    else if (unit->getType() == supply && unit->isCompleted())
    {
        supplyBuilt++;
    }
    else if (unit->getType() == refinery && unit->isCompleted())
    {
        refineryBuilt++;
        m_refinery = unit;
        sendIdleWorkersToRefineries();
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
    if (unit->getPlayer() == BWAPI::Broodwar->enemy() && unit->getType().isBuilding())
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