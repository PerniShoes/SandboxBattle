#pragma once
#include <vector>
#include "utils.h"

struct Tile
{
    // Order of verticies in the vectors
    // BottomLeft, BR, TR, TL
    // FIX 
    // Should use storage with names, instead of having to remember the order, 
    // also makes it unsafe if the amount of vertices changes to less than 4

    std::vector<Point2f> vertices;
    Point2f tileCenter;
    std::vector<Point2f> spacingOnLeftVertices;
    std::vector<Point2f> spacingBellowVertices;
    bool fillLeft{false};
    bool fillRight{false};
    bool highlightTile{false};
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
    void DrawHighlights()const;
    Point2f GetTileCenter(int tileIndex);
    int GetHoverTileId(Point2f mousePos);
    int GetTileAmount() const;
    bool IsValidTileId(int tileIndex) const;
    void HighlightReachableTiles(std::vector<int> reachableTilesIds);
    void UnHighlightTiles();

    void SetTileOffset(Point2f tileOffset);

private:

    const Rectf m_ScreenRect;
    std::vector<Tile> m_Tiles;
    Point2f m_TilesOffset;
    Point2f m_StartingPoint;

};