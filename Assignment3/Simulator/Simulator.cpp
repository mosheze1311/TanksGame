#include "Simulator.h"
#include "../UserCommon/Utils/Timestamp.h"
#include "../UserCommon/Utils/GameResultUtils.h"
#include "../UserCommon/Utils/SatelliteViewUtils.h"
#include "../UserCommon/Config/ConfigReader.h"

#include <iostream>
#include <dlfcn.h>

// === Private Functions === //
// === Loading SO Files === //
void Simulator::loadSOFile(const std::string &filepath)
{
    void *handle = dlopen(filepath.c_str(), RTLD_NOW);
    if (!handle)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Cannot load .so file: " + filepath + " | dlerror: " + dlerror());
        return;
    }
    this->so_handlers.push_back(handle);
}

void Simulator::loadAlgorithmFile(const std::string &algorithm_filepath)
{
    fs::path abs_path = fs::absolute(algorithm_filepath).lexically_normal();
    Registrar<TankAlgorithmFactory>::getRegistrar().createEntry(abs_path);
    Registrar<PlayerFactory>::getRegistrar().createEntry(abs_path);

    loadSOFile(algorithm_filepath);

    try
    {
        Registrar<TankAlgorithmFactory>::getRegistrar().validateLastEntry();
        Registrar<PlayerFactory>::getRegistrar().validateLastEntry();
    }
    catch (InvalidRegistrationEntryException &e)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Algorithm/Player were not loaded correctly to registrar" + e.reason);
        Registrar<TankAlgorithmFactory>::getRegistrar().deleteLastEntry();
        Registrar<PlayerFactory>::getRegistrar().deleteLastEntry();
    }
}
void Simulator::loadManagerFile(const std::string &manager_filepath)
{
    Registrar<GameManagerFactory>::getRegistrar().createEntry(manager_filepath);

    loadSOFile(manager_filepath);

    try
    {
        Registrar<GameManagerFactory>::getRegistrar().validateLastEntry();
    }
    catch (InvalidRegistrationEntryException &e)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Manager was not loaded correctly to registrar");
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
        UserCommon_211388913_322330820::Logger::runtime().logLine("Failed to read map file: " + game_map_file);
    }
}

// === Comparative Mode === //
void Simulator::runComparativeMode()
{
    auto &comparative_args = dynamic_cast<ParsedComparativeCmdArguments &>(*args);

    readMapFile(comparative_args.getGameMap());
    if (maps_details.empty())
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("No valid maps loaded for comparative mode.\n");
        return;
    }

    // load managers
    for (const auto &entry : std::filesystem::directory_iterator(comparative_args.getGameManagerFolder()))
    {
        if (entry.path().extension() == ".so")
        {
            loadManagerFile(entry.path().string());
        }
    }
    if (Registrar<GameManagerFactory>::getRegistrar().count() < 1)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Could not load any managers, exiting...");
        return;
    }

    // load algorithms
    loadAlgorithmFile(comparative_args.getAlgorithm1());
    loadAlgorithmFile(comparative_args.getAlgorithm2());

    if (Registrar<TankAlgorithmFactory>::getRegistrar().count() != 2)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Could not load both algorithms, exiting...");
        return;
    }

    this->runManagersOnComparativeMode();

    // create output file
    std::string output = createComparativeOutput();
    this->logToOutputFile(output);
}

void Simulator::runManagersOnComparativeMode()
{
    size_t tasks_count = Registrar<GameManagerFactory>::getRegistrar().count();
    size_t num_threads = static_cast<size_t>(this->args->getNumThreads());
    ThreadPool pool(std::min(num_threads, tasks_count));
    for (auto manager_factory_entry : Registrar<GameManagerFactory>::getRegistrar())
    {
        auto runGame = [this, manager_factory_entry]()
        {
            this->runSingleComparativeManager(manager_factory_entry);
        };
        num_threads == 0 ? runGame() : pool.enqueueTask(runGame);
    }
}

