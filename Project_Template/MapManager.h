#pragma once
#include <array>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <filesystem>

#include "Texture.h"
#include <functional>
#include "utils.h"
#include "Transform.h"
#include "MapHelpers.h"

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

// FIX, it's public, not good
struct LayerOverride;

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
    void RegisterLayerOverrides();

    void SetMap(const std::string& mapName);
    std::string GetCurrentMap() const noexcept { return m_CurrentMapName; };

    // Position helpers
    Rectf GetLayerSrcRect(const MapLayer& layer) const;
    Point2f CalcFillWidthScale(const MapLayer& layer) const;
    Point2f CalcFillHeightScale(const MapLayer& layer) const;
    Point2f CalcTopLeft(const MapLayer& layer) const;
    Point2f CalcTopRight(const MapLayer& layer) const;
    Point2f CalcBottomLeft(const MapLayer& layer) const;
    Point2f CalcBottomRight(const MapLayer& layer) const;
    Point2f CalcCenter(const MapLayer& layer) const;
    Point2f CalcFillScreenScale(const MapLayer& layer) const;

private:

    void SetAllLayerPositions();
    
    std::unordered_map<std::string,MapData> m_MapSets;
    Rectf m_ScreenRect;
    std::string m_CurrentMapName;

    std::unordered_map<std::string,std::vector<LayerOverride>> m_LayerOverrides;

    bool m_StateChanged;

    // Fallback scale
    Point2f m_GlobalTempScale{1.0f, 1.0f};
};

struct LayerOverride
{
    std::string nameContains;
    MapHelpers::ScaleFunc scaleFunc;
    MapHelpers::PositionFunc posFunc;
    Point2f extraScale{1.f,1.f};
};

