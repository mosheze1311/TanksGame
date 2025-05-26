

#pragma once
#include <memory>
#include "../common/Player.h"
#include "../common/PlayerFactory.h"
#include "../Players/PlayerOne.h" // Make sure this header defines the PlayerOne class

// TODO: change to create multiple types of Players not just PlayerOne
class MyPlayerFactory : public PlayerFactory
{
public:
    virtual ~MyPlayerFactory() {}

    virtual std::unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                      size_t max_steps, size_t num_shells) const override
    {
        return std::make_unique<PlayerOne>(player_index, x, y, max_steps, num_shells);
    }
};