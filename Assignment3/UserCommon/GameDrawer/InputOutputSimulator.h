#include "GameDrawer.h"
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../Utils/ActionRequestUtils.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../../Simulator/MapFileParser/MapFileParser.h"

#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <sstream>
#include <vector>
using namespace UserCommon_211388913_322330820;


class InputOutputSimulator
{
private:
    class OutputParser
    {
    private:
        std::ifstream manager_output_file;

    public:
        OutputParser(std::string manager_output_file) : manager_output_file(manager_output_file) {};

        std::vector<std::optional<ActionRequest>> getNextStepActionRequests()
        {
            std::string line;
            std::vector<std::optional<ActionRequest>> actions;
            // Read a non-empty line

            if (!std::getline(manager_output_file, line) || line.empty())
                return actions;

            std::stringstream ss(line);
            std::string word;

            // Vector to store all words from the line
            std::vector<std::string> words;

            while (std::getline(ss, word, ','))
            {
                // Trim leading/trailing whitespace
                word.erase(0, word.find_first_not_of(" \t\r\n"));
                word.erase(word.find_last_not_of(" \t\r\n") + 1);

                // Split by space and take only the first word
                std::istringstream word_stream(word);
                std::string first_word;
                word_stream >> first_word;

                bool found = false;
                for (int i = 0; i < 9; ++i)
                {
                    ActionRequest request = static_cast<ActionRequest>(i);
                    std::string request_string = ActionRequestUtils::actionToString(request);
                    if (first_word == request_string)
                    {
                        actions.push_back(request);
                        found = true;
                    }
                }

                if (!found)
                {
                    actions.push_back(std::nullopt);
                }
            }

            return actions;
        }
    };

public:
    InputOutputSimulator(std::string input_file, std::string output_file, DrawingType dt)
    {
        MapFileParser map_details(input_file);
        OutputParser output_parser(output_file);
        GameBoard b;
        b.initFromDetails(map_details.getSatelliteView(), map_details.getWidth(), map_details.getHeight(), map_details.getMaxSteps(), map_details.getNumShells());

        std::vector<Tank *> tanks = b.getAllTanksOrderedByCell();

        GameDrawer drawer(b, dt);
        drawer.draw();
        GameCollisionHandler h(b);
        std::vector<std::optional<ActionRequest>> next_actions = output_parser.getNextStepActionRequests();
        while (next_actions.size() != 0)
        {
            for (size_t i = 0; i < next_actions.size(); ++i)
            {
                auto action = next_actions[i];
                if (action == std::nullopt)
                    continue;

                tanks[i]->playTankRound(*action);
            }
            h.handleCollisions();
            drawer.draw();
            for (size_t shell_step = 0; shell_step < ConfigReader::getConfig().getShellsSpeed(); ++shell_step)
            {
                for (GameObject *obj : b.getGameObjects(GameObjectType::SHELL))
                {
                    Shell *shell = dynamic_cast<Shell *>(obj);
                    shell->advance();
                }
                h.handleCollisions();
                drawer.draw();
            }
            next_actions = output_parser.getNextStepActionRequests();
        }
    }
};