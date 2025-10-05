#pragma once
#include "Texture.h"
#include <array>
#include <string>
#include "utils.h"
#include <memory>
#include <unordered_map>

struct MapLayer
{
    std::unique_ptr<Texture> texture;
    std::string path;
    Rectf dstRect; 
};

struct MapData
{
    std::string name;
    std::vector<MapLayer> layers;
};


enum class MapTexture
{
    Default = 0,
    Selected,
    HoverEnemy,
    Move,
    HoverAlly,

    TotalMaps
};

class MapManager final
{
public:

    // FIX rule of 5/6
    MapManager(Rectf screenRect);
    ~MapManager();

    void Draw() const;
    void Update(float elapsedTime);

    void LoadMapTextures();
    void SetMapTexture(MapTexture mapTexture);

private:

    // FIX Generated with chatGPT, didn't have time to test before leaving, test them, maybe adjust too
    // (implementation too, it all seems somewhat okay, but look into it)
    Rectf CalcTopLeft(const Rectf& srcRect) const;
    Rectf CalcTopRight(const Rectf& srcRect) const;
    Rectf CalcBottomLeft(const Rectf& srcRect) const;
    Rectf CalcBottomRight(const Rectf& srcRect) const;
    Rectf CalcCenter(const Rectf& srcRect) const;





    std::unordered_map<MapTexture,MapData> m_MapSets;

    // TEMP scale to test FIX
    Point2f m_TempScale{1.0f,1.0f};
    Rectf m_ScreenRect;
    MapTexture m_CurrentState;
    bool m_StateChanged;
};
