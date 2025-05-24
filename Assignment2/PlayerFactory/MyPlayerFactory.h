

#pragma once
#include <memory>
#include "../common/Player.h"
#include "../common/PlayerFactory.h"
#include "../Players/PlayerOne.h" // Make sure this header defines the PlayerOne class

class MyPlayerFactory : public PlayerFactory
{
public:
    virtual ~MyPlayerFactory() {}

    virtual unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                      size_t max_steps, size_t num_shells) const override
    {
        return make_unique<PlayerOne>(player_index, x, y, max_steps, num_shells);
    }
};