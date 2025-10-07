#include "MapManager.h"
#include <print> 
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

static LayerType InferLayerTypeFromName(const std::string& name)
{
    std::string lower = name;
    std::transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char c) { return std::tolower(c); });

    if (lower.find("background") != std::string::npos) 
        return LayerType::Background;

    if (lower.find("middleground") != std::string::npos || lower.find("mid") != std::string::npos) 
        return LayerType::Midground;

    if (lower.find("foreground") != std::string::npos || lower.find("fg") != std::string::npos) 
        return LayerType::Foreground;

    
    return LayerType::Unknown;
}

MapManager::MapManager(Rectf screenRect)
    : m_ScreenRect{screenRect}
    ,m_CurrentMapName{"None (default)"}
    ,m_StateChanged{false}
{


    
    // Autoload all maps from specific folder
    LoadMapsFromFolder("../Resources/DuelystResc/maps/");

    // Load all textures 
    LoadMapTextures();

    SetAllLayerPositions();

}

MapManager::~MapManager()
{
    // Prolly not needed since unique is used
}

void MapManager::LoadMapsFromFolder(const std::string& folderPath)
{
    // Search the folder and subfolders (one level deep). For each image file, create MapData entry or add to existing
    // Skip files with @2x before extension
    // Files named like "battlemap0_background.png" -> map name "battlemap0"

    if (!fs::exists(folderPath))
    {
        std::println("Map folder not found: {}",folderPath);
        return;
    }

    for (auto const& entry : fs::directory_iterator(folderPath))
    {
        // -------- FOLDER --------
        if (entry.is_directory())
        {
            const std::string folderName = entry.path().filename().string();
            MapData data;
            data.name = folderName;

            for (auto const& f : fs::directory_iterator(entry.path()))
            {
                if (!f.is_regular_file()) continue;
                std::string filename = f.path().filename().string();
                if (filename.find("@2x") != std::string::npos) continue; // skip 2x

                std::string ext = f.path().extension().string();
                std::transform(ext.begin(),ext.end(),ext.begin(),::tolower);
                if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") continue;

                LayerType lt = InferLayerTypeFromName(filename);
                MapLayer layer(f.path().string(),filename,Rectf{0, 0, 0, 0},lt);
                data.layers.emplace_back(std::move(layer));
            }

            if (!data.layers.empty())
            {
                // Store the folder as its own map
                m_MapSets[folderName] = std::move(data);
                std::println("Loaded map folder: {}",folderName);
            }
        }

        // -------- FILE --------
        else if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            if (filename.find("@2x") != std::string::npos) continue;

            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(),ext.end(),ext.begin(),::tolower);
            if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") continue;

            // Extract base map name (prefix before first '_')
            std::string base = filename.substr(0,filename.find_first_of('_'));
            if (base.empty()) base = entry.path().stem().string();

            LayerType lt = InferLayerTypeFromName(filename);

            // Find or create map entry
            MapData& mapData = m_MapSets[base];
            mapData.name = base;

            MapLayer layer(entry.path().string(),filename,Rectf{0, 0, 0, 0},lt);
            mapData.layers.emplace_back(std::move(layer));
        }
    }

    std::println("Loaded {} maps from {}",m_MapSets.size(),folderPath);
}

void MapManager::LoadMapTextures()
{
    for (auto& [mapName,mapData] : m_MapSets)
    {
        for (auto& layer : mapData.layers)
        {
            try
            {
                layer.texture = std::make_unique<Texture>(layer.path);
                // Read size
                // FIX, not needed cast, or maybe it's future proof? Hmmm
                layer.originalSize.x = static_cast<float>(layer.texture->GetWidth());
                layer.originalSize.y = static_cast<float>(layer.texture->GetHeight());
                // set dstRect w/h to original size (x/y at 0,0)
                layer.dstRect = Rectf{0.f, 0.f, layer.originalSize.x, layer.originalSize.y};

                // default transform scale to 1 if not already set
                if (layer.transform.scale.x == 0.f) layer.transform.scale.x = 1.f;
                if (layer.transform.scale.y == 0.f) layer.transform.scale.y = 1.f;

            }
            catch (const std::exception& e)
            {
                std::println("Failed to load texture {} : {}",layer.path,e.what());
                layer.texture.reset();
            }
        }
    }
}

