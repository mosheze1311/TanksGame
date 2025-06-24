#include "Simulator.h"
#include "Registrar/Registrar.h"

#include <dlfcn.h>
#include <iostream>

using namespace UserCommon_211388913_322330820;
// === Private Functions === //
// === Loading SO Files === //
void Simulator::loadSOFile(const std::string &filepath)
{
    void *handle = dlopen(filepath.c_str(), RTLD_NOW);
    if (!handle)
    {
        Logger::runtime().logLine("Cannot load .so file: " + filepath);
        return;
    }

    // Print the loaded file name
    std::cout << "Loaded file: " << filepath << '\n';
}
void Simulator::loadAlgorithmFile(const std::string &algorithm_file)
{
    Registrar<TankAlgorithmFactory>::getRegistrar().createEntry(algorithm_file);
    Registrar<PlayerFactory>::getRegistrar().createEntry(algorithm_file);

    loadSOFile(algorithm_file);

    try
    {
        Registrar<TankAlgorithmFactory>::getRegistrar().validateLastEntry();
        Registrar<PlayerFactory>::getRegistrar().validateLastEntry();
    }
    catch (InvalidRegistrationEntryException &e)
    {
        Logger::runtime().logLine("Algorithm/Player were not loaded correctly to registrar");
        Registrar<TankAlgorithmFactory>::getRegistrar().deleteLastEntry();
        Registrar<PlayerFactory>::getRegistrar().deleteLastEntry();
    }
}
void Simulator::loadManagerFile(const std::string &manager_file)
{
    Registrar<GameManagerFactory>::getRegistrar().createEntry(manager_file);

    loadSOFile(manager_file);

    try
    {
        Registrar<GameManagerFactory>::getRegistrar().validateLastEntry();
    }
    catch (InvalidRegistrationEntryException &e)
    {
        Logger::runtime().logLine("Manager was not loaded correctly to registrar");
        Registrar<GameManagerFactory>::getRegistrar().deleteLastEntry();
    }
}

// === Read Maps === //
void Simulator::readMapFile(const std::string &game_map_file)
{
    try
    {
        maps_details.emplace_back(game_map_file);
    }
    catch (const MapReadException &e)
    {
        Logger::runtime().logLine("Failed to read map file: " + game_map_file);
    }
}

// === Run Modes === //
void Simulator::runComparativeMode()
{
    auto &comparative_args = dynamic_cast<ParsedComparativeCmdArguments &>(*args);

    readMapFile(comparative_args.getGameMap());
    if (maps_details.empty())
    {
        Logger::runtime().logLine("No valid maps loaded for comparative mode.\n");
        return;
    }

    // load managers
    for (const auto &entry : std::filesystem::directory_iterator(comparative_args.getGameManagerFolders()))
    {
        if (entry.path().extension() == ".so")
        {
            loadManagerFile(entry.path().string());
        }
    }

    // load algorithms
    loadAlgorithmFile(comparative_args.getAlgorithm1());
    loadAlgorithmFile(comparative_args.getAlgorithm2());

    // TODO: run all with threads
}

void Simulator::runCompetitiveMode()
{
    auto &competitive_args = dynamic_cast<ParsedCompetitiveCmdArguments &>(*args);

    for (const auto &entry : std::filesystem::directory_iterator(competitive_args.getGameMapsFolder()))
    {
        if (entry.path().extension() == ".txt")
        {
            readMapFile(entry.path().string());
        }
    }

    // load manager
    loadManagerFile(competitive_args.getGameManager());

    // load algorithms
    for (const auto &entry : std::filesystem::directory_iterator(competitive_args.getAlgorithmsFolder()))
    {
        if (entry.path().extension() == ".so")
        {
            loadAlgorithmFile(entry.path().string());
        }
    }

    // TODO: run all with threads
}

// === Public Functions == //

// === Consturctor === //
Simulator::Simulator(std::unique_ptr<ParsedCmdArguments> args)
    : args(std::move(args)) {}

// === Public API === //
void Simulator::run()
{
    std::cout << "Running Simulator with Args:\n";
    std::cout << args->toString();

    switch (args->getSimulatorMode())
    {
    case SimulatorMode::Comparative:
        runComparativeMode();
        break;
    case SimulatorMode::Competitive:
        runCompetitiveMode();
        break;
    default:
        std::cerr << "Unknown simulator mode.\n";
        break;
    }
}
