#pragma once

#include "../common/Player.h"
#include "../common/PlayerFactory.h"

#include "../Players/Player1.h"
#include "../Players/Player2.h"

#include <memory>

class MyPlayerFactory : public PlayerFactory
{
public:
    // === Constructor === //
    MyPlayerFactory() = default;
    // === Destructor === //
    ~MyPlayerFactory() override = default;

    // === Interface Implementation === //
    std::unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                           size_t max_steps, size_t num_shells) const override
    {
        if (player_index % 2 == 0)
            return std::make_unique<Player1>(player_index, x, y, max_steps, num_shells);
        return std::make_unique<Player2>(player_index, x, y, max_steps, num_shells);
    }
};