#include "Simulator.h"
#include "Registrar/Registrar.h"

#include "../UserCommon/Utils/Timestamp.h"
#include "../UserCommon/Utils/GameResultUtils.h"
#include "../UserCommon/Utils/SatelliteViewUtils.h"

#include <dlfcn.h>
#include <iostream> // TODO: delete this include

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
    this->runManagersOnComparativeMode();

    // create output file
    createComparativeOutput();
}
void Simulator::runManagersOnComparativeMode()
{
    GameResult a{};
    BoardSatelliteView a_view = this->maps_details[0].getSatelliteView();
    a.winner = 1;
    a.reason = GameResult::ALL_TANKS_DEAD;
    a.remaining_tanks = {1, 0};
    a.gameState = std::make_unique<BoardSatelliteView>(a_view);
    GameResult b{};
    BoardSatelliteView b_view = this->maps_details[0].getSatelliteView();
    b.winner = 1;
    b.reason = GameResult::ALL_TANKS_DEAD;
    b.gameState = std::make_unique<BoardSatelliteView>(b_view);
    b.remaining_tanks = {1, 0};
    GameResult c{};
    BoardSatelliteView c_view = this->maps_details[0].getSatelliteView();
    c.winner = 2;
    c.reason = GameResult::ALL_TANKS_DEAD;
    c.gameState = std::make_unique<BoardSatelliteView>(c_view);
    c.remaining_tanks = {0, 1};
    GameResult d{};
    BoardSatelliteView d_view = this->maps_details[0].getSatelliteView();
    d.winner = 0;
    d.reason = GameResult::ALL_TANKS_DEAD;
    d.gameState = std::make_unique<BoardSatelliteView>(d_view);
    d.remaining_tanks = {0, 0};

    for (auto manager_factory_entry : Registrar<GameManagerFactory>::getRegistrar())
    {
        auto manager = manager_factory_entry.getFactory()(true);
        auto player_ptr = Registrar<PlayerFactory>::getRegistrar()[0].getFactory()(1, 1ul, 1ul, 1ul, 1ul);
        auto algoFactory = Registrar<TankAlgorithmFactory>::getRegistrar()[0].getFactory();
        auto sat_view = this->maps_details[0].getSatelliteView();
                            GameResult e = manager->run(this->maps_details[0].getWidth(), this->maps_details[0].getHeight(),
                                                        sat_view,
                                                        this->maps_details[0].getMaxSteps(), this->maps_details[0].getNumShells(),
                                                        *player_ptr, *player_ptr, algoFactory, algoFactory);
        comparative_results["fifth"] = std::move(e);

        break;
    }

    comparative_results["first"] = std::move(a);
    comparative_results["third"] = std::move(c);
    comparative_results["second"] = std::move(b);
    comparative_results["fourth"] = std::move(d);
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

    // TODO: run all with threads and update scores

    // create output file
    createCompetitiveOutput();
}
void Simulator::runManagersOnCompetitiveMode()
{
    competitive_results["algo1"] = 10;
    competitive_results["algo2"] = 40;
    competitive_results["algo3"] = 15;
    competitive_results["algo4"] = 12;
    competitive_results["algo5"] = 0;
    competitive_results["algo6"] = 13;
}

// === Output Files === //
std::string Simulator::getComparativeOutputFileName() const
{
    return "comparative_results_" + getTimestampForNow() + ".txt";
}
std::string Simulator::getCompetitiveOutputFileName() const
{
    return "competition_" + getTimestampForNow() + ".txt";
}
std::vector<std::pair<std::string, std::vector<std::string>>> Simulator::createSortedGroupedManagers(const std::map<std::string, GameResult> &managers_results) const
{
    /*
    This is a helper function for the createComparativeOutput() method
    */

    using namespace UserCommon_211388913_322330820::GameResultUtils; // use the namespace to get the GameResult == operator

    // first group the managers with equal result objects
    std::vector<std::pair<std::string, std::vector<std::string>>> sorted_grouped_managers;
    for (auto &[manager, result] : managers_results)
    {
        bool group_exists = false;
        for (auto &[first_manager, equal_managers] : sorted_grouped_managers)
        {
            if (result == managers_results.at(first_manager))
            {
                group_exists = true;
                equal_managers.push_back(manager);
                break;
            }
        }
        if (!group_exists)
        {
            sorted_grouped_managers.emplace_back(manager, std::vector<std::string>({manager}));
        }
    }

    // sort the vector in decsending order by amount of managers that share the same reesult (size of the inner vector)
    std::sort(sorted_grouped_managers.begin(), sorted_grouped_managers.end(),
              [](const auto &a, const auto &b)
              {
                  return a.second.size() > b.second.size();
              });

    return sorted_grouped_managers;
}
void Simulator::logManagerGroup(const std::string &representative_manager, const std::vector<std::string> &group,
                                const std::string &comparative_output_file) const
{
    for (size_t i = 0; i < group.size(); ++i)
    {
        std::string manager = group[i];
        std::string separator = i < group.size() - 1 ? ", " : "\n";
        Logger::output(comparative_output_file).log(manager + separator);
    }

    const GameResult &result = comparative_results.at(representative_manager);
    Logger::output(comparative_output_file).log(GameResultUtils::toString(result));
}

void Simulator::createComparativeOutput() const
{
    auto &comparative_args = dynamic_cast<ParsedComparativeCmdArguments &>(*args);
    std::string comparative_output_file = getComparativeOutputFileName();

    Logger::output(comparative_output_file).logLine("game_map=" + comparative_args.getGameMap());
    Logger::output(comparative_output_file).logLine("algorithm1=" + comparative_args.getAlgorithm1());
    Logger::output(comparative_output_file).logLine("algorithm2=" + comparative_args.getAlgorithm2());
    Logger::output(comparative_output_file).logLine("");

    std::vector<std::pair<std::string, std::vector<std::string>>> sorted_grouped_managers = createSortedGroupedManagers(comparative_results);
    //  print managers result groups
    for (size_t i = 0; i < sorted_grouped_managers.size(); ++i)
    {
        auto &[representative_manager, group] = sorted_grouped_managers[i];
        logManagerGroup(representative_manager, group, comparative_output_file);

        if (i != sorted_grouped_managers.size() - 1)
            Logger::output(comparative_output_file).logLine("");
    }
}

void Simulator::createCompetitiveOutput() const
{
    auto &competitive_args = dynamic_cast<ParsedCompetitiveCmdArguments &>(*args);
    std::string competition_output_file = getCompetitiveOutputFileName();

    Logger::output(competition_output_file).logLine("game_maps_folder=" + competitive_args.getGameMapsFolder());
    Logger::output(competition_output_file).logLine("game_manager=" + competitive_args.getGameManager());
    Logger::output(competition_output_file).logLine("");

    std::vector<std::pair<std::string, size_t>> sorted_algorithms(competitive_results.begin(), competitive_results.end());
    std::sort(sorted_algorithms.begin(), sorted_algorithms.end(), [](const auto &a, const auto &b)
              {
                  return a.second > b.second; // descending order by score
              });
    for (const auto &entry : sorted_algorithms)
    {
        Logger::output(competition_output_file).logLine(entry.first + " " + std::to_string(entry.second));
    }
}

// === Public Functions == //
// === Consturctor === //
Simulator::Simulator(std::unique_ptr<ParsedCmdArguments> args)
    : args(std::move(args))
{
}

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
