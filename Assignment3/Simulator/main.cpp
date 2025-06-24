#include "Simulator.h"
#include "ConsoleArgumentsParser.h"

#include <string>
#include <set>
#include <map>
#include <iostream>
#include <memory>

void clearConsole()
{
    std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
}

int main(int argc, const char *argv[])
{
    clearConsole();
    auto ptr = ConsoleArgumentsParser(argc, argv).getParsedArguments();
    if (ptr == nullptr){
        return 0;
    }

    Simulator simulator(std::move(ptr));
    simulator.run();
    return 0;

}


// === CORRECT IN ORDER === //
// ./simulator_211388913_322330820 -competition game_maps_folder="maps" game_manager="managers/manager1.tmp" algorithms_folder="algorithms" num_threads=1 -verbose
// ./simulator_211388913_322330820 -comparative game_map="maps\map1.tmp" game_managers_folder="managers" algorithm1="algorithms/algo1.tmp" algorithm2="algorithms/algo2.tmp" num_threads=1 -verbose

// === CORRECT OUT OF ORDER === //
// ./simulator_211388913_322330820 -verbose game_maps_folder="maps" -competition algorithms_folder="algorithms" num_threads=1 game_manager="managers/manager1.tmp"
// ./simulator_211388913_322330820 game_map="maps\map1.tmp" -comparative algorithm1="algorithms/algo1.tmp" algorithm2="algorithms/algo2.tmp" num_threads=1 -verbose game_managers_folder="managers"

// === WRONG OR MISSING MODE FLAG === //
// ./simulator_211388913_322330820 game_maps_folder="maps" game_manager="managers/manager1.tmp" algorithms_folder="algorithms" num_threads=1 -verbose;
// ./simulator_211388913_322330820 -nononono game_maps_folder="maps" game_manager="managers/manager1.tmp" algorithms_folder="algorithms" num_threads=1 -verbose;

// === MISSING ATTRIBUTE === //
// ./simulator_211388913_322330820 -competition game_maps_folder="maps" algorithms_folder="algorithms" num_threads=1 -verbose;
// ./simulator_211388913_322330820 -comparative game_map="maps\map1.tmp" game_managers_folder="managers" algorithm2="algorithms/algo2.tmp" num_threads=1 -verbose

// === ADDITIONAL ATTRIBUTE === //
// ./simulator_211388913_322330820 -competition a=a game_maps_folder="maps" game_manager="managers/manager1.tmp" algorithms_folder="algorithms" num_threads=1 -verbose;
// ./simulator_211388913_322330820 -comparative game_map="maps\map1.tmp" b=b game_managers_folder="managers" algorithm1="algorithms/algo1.tmp" algorithm2="algorithms/algo2.tmp" num_threads=1 -verbose

// === MISSING OPTIONAL num_threads === //
// ./simulator_211388913_322330820 -competition game_maps_folder="maps" game_manager="managers/manager1.tmp" algorithms_folder="algorithms" -verbose;
// ./simulator_211388913_322330820 -comparative game_map="maps\map1.tmp" game_managers_folder="managers" algorithm1="algorithms/algo1.tmp" algorithm2="algorithms/algo2.tmp" -verbose

// === MISSING OPTIONAL -verbose === //
// ./simulator_211388913_322330820 -competition game_maps_folder="maps" game_manager="managers/manager1.tmp" algorithms_folder="algorithms" num_threads=1;
// ./simulator_211388913_322330820 -comparative game_map="maps\map1.tmp" game_managers_folder="managers" algorithm1="algorithms/algo1.tmp" algorithm2="algorithms/algo2.tmp" num_threads=1