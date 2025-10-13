#pragma once
#include <vector>
#include "utils.h"

struct Tile
{
    std::vector<Point2f> vertices;
    Point2f tileCenter;

};

class Grid final
{
public:
    Grid(Point2f startingPoint, int tilesPerRow, int tilesPerCol, Rectf screenRect, Point2f tileSize);
    Grid(const Grid& other) = delete;
    Grid& operator=(const Grid& other) = delete;
    Grid(Grid&& other) = delete;
    Grid& operator=(Grid&& other) = delete;
    ~Grid();

    void InitializeGrid(int tilesPerRow,int tilesPerCol,Point2f tileSize);
    void DrawGrid()const;
    
    // Point2f GetTileCenter(int tileIndex);
    void SetTileOffset(Point2f tileOffset);

private:

    const Rectf m_ScreenRect;
    std::vector<Tile> m_Tiles;
    Point2f m_TilesOffset;
    Point2f m_StartingPoint;

};