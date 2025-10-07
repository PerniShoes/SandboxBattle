#pragma once
#include "Texture.h"
#include <array>
#include <string>
#include "utils.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include "Transform.h"

enum class LayerType
{
    Background,
    Midground,
    Foreground,
    Unknown
};

struct MapLayer
{
    std::unique_ptr<Texture> texture;
    std::string path;                 // Full path (unique id)
    std::string name;                 // File name
    Rectf dstRect;                    // (0,0,w,h) normally
    Transform transform;              
    LayerType type{LayerType::Unknown};
    Point2f originalSize{0.f,0.f};    // Width and height

    // Convenience ctor for constructing layer metadata before texture is loaded
    MapLayer(std::string path_,std::string name_,Rectf dst = Rectf{0,0,0,0},LayerType t = LayerType::Unknown)
        : texture{nullptr},path{std::move(path_)},name{std::move(name_)},dstRect{dst},transform{},type{t}
    {
    }
    MapLayer(MapLayer&&) noexcept = default;
    MapLayer& operator=(MapLayer&&) noexcept = default;
    MapLayer(const MapLayer&) = delete;
    MapLayer& operator=(const MapLayer&) = delete;
};

struct MapData
{
    std::string name;
    std::vector<MapLayer> layers;
};

// Might scratch this idea for string based keys
enum class MapList
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

    void DrawLayerType(LayerType type) const; 
    void DrawAllLayers() const;
    void Update(float elapsedTime);

    // FIX private
    void LoadMapTextures();        
    void LoadMapsFromFolder(const std::string& folderPath); 

    void SetMap(const std::string& mapName);
    std::string GetCurrentMap() const noexcept { return m_CurrentMapName; };

private:

    void SetAllLayerPositions();
    // Position helpers
    Rectf GetLayerSrcRect(const MapLayer& layer) const;
    Rectf CalcTopLeft(const Point2f& scale,const Point2f& size) const;
    Rectf CalcTopRight(const Point2f& scale,const Point2f& size) const;
    Rectf CalcBottomLeft(const Point2f& scale,const Point2f& size) const;
    Rectf CalcBottomRight(const Point2f& scale,const Point2f& size) const;
    Rectf CalcCenter(const Point2f& scale,const Point2f& size) const;
    Point2f CalcFillScreenScale(const MapLayer& layer) const;

    std::unordered_map<std::string,MapData> m_MapSets;
    Rectf m_ScreenRect;
    std::string m_CurrentMapName;

    bool m_StateChanged;

    // Fallback scale
    Point2f m_GlobalTempScale{1.0f, 1.0f};
};
