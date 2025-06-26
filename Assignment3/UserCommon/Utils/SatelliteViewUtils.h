#pragma once

#include "../../common/SatelliteView.h"
#include "../BoardSatelliteView/SpecialSatelliteViewChars.h"

namespace UserCommon_211388913_322330820
{
    namespace SatelliteViewUtils
    {
        bool areEqual(const SatelliteView &a, const SatelliteView &b)
        {
            size_t width = 0;
            while (true)
            {
                size_t height = 0;
                while (true)
                {
                    char a_char = a.getObjectAt(width, height);
                    char b_char = b.getObjectAt(width, height);

                    if (a_char == static_cast<char>(SpecialSatelliteViewChars::OutOfBounds) &&
                        b_char == static_cast<char>(SpecialSatelliteViewChars::OutOfBounds))
                    {
                        break;
                    }

                    if (a_char != b_char)
                        return false;

                    height++;
                }

                char next_a_char = a.getObjectAt(width + 1, 0);
                char next_b_char = b.getObjectAt(width + 1, 0);
                if (next_a_char == static_cast<char>(SpecialSatelliteViewChars::OutOfBounds) &&
                    next_b_char == static_cast<char>(SpecialSatelliteViewChars::OutOfBounds))
                {
                    break;
                }

                width++;
            }

            return true;
        }

        std::string toString(const SatelliteView &sat_view)
        {
            std::ostringstream oss;
            for (size_t y = 0;; ++y)
            {
                bool row_empty = true;
                for (size_t x = 0;; ++x)
                {
                    char c = sat_view.getObjectAt(x, y);
                    if (c == static_cast<char>(SpecialSatelliteViewChars::OutOfBounds))
                        break;

                    oss << c;
                    row_empty = false;
                }
                if (row_empty)
                    break;
                
                oss << '\n';
            }
            return oss.str();
        }
    }
}