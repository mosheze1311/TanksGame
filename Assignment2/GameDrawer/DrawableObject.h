#pragma once

#include "DrawingTypes.h"

#include <string>

class DrawableObject
{
public:
    virtual ~DrawableObject() = default;
    virtual std::string getDrawing(DrawingType t) const = 0;
};