#pragma once

#include "../Config/ConfigReader.h"

#include <algorithm>

class CooldownHandler
{

private:
    // === Attributes === //
    int backward_cooldown = -2;
    int shoot_cooldown = 0;

    // === Private Functions === //
    void tickBackwardsWait()
    {
        backward_cooldown = std::max(backward_cooldown - 1, -2);
    }
    void tickShootCooldown()
    {
        shoot_cooldown = std::max(shoot_cooldown - 1, 0);
    }
public:
    // === Constructor === //
    CooldownHandler(){};

    // === Backward cooldown === //
    void startBackwardWait()
    {
        backward_cooldown = ConfigReader::getConfig().getBackwardWaitSteps();
    }

    void cancelBackwardWait()
    {
        backward_cooldown = -2;
    }

    bool isPendingBackwards() const
    {
        return backward_cooldown >= 0;
    }

    bool canImmediateBackwards() const
    {
        return backward_cooldown == -1;
    }

    void extendBackwardsStreak()
    {
        backward_cooldown = 0;
    }

    bool isBackwardsDue() const
    {
        return backward_cooldown == 0;
    }

    // === Shooting cooldown === //
    bool isDoneShootCooldown() const
    {
        return shoot_cooldown == 0;
    }

    void beginShoot()
    {
       shoot_cooldown = ConfigReader::getConfig().getShootCooldownSteps() + 1; // +1 because changing in this step and it does not count as cooldown
    }

    void tickCooldowns(){
        tickBackwardsWait();
        tickShootCooldown();
    };
};