// FIX FIX FIX, this has to be adjusted, first of, too many if's, second off, it doesn't use transform correctly to translate
// Besides first if, the rest seemd pretty okay (before testing)
Rectf MapManager::GetLayerSrcRect(const MapLayer& layer) const
{
    float w = layer.originalSize.x;
    float h = layer.originalSize.y;

    return Rectf{0.0f,0.0f, w, h};
}

Rectf MapManager::CalcTopLeft(const Point2f& scale,const Point2f& size) const
{
    float w = size.x * scale.x;
    float h = size.y * scale.y;
    return Rectf{0.f, m_ScreenRect.height - h, w, h};
}
Rectf MapManager::CalcTopRight(const Point2f& scale,const Point2f& size) const
{
    float w = size.x * scale.x;
    float h = size.y * scale.y;
    return Rectf{m_ScreenRect.width - w, m_ScreenRect.height - h, w, h};
}
Rectf MapManager::CalcBottomLeft(const Point2f& scale,const Point2f& size) const
{
    float w = size.x * scale.x;
    float h = size.y * scale.y;
    return Rectf{0.f, 0.f, w, h};
}
Rectf MapManager::CalcBottomRight(const Point2f& scale,const Point2f& size) const
{
    float w = size.x * scale.x;
    float h = size.y * scale.y;
    return Rectf{m_ScreenRect.width - w, 0.f, w, h};
}
Rectf MapManager::CalcCenter(const Point2f& scale,const Point2f& size) const
{
    float w = size.x * scale.x;
    float h = size.y * scale.y;
    return Rectf{(m_ScreenRect.width - w) / 2.f, (m_ScreenRect.height - h) / 2.f, w, h};
}
Point2f MapManager::CalcFillScreenScale(const MapLayer& layer) const
{
    float scaleX = m_ScreenRect.width / layer.originalSize.x;
    float scaleY = m_ScreenRect.height / layer.originalSize.y;
    return Point2f{scaleX, scaleY};
}


// This seems okay? But transform usage is overall flawed above
void MapManager::DrawLayerType(LayerType type) const
{
    auto it = m_MapSets.find(m_CurrentMapName);
    if (it == m_MapSets.end())
    {
        std::println("Map name not found, current name set: {}",m_CurrentMapName);
        return;
    }
    const MapData& map = it->second;

    for (const auto& layer : map.layers)
    {
        if (layer.type != type) continue;
        if (!layer.texture) continue;

        Rectf src = GetLayerSrcRect(layer);
        
        layer.transform.SetExactScale(1.0f,1.0f);
        layer.transform.Push();  
        layer.transform.Apply();

        layer.texture->Draw(m_ScreenRect);
        
        layer.transform.Pop();
    }
}

// Pure debug, prolly not needed
void MapManager::DrawAllLayers() const
{
    DrawLayerType(LayerType::Background);
    DrawLayerType(LayerType::Midground);

    DrawLayerType(LayerType::Foreground);
}

void MapManager::SetMap(const std::string& mapName)
{
    if (mapName != m_CurrentMapName)
    {
        m_CurrentMapName = mapName;
        m_StateChanged = true;
    }
}

void MapManager::Update(float elapsedTime)
{
    if (m_StateChanged)
    {
        m_StateChanged = false;
    }
}

void MapManager::SetAllLayerPositions()
{
    for (auto& [mapName,map] : m_MapSets)
    {
        for (auto& layer : map.layers)
        {
            switch (layer.type)
            {
            case LayerType::Background:
                layer.transform.position = Point2f{0.f, 25.f}; 
                layer.transform.scale = CalcFillScreenScale(layer);
                break;

            case LayerType::Midground:
                layer.transform.scale = CalcFillScreenScale(layer);
                layer.transform.position = Point2f{0.f, 25.f};
                break;

            case LayerType::Foreground:
                layer.transform.position = Point2f{0.f, 0.f};
                layer.transform.scale = Point2f{1.f, 1.f};
                break;

            default:
                break;
            }
        }
    }
}
