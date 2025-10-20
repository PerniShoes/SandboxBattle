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
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!      CAUTION     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! WHEN REFACTORING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // This **WILL** crash if changed carelessly 
    // 
    // 1. The code bellow works based on the assumption that tiles are drawn:
    // left -> right
    // bottom -> top
    // So, first (very bottom) row is drawn first, then the second one, etc.
    
    // 2. Vertex order matters (changing the order, won't crash but will result in very weird drawings)

    // 3. It also assumes each tile (and space between them) has *at least* 4 vertices, will crash if less, won't work if more xD
    // Will FIX the issue above ^
  
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

            // ORDER MATTERS
            // BL, BR, TR, TL
            temp.vertices.push_back(vertexBottomLeft);
            temp.vertices.push_back(vertexBottomRight);
            temp.vertices.push_back(vertexTopRight);
            temp.vertices.push_back(vertexTopLeft);

            // Calc center of the pollygon
            Point2f tempCenter{vertexBottomLeft};
            tempCenter.x += tileSize.x / 2.0f;
            tempCenter.y += tempHeight / 2.0f;
            temp.tileCenter = tempCenter;

            // Setup spacing on the left and bellow
            if (indexCol > 0)
            {
                Tile& leftTile = m_Tiles[m_Tiles.size() - 1];
                // ORDER MATTERS
                // BL, BR, TR, TL

                // Bottom left vertex = bottom right vertex of the tile on the left
                temp.spacingOnLeftVertices.push_back(leftTile.vertices[1]);
                // BR
                temp.spacingOnLeftVertices.push_back(temp.vertices[0]);
                // TR
                temp.spacingOnLeftVertices.push_back(temp.vertices[3]);
                // TL
                temp.spacingOnLeftVertices.push_back(leftTile.vertices[2]);
            }

            if (indexRow > 0)
            {
                Tile& bottomTile = m_Tiles[(indexRow - 1) * tilesPerCol + indexCol];
                // ORDER MATTERS
                // BL, BR, TR, TL

                // BL
                temp.spacingBellowVertices.push_back(bottomTile.vertices[3]);
                // BR
                temp.spacingBellowVertices.push_back(bottomTile.vertices[2]);
                // TR
                temp.spacingBellowVertices.push_back(temp.vertices[1]);
                // TL
                temp.spacingBellowVertices.push_back(temp.vertices[0]);
            }

            m_Tiles.push_back(temp);
        }
    }
}

void Grid::DrawGrid()const
{
    using namespace utils;
    using namespace PrettyColors;

    Color4f gridColor{GetColor(black)};
    gridColor.a = 0.2f; // 0.2f orignal
    SetColor(gridColor);

    for (auto& tile : m_Tiles)
    {
        SetColor(gridColor);
        FillPolygon(tile.vertices);

        if (tile.spacingOnLeftVertices.size() != 0) 
        {
            SetColor(GetColor(green));
            FillPolygon(tile.spacingOnLeftVertices);
        }

        if (tile.spacingBellowVertices.size() != 0)
        {
            SetColor(GetColor(red));
            FillPolygon(tile.spacingBellowVertices);
        }
    }

    // If there are any
    DrawHighlighted();

}
void Grid::DrawHighlighted() const
{
    // Check if there are any

}

int Grid::GetHoverTileId(Point2f mousePos)
{
    for (int i{0};i<int(m_Tiles.size());++i)
    {
        if (utils::IsPointInPolygon(mousePos,m_Tiles[i].vertices))
        {
            return i;
        }
    }
    return -1;
}
Point2f Grid::GetTileCenter(int tileIndex)
{
    if (m_Tiles.size() < tileIndex)
    {
        return Point2f{-1.0f,-1.0f};
    }
    return m_Tiles[tileIndex].tileCenter;
}
int Grid::GetTileAmount() const
{
    return int(m_Tiles.size());
}

void Grid::HighlightReachableTiles(std::vector<int> reachableTilesIds)
{
    using namespace utils;
    using namespace PrettyColors;

    // Call once per unit select

    int neighbourId{0};
    auto itterator = std::find(reachableTilesIds.begin(),reachableTilesIds.end(),neighbourId);

    // ORDER MATTERS
    // BL, BR, TR, TL


    // Get ids of all reachable tiles (with your calc method of going max range one dir, then 0 up, 
    // max range -1 one dir, then 1 up and so on, checking if an obstacle is in a tile


    // One by one, calc Rects (or polygons rather), check if it has a neighbor on left or bellow
    // On a side there is a neighbour -> "fill" the spacing
     
    // If there is no neighbour on a side -> draw edge (later will have to extend edges to connect, 
    // probably just drawing up/down half the missing distance for each)


    // Polish
    // Check if a tile has neighbours on left + down + downLeft diagonal. If so, fill the gap between those four 
    // This might cause issues with overlap and alpha, if so, you can adjust spacingVertices to fill the gaps instead
    // either here or have some "one time call" func that sets this up after creating them


    // Store all calcutaions

    
}
void Grid::UnHighlightTiles()
{

}

bool Grid::IsValidTileId(int tileIndex) const
{
    return tileIndex < int(m_Tiles.size()) && tileIndex >= 0;
}