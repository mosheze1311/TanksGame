#pragma once
namespace UserCommon_211388913_322330820
{

    enum class DrawingType
    {
        REGULAR,     //
        TENNIS,      // t1🏸 t2🏓 wall🥅🥅 mine🔥 shell🎾
        SCIFI,       // t1👽 t2👾 wall📡🛰️ mine💥 shell🔮
        PIRATE,      // t1⛴️ t2🚢 wall🌊🪸 mine☠️ shell💣
        MIDDLE_EAST, // t1🇮🇱 t2🇮🇷 wall🏰🛡️ mine🧨 shell🚀
        NONE
    };

    namespace DrawingTypes
    {
        constexpr inline DrawingType fromInt(int drawingType)
        {
            switch (drawingType)
            {
            case 0:
                return DrawingType::REGULAR;
            case 1:
                return DrawingType::TENNIS;
            case 2:
                return DrawingType::SCIFI;
            case 3:
                return DrawingType::PIRATE;
            case 4:
                return DrawingType::MIDDLE_EAST;
            default:
                return DrawingType::NONE;
            }
        }
    }
}