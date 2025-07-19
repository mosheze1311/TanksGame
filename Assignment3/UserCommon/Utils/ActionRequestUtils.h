#pragma once
#include "../../common/ActionRequest.h"

#include <string>
#include <set>
namespace UserCommon_211388913_322330820
{
    namespace ActionRequestUtils
    {
        inline std::string actionToString(const ActionRequest tank_action)
        {
            switch (tank_action)
            {
            case ActionRequest::MoveForward:
                return "MoveForward";
            case ActionRequest::MoveBackward:
                return "MoveBackward";
            case ActionRequest::RotateLeft45:
                return "RotateLeft45";
            case ActionRequest::RotateLeft90:
                return "RotateLeft90";
            case ActionRequest::RotateRight45:
                return "RotateRight45";
            case ActionRequest::RotateRight90:
                return "RotateRight90";
            case ActionRequest::Shoot:
                return "Shoot";
            case ActionRequest::GetBattleInfo:
                return "GetBattleInfo";
            case ActionRequest::DoNothing:
                return "DoNothing";
            // Should never happen
            default:
                return "UNKNOWN";
            }
        }

        inline std::set<ActionRequest> getAllRequests()
        {
            return {
                ActionRequest::MoveForward,
                ActionRequest::MoveBackward,
                ActionRequest::RotateLeft45,
                ActionRequest::RotateLeft90,
                ActionRequest::RotateRight45,
                ActionRequest::RotateRight90,
                ActionRequest::Shoot,
                ActionRequest::GetBattleInfo,
                ActionRequest::DoNothing};
        }

    }

}