# pragma once
#include <string>
enum class TankAction
{
    NOTHING,
    FORWARD,
    BACKWARD,
    TURN45LEFT,
    TURN45RIGHT,
    TURN90LEFT,
    TURN90RIGHT,
    FIRE
};

namespace TankActionUtil{
    inline std::string getName(TankAction action)
    {
        switch (action) {
            case TankAction::NOTHING: return "NOTHING";
            case TankAction::FORWARD: return "FORWARD";
            case TankAction::BACKWARD: return "BACKWARD";
            case TankAction::TURN45LEFT: return "TURN45LEFT";
            case TankAction::TURN45RIGHT: return "TURN45RIGHT";
            case TankAction::TURN90LEFT: return "TURN90LEFT";
            case TankAction::TURN90RIGHT: return "TURN90RIGHT";
            case TankAction::FIRE: return "FIRE";
            default: return "UNKNOWN";
        }
    }
}
