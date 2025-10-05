#include "MapManager.h"
#include <print>

MapManager::MapManager(Rectf screenRect)
    :m_ScreenRect{screenRect}
    ,m_CurrentState{MapTexture::Default}
    ,m_StateChanged{false}
{


    // Remember to load paths in the same order as they are in MapTexture enum

        //"../Resources/DuelystResc/maps/battlemap1_middleground.png",
        //"../Resources/DuelystResc/maps/mouse_select.png",
        //"../Resources/DuelystResc/maps/mouse_attack.png",
        //"../Resources/DuelystResc/maps/mouse_move.png",
        //"../Resources/DuelystResc/maps/mouse_assist.png"

    LoadMapTextures();

}

MapManager::~MapManager()
{
    // Clean textures

}

void MapManager::LoadMapTextures()
{
    for (auto& [typeEnum,mapData] : m_MapSets)
    {
        for (auto& layer : mapData.layers)
        {
            layer.texture = std::make_unique<Texture>(layer.path);
        }
    }
}

void MapManager::Draw() const
{
    const auto& map = m_MapSets.at(m_CurrentState);
    for (const auto& layer : map.layers)
    {
        if (layer.texture)
            layer.texture->Draw(layer.dstRect);
    }
}

void MapManager::SetMapTexture(MapTexture mapTexture)
{
    if (mapTexture != m_CurrentState)
    {
        m_CurrentState = mapTexture;
        // Useless for now (stateChanged)
        m_StateChanged = true;
    }
}

void MapManager::Update(float elapsedTime)
{
    if (m_StateChanged)
    {
        // Useless for now (stateChanged)
        m_StateChanged = false;
    }
}

// Group these somehow better maybe
// Generated with chatGPT (read FI X in .h file)
Rectf MapManager::CalcTopLeft(const Rectf& srcRect) const
{
    float w = srcRect.width * m_TempScale.x;
    float h = srcRect.height * m_TempScale.y;
    return Rectf{0.f, m_ScreenRect.height - h, w, h};
}
Rectf MapManager::CalcTopRight(const Rectf& srcRect) const
{
    float w = srcRect.width * m_TempScale.x;
    float h = srcRect.height * m_TempScale.y;
    return Rectf{m_ScreenRect.width - w, m_ScreenRect.height - h, w, h};
}
Rectf MapManager::CalcBottomLeft(const Rectf& srcRect) const
{
    float w = srcRect.width * m_TempScale.x;
    float h = srcRect.height * m_TempScale.y;
    return Rectf{0.f, 0.f, w, h};
}
Rectf MapManager::CalcBottomRight(const Rectf& srcRect) const
{
    float w = srcRect.width * m_TempScale.x;
    float h = srcRect.height * m_TempScale.y;
    return Rectf{m_ScreenRect.width - w, 0.f, w, h};
}
Rectf MapManager::CalcCenter(const Rectf& srcRect) const
{
    float w = srcRect.width * m_TempScale.x;
    float h = srcRect.height * m_TempScale.y;
    return Rectf{(m_ScreenRect.width - w) / 2.f, (m_ScreenRect.height - h) / 2.f, w, h};
}