void Simulator::runSingleComparativeManager(RegistrarEntry<GameManagerFactory> manager_factory_entry)
{

    // Extract map details
    auto &map_details = this->maps_details[0];
    size_t width = map_details.getWidth();
    size_t height = map_details.getHeight();
    size_t max_steps = map_details.getMaxSteps();
    size_t num_shells = map_details.getNumShells();
    std::string map_file_name = map_details.getFileName();

    // Create players
    auto player1_ptr = Registrar<PlayerFactory>::getRegistrar()[0].getFactory()(1, width, height, max_steps, num_shells);
    auto player1_name = Registrar<PlayerFactory>::getRegistrar()[0].getName();
    auto player2_ptr = Registrar<PlayerFactory>::getRegistrar()[1].getFactory()(2, width, height, max_steps, num_shells);
    auto player2_name = Registrar<PlayerFactory>::getRegistrar()[1].getName();

    // Extract algorithms factories
    auto algo1Factory = Registrar<TankAlgorithmFactory>::getRegistrar()[0].getFactory();
    auto algo2Factory = Registrar<TankAlgorithmFactory>::getRegistrar()[1].getFactory();

    // Create and run game manager
    auto manager = manager_factory_entry.getFactory()(this->args->isVerbose());
    GameResult res = manager->run(
        width, height,
        map_details.getSatelliteView(),
        map_file_name,
        max_steps, num_shells,
        *player1_ptr, player1_name, *player2_ptr, player2_name,
        algo1Factory, algo2Factory);

    // Insert result to map
    this->insertComparativeResult(manager_factory_entry.getName(), std::move(res));
}

void Simulator::insertComparativeResult(std::string key, GameResult res)
{
    std::lock_guard<std::mutex> lock(this->result_mutex);
    this->comparative_results[key] = std::move(res);
}

// === Comparative Output File === //
std::string Simulator::getComparativeOutputFileName() const
{
    auto comp_args = static_cast<ParsedComparativeCmdArguments &>(*(this->args));
    std::string managers_folder = comp_args.getGameManagerFolder();
    return managers_folder + "/comparative_results_" + UserCommon_211388913_322330820::getTimestampForNow() + ".txt";
}

std::string Simulator::createComparativeOutput() const
{
    auto &comparative_args = dynamic_cast<ParsedComparativeCmdArguments &>(*args);
    std::ostringstream oss;

    oss << "game_map=" << getNameForOutputFile(comparative_args.getGameMap()) << std::endl;
    oss << "algorithm1=" << getNameForOutputFile(comparative_args.getAlgorithm1()) << std::endl;
    oss << "algorithm2=" << getNameForOutputFile(comparative_args.getAlgorithm2()) << std::endl;
    oss << std::endl;

    std::vector<std::pair<std::string, std::vector<std::string>>> sorted_grouped_managers = createSortedGroupedManagers();
    //  print managers result groups
    for (size_t i = 0; i < sorted_grouped_managers.size(); ++i)
    {
        auto &[representative_manager, group] = sorted_grouped_managers[i];
        oss << createManagersGroupLine(representative_manager, group);

        if (i != sorted_grouped_managers.size() - 1)
            oss << std::endl;
    }
    return oss.str();
}

