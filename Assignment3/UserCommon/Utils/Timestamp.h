#pragma once

#include <chrono>
#include <cmath>
#include <iomanip>
#include <string>

namespace UserCommon_211388913_322330820
{
    std::string getTimestampFor(std::chrono::system_clock::time_point time_point)
    {
        std::time_t time_point_c = std::chrono::system_clock::to_time_t(time_point);
        std::chrono::duration<double> ts = time_point.time_since_epoch();

        //  Format timestamp to date/time
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_point_c), "%Y%m%d_%H%M%S");

        // Append high-precision fraction
        constexpr size_t NUM_DIGITS = 9;
        size_t NUM_DIGITS_P = std::pow(10, NUM_DIGITS);
        size_t fractional_part = size_t(ts.count() * NUM_DIGITS_P) % NUM_DIGITS_P;

        oss << "_" << std::setw(NUM_DIGITS) << std::setfill('0') << fractional_part;
        return oss.str();
    }

    std::string getTimestampForNow()
    {
        return getTimestampFor(std::chrono::system_clock::now());
    }
}