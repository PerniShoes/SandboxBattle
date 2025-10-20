#pragma once
#include "utils.h"
#include <functional>

struct MapLayer;
class MapManager; 

namespace MapHelpers
{

    using ScaleFunc = std::function<Point2f(const MapManager*,const MapLayer&)>;
    using PositionFunc = std::function<Point2f(const MapManager*,const MapLayer&)>;

    // Helper wrappers
    inline ScaleFunc WrapScale(Point2f(MapManager::* func)(const MapLayer&) const)
    {
        return [func](const MapManager* mgr,const MapLayer& layer)
            {
                return (mgr->*func)(layer);
            };
    }
    inline PositionFunc WrapPosition(Point2f(MapManager::* func)(const MapLayer&) const)
    {
        return [func](const MapManager* mgr,const MapLayer& layer)
            {
                return (mgr->*func)(layer);
            };
    }
    inline ScaleFunc FixedScale(Point2f scale)
    {
        return [scale](const MapManager*,const MapLayer&)
            {
                return scale;
            };
    }

    extern ScaleFunc FS;   // FillScreen
    extern ScaleFunc WS;   // FillWidth
    extern ScaleFunc HS;   // FillHeight

    extern PositionFunc BL; // BottomLeft
    extern PositionFunc BR; // BottomRight
    extern PositionFunc TL; // TopLeft
    extern PositionFunc TR; // TopRight
    extern PositionFunc C;  // Center

    ScaleFunc FSX(float factorX = 1.f,float factorY = 1.f); // FixedScale helper
}