std::vector<std::pair<std::string, std::vector<std::string>>> Simulator::createSortedGroupedManagers() const
{
    /*
    This is a helper function for the createComparativeOutput() method
    */

    using namespace UserCommon_211388913_322330820::GameResultUtils; // use the namespace to get the GameResult == operator
    // first group the managers with equal result objects
    std::vector<std::pair<std::string, std::vector<std::string>>> sorted_grouped_managers;
    for (auto &[manager, result] : this->comparative_results)
    {
        bool group_exists = false;
        for (auto &[first_manager, equal_managers] : sorted_grouped_managers)
        {
            if (result == this->comparative_results.at(first_manager))
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

std::string Simulator::createManagersGroupLine(const std::string &representative_manager, const std::vector<std::string> &group) const
{
    std::ostringstream oss;
    for (size_t i = 0; i < group.size(); ++i)
    {
        std::string manager = group[i];
        std::string separator = i < group.size() - 1 ? ", " : "\n";
        oss << getNameForOutputFile(manager) << separator;
    }
    const GameResult &result = comparative_results.at(representative_manager);
    oss << UserCommon_211388913_322330820::GameResultUtils::toString(result);
    return oss.str();
}

// === Competitive Mode === //
void Simulator::runCompetitiveMode()
{
    auto &competitive_args = dynamic_cast<ParsedCompetitiveCmdArguments &>(*args);

    // read maps
    for (const auto &entry : std::filesystem::directory_iterator(competitive_args.getGameMapsFolder()))
    {
        if (entry.path().extension() == ".txt")
        {
            readMapFile(entry.path().string());
        }
    }

    // load manager
    loadManagerFile(competitive_args.getGameManager());
    if (Registrar<GameManagerFactory>::getRegistrar().count() != 1)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Could not load manager, exiting...");
        return;
    }

    // load algorithms
    for (const auto &entry : std::filesystem::directory_iterator(competitive_args.getAlgorithmsFolder()))
    {
        if (entry.path().extension() == ".so")
        {
            loadAlgorithmFile(entry.path().string());
        }
    }
    if (Registrar<TankAlgorithmFactory>::getRegistrar().count() < 2)
    {
        UserCommon_211388913_322330820::Logger::runtime().logLine("Could not load 2 algorithms, exiting...");
        return;
    }

    // run matches using threads
    this->runManagersOnCompetitiveMode();

    // create output file
    std::string output = createCompetitiveOutput();
    this->logToOutputFile(output);
}

void Simulator::runManagersOnCompetitiveMode()
{
    size_t N = Registrar<TankAlgorithmFactory>::getRegistrar().count();
    size_t K = this->maps_details.size();
    size_t tasks_count = K * N; // approximately
    size_t num_threads = static_cast<size_t>(this->args->getNumThreads());
    ThreadPool pool(std::min(num_threads, tasks_count)); 
    for (size_t k = 0; k < K; ++k)
    {
        for (size_t i = 0; i < N; ++i)
        {
            size_t j = (i + 1 + (k % (N - 1))) % N;

            if (((2 * (k + 1)) - N) % (2 * (N - 1)) == 0 && j < i) // skip double pairing
                continue;

            auto runGame = [this, k, i, j]()
            {
                this->runSingleCompetitiveMatch(k, i, j);
            };
            num_threads == 0 ? runGame() : pool.enqueueTask(runGame);
        }
    }
}

void Simulator::runSingleCompetitiveMatch(size_t map_idx, size_t algorithm1_entry_idx, size_t algorithm2_entry_idx)
{
    auto &map_details = this->maps_details[map_idx];
    std::string map_name = map_details.getFileName();
    size_t width = map_details.getWidth();
    size_t height = map_details.getHeight();
    size_t max_steps = map_details.getMaxSteps();
    size_t num_shells = map_details.getNumShells();

    auto algo1_entry = Registrar<TankAlgorithmFactory>::getRegistrar()[algorithm1_entry_idx];
    auto algo2_entry = Registrar<TankAlgorithmFactory>::getRegistrar()[algorithm2_entry_idx];
    auto algo1Factory = algo1_entry.getFactory();
    auto algo2Factory = algo2_entry.getFactory();

    auto player1_ptr = Registrar<PlayerFactory>::getRegistrar()[algorithm1_entry_idx].getFactory()(1, width, height, max_steps, num_shells);
    auto player1_name = Registrar<PlayerFactory>::getRegistrar()[0].getName();
    auto player2_ptr = Registrar<PlayerFactory>::getRegistrar()[algorithm2_entry_idx].getFactory()(2, width, height, max_steps, num_shells);
    auto player2_name = Registrar<PlayerFactory>::getRegistrar()[1].getName();

    auto manager_factory_entry = Registrar<GameManagerFactory>::getRegistrar()[0];
    auto manager = manager_factory_entry.getFactory()(this->args->isVerbose());
    GameResult res = manager->run(
        width, height,
        map_details.getSatelliteView(), map_name,
        max_steps, num_shells,
        *player1_ptr, player1_name, *player2_ptr, player2_name,
        algo1Factory, algo2Factory);

    if (res.winner == 0)
    {
        this->insertCompetitiveResult(algo1_entry.getName(), 1ul);
        this->insertCompetitiveResult(algo2_entry.getName(), 1ul);
    }
    else if (res.winner == 1)
    {
        this->insertCompetitiveResult(algo1_entry.getName(), 3ul);
        this->insertCompetitiveResult(algo2_entry.getName(), 0ul);
    }
    else // res.winner == 2
    {
        this->insertCompetitiveResult(algo1_entry.getName(), 0ul);
        this->insertCompetitiveResult(algo2_entry.getName(), 3ul);
    }
}

void Simulator::insertCompetitiveResult(std::string key, size_t points)
{
    std::lock_guard<std::mutex> lock(this->result_mutex);
    this->competitive_results[key] += points;
}

// === Competitive Output File === //
std::string Simulator::getCompetitiveOutputFileName() const
{
    auto comp_args = static_cast<ParsedCompetitiveCmdArguments &>(*(this->args));
    std::string algorithms_folder = comp_args.getAlgorithmsFolder();
    return algorithms_folder + "/competition_" + UserCommon_211388913_322330820::getTimestampForNow() + ".txt";
}

std::string Simulator::createCompetitiveOutput() const
{
    auto &competitive_args = dynamic_cast<ParsedCompetitiveCmdArguments &>(*args);
    std::ostringstream oss;
    oss << "game_maps_folder=" << getNameForOutputFile(competitive_args.getGameMapsFolder()) << std::endl;
    oss << "game_manager=" << getNameForOutputFile(competitive_args.getGameManager()) << std::endl;
    oss << std::endl;

    std::vector<std::pair<std::string, size_t>> sorted_algorithms(competitive_results.begin(), competitive_results.end());
    std::sort(sorted_algorithms.begin(), sorted_algorithms.end(), [](const auto &a, const auto &b)
              {
                  return a.second > b.second; // descending order by score
              });
    for (const auto &[algo_path, score] : sorted_algorithms)
    {
        oss << getNameForOutputFile(algo_path) << " " << score << std::endl;
    }

    return oss.str();
}

// === Shared Functions === //
std::string Simulator::getOutputFileName() const
{
    switch (args->getSimulatorMode())
    {
    case SimulatorMode::Competitive:
        return getCompetitiveOutputFileName();
    case SimulatorMode::Comparative:
        return getComparativeOutputFileName();
    default:
        throw std::logic_error("Simulator::getOutputFileName illegal mode"); // should not get here
    }
}

void Simulator::logToOutputFile(const std::string &output) const
{
    std::string output_mode_file = getOutputFileName();
    try
    {
        UserCommon_211388913_322330820::Logger::output(output_mode_file).log(output);
    }
    catch (const std::exception &e)
    {
        std::cout << "could not write simulator output to file" << std::endl;
        std::cout << output;
    }
}

void Simulator::closeRegistrars()
{
    Registrar<GameManagerFactory>::getRegistrar().clear();
    Registrar<TankAlgorithmFactory>::getRegistrar().clear();
    Registrar<PlayerFactory>::getRegistrar().clear();
}

void Simulator::closeSOHandlers()
{
    // clear dependencies
    this->closeRegistrars();
    this->comparative_results.clear();
    this->competitive_results.clear();

    // close so handlers
    for (void *handler : this->so_handlers)
    {
        dlclose(handler);
    }
}

std::string Simulator::getNameForOutputFile(const std::string &path) const
{
    bool is_full_name = UserCommon_211388913_322330820::ConfigReader::getConfig().getFullNamesOnOutput() == 1;
    if (is_full_name)
        return path;

    std::string filename = std::filesystem::path(path).filename().string();
    return filename;
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

    this->closeSOHandlers();
}
