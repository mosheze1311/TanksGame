#pragma once

#include "../../common/GameResult.h"
#include "../../common/SatelliteView.h"
#include "../Config/ConfigReader.h"
#include "SatelliteViewUtils.h"

namespace UserCommon_211388913_322330820
{
	namespace GameResultUtils
	{
		bool operator==(const GameResult &first_result, const GameResult &second_result)
		{

			if (first_result.winner != second_result.winner)
				return false;

			if (first_result.reason != second_result.reason)
				return false;

			if (first_result.remaining_tanks != second_result.remaining_tanks)
				return false;

			if (first_result.rounds != second_result.rounds)
				return false;

			if (first_result.gameState && second_result.gameState)
			{
				if (!SatelliteViewUtils::areEqual(*first_result.gameState, *second_result.gameState))
					return false;
			}
			else if (first_result.gameState || second_result.gameState)
			{
				return false;
			}

			return true;
		}

		inline std::string reasonToString(const GameResult &result)
		{
			std::ostringstream oss;
			switch (result.reason)
			{
			case GameResult::ALL_TANKS_DEAD:
				return "both players have zero tanks";

			case GameResult::MAX_STEPS: // need to know how many steps
				oss << "reached max steps = " << result.rounds;
				for (size_t i = 0; i < result.remaining_tanks.size(); ++i)
				{
					size_t player_id = i + 1;
					size_t player_tanks_count = result.remaining_tanks[i];
					oss << ", player " << std::to_string(player_id) << " has " << player_tanks_count << " tanks";
				}
				return oss.str();

			case GameResult::ZERO_SHELLS:
				return "both players have zero shells for " + std::to_string(ConfigReader::getConfig().getStepsAfterShellsEnd()) + " steps";

			default:
				return "unknown reason";
			}
		}

		std::string endOfGameLine(const GameResult &result)
		{
			std::ostringstream oss;

			if (result.winner == 0)
			{
				oss << "Tie, " << reasonToString(result);
			}

			else
			{
				oss << "Player " << result.winner << " won with " << result.remaining_tanks[result.winner - 1] << " tanks still alive";
			}

			return oss.str();
		}

		std::string toString(const GameResult &result)
		{
			std::ostringstream oss;
			oss << endOfGameLine(result) << std::endl;
			oss << result.rounds << std::endl;
			oss << SatelliteViewUtils::toString(*(result.gameState));
			return oss.str();
		}
	}
}
