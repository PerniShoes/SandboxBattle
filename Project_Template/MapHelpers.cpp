#include "MapHelpers.h"
#include "MapManager.h"

namespace MapHelpers
{

    auto FS = WrapScale(&MapManager::CalcFillScreenScale);
    auto WS = WrapScale(&MapManager::CalcFillWidthScale);
    auto HS = WrapScale(&MapManager::CalcFillHeightScale);

    auto BL = WrapPosition(&MapManager::CalcBottomLeft);
    auto BR = WrapPosition(&MapManager::CalcBottomRight);
    auto TL = WrapPosition(&MapManager::CalcTopLeft);
    auto TR = WrapPosition(&MapManager::CalcTopRight);
    auto C = WrapPosition(&MapManager::CalcCenter);

    ScaleFunc MapHelpers::FSX(float factorX,float factorY)
    {
        return FixedScale(Point2f{factorX, factorY});
    }

}
