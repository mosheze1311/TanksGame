#pragma once
#include "../common/ActionRequest.h"

#include <string>

namespace ActionRequestUtils{
    std::string actionToString(const ActionRequest tank_action)
    {
        switch (tank_action){
        case ActionRequest::MoveForward: return "MoveForward";
        case ActionRequest::MoveBackward: return "MoveBackward";
        case ActionRequest::RotateLeft45: return "RotateLeft45";
        case ActionRequest::RotateLeft90: return "RotateLeft90";
        case ActionRequest::RotateRight45: return "RotateRight45";
        case ActionRequest::RotateRight90: return "RotateRight90";
        case ActionRequest::Shoot: return "Shoot";
        case ActionRequest::GetBattleInfo: return "GetBattleInfo";
        case ActionRequest::DoNothing: return "DoNothing";
        //Should never happen
        default: return "UNKNOWN";
        } 
    }
}