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
    std::cout << "\033c";         // THIS RESET THE BOARD - MAY CAUSE PROBLEMS
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
// ./simulator_211388913_322330820 -competition game_maps_folder="Maps" game_manager="Managers/GameManager_211388913_322330820.so" algorithms_folder="Algorithms" num_threads=1 -verbose
// ./simulator_211388913_322330820 -comparative game_map="maps\map1.tmp" game_managers_folder="managers" algorithm1="algorithms/algo1.tmp" algorithm2="algorithms/algo2.tmp" num_threads=1 -verbose