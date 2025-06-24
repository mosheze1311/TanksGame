#pragma once

#include "DrawingTypes.h"

#include <string>
namespace UserCommon_211388913_322330820
{

    class DrawableObject
    {
    public:
        virtual ~DrawableObject() = default;
        virtual std::string getDrawing(DrawingType t) const = 0;
    };
}