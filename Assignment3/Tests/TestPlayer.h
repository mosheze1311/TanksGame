#pragma once

#include "../common/Player.h"
#include "../common/PlayerFactory.h"

#include "TestBattleInfo.h"

#include <fstream>
#include <vector>

class TestPlayer : public Player
{
private:
    // === Attributes === //
    std::vector<ActionRequest> action_requests;

    // === Handle With Player File === //
    std::string getPlayerFile() const
    {
        return "GetBattleInfoOutputs/get_battle_info_log.txt";
    }

    void deleteGetBattleInfoLog() const
    {
        std::string file_path = getPlayerFile();
        std::ifstream file(file_path);
        if (!file.is_open())
            return;

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();

        if (!lines.empty())
        {
            lines.pop_back(); // remove the last line
        }

        std::ofstream out_file(file_path, std::ios::trunc);
        for (const auto &l : lines)
        {
            out_file << l << '\n';
        }
        out_file.close();
    }

public:
    // === Constructor === //
    TestPlayer(int player_index, size_t x, size_t y,
               size_t max_steps, size_t num_shells) : Player(player_index, x, y, max_steps, num_shells) {};
    
    // === Destructor === //
    ~TestPlayer() override = default;

    // === Interface Function === //
    void updateTankWithBattleInfo(TankAlgorithm &t, SatelliteView &sat_view) override
    {
        deleteGetBattleInfoLog();

        // simulate flow
        (void)sat_view; // ignore pedantic errors
        TestBattleInfo battle_info;
        t.updateBattleInfo(battle_info);
    };
};

class TestPlayerFactory : public PlayerFactory
{
public:
    // === Constructor === //
    TestPlayerFactory() {};

    // === Destructor === //
    ~TestPlayerFactory() override = default;

    // === Interface Function === //
    std::unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                   size_t max_steps, size_t num_shells) const override
    {
        return std::make_unique<TestPlayer>(player_index, x, y, max_steps, num_shells);
    }
};
