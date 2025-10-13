#include "Grid.h"
#include "PrettyColors.h"

Grid::Grid(Point2f startingPoint, int tilesPerRow, int tilesPerCol, Rectf screenRect,Point2f tileSize)
    :m_ScreenRect{screenRect}
    ,m_StartingPoint{startingPoint}
    
{
    Point2f tempOffset{5.0f,5.0f};
    SetTileOffset(tempOffset);

    InitializeGrid(tilesPerRow,tilesPerCol,tileSize);
   

}

Grid::~Grid()
{
    
}

void Grid::SetTileOffset(Point2f tileOffset)
{
    m_TilesOffset = tileOffset;
}

void Grid::InitializeGrid(int tilesPerRow,int tilesPerCol,Point2f tileSize)
{

    int totalTiles = tilesPerRow * tilesPerCol;

    float accumulatedHeightOffset{0.0f};
    float tempHeight{tileSize.y};
    int middleCol = (tilesPerCol / 2);
    
    // Calc position
    for (int indexRow{0}; indexRow < tilesPerRow; ++indexRow)
    {
        // 3D illusion
        float sizeOffset{3.0f};
        if (indexRow > 1)
        {
            accumulatedHeightOffset += sizeOffset * (indexRow-1);
        }
        tempHeight = tileSize.y - sizeOffset * indexRow;
        
        for (int indexCol{0}; indexCol < tilesPerCol; ++indexCol)
        {
            Tile temp;

            // Calculate all 4 points
            Point2f vertexBottomLeft{m_StartingPoint.x + (indexCol * (tileSize.x + m_TilesOffset.x))
                ,m_StartingPoint.y + (indexRow * (tileSize.y + m_TilesOffset.y)) - accumulatedHeightOffset};

            Point2f vertexBottomRight{(m_StartingPoint.x + (indexCol * (tileSize.x + m_TilesOffset.x)))+ tileSize.x
                ,m_StartingPoint.y + (indexRow * (tileSize.y + m_TilesOffset.y)) - accumulatedHeightOffset};


            Point2f tempTopRight{vertexBottomRight};
            tempTopRight.y += tempHeight;
            Point2f vertexTopRight{tempTopRight};

            Point2f tempTopLeft{vertexBottomLeft};
            tempTopLeft.y += tempHeight;
            Point2f vertexTopLeft{tempTopLeft};

            // Push them into vertices
            temp.vertices.push_back(vertexBottomLeft);
            temp.vertices.push_back(vertexBottomRight);
            temp.vertices.push_back(vertexTopRight);
            temp.vertices.push_back(vertexTopLeft);

            // Calc center of the pollygon
            Point2f tempCenter{vertexBottomLeft};
            tempCenter.x += tileSize.x / 2.0f;
            tempCenter.y += tempHeight / 2.0f;
            temp.tileCenter = tempCenter;
 
            m_Tiles.push_back(temp);
        }
    }
}

void Grid::DrawGrid()const
{
    using namespace utils;
    using namespace PrettyColors;

    Color4f gridColor{GetColor(black)};
    gridColor.a = 0.2f;
    SetColor(gridColor);

    for (auto& tile : m_Tiles)
    {
        FillPolygon(tile.vertices);
    }


}