#include "MouseManager.h"

#include <print>

MouseManager::MouseManager()
    :m_CurrentState{MouseState::Default}
    ,m_StateChanged{false}
    ,m_LastMousePos{}
{
    SDL_ShowCursor(SDL_DISABLE);
    m_Paths = 
    {
        "../Resources/DuelystResc/ui/mouse_auto.png",
        "../Resources/DuelystResc/ui/mouse_select.png",
        "../Resources/DuelystResc/ui/mouse_attack.png",
        "../Resources/DuelystResc/ui/mouse_move.png",
        "../Resources/DuelystResc/ui/mouse_assist.png"
    };
    LoadMouseTextures();
   
}

MouseManager::~MouseManager()
{
    // Clean textures

}

void MouseManager::LoadMouseTextures()
{
    // Remember to load paths in the same order as they are in MouseState
    if (m_Paths.size() > int(MouseState::TotalStates))
    {
        std::println("Too many paths passed in MouseManager::LoadMouseTextures, amount: {}",m_Paths.size());
    }
    for (int i{0}; i < m_Paths.size(); ++i)
    {
        m_Textures[i] = std::make_unique<Texture>(m_Paths[i]);
    }
}

void MouseManager::Draw() const
{
    Point2f drawingPosition{m_LastMousePos};
    drawingPosition.y = m_LastMousePos.y - m_Textures[int(m_CurrentState)]->GetHeight();
    m_Textures[int(m_CurrentState)]->Draw(drawingPosition);
}

void MouseManager::SetMouseState(MouseState state)
{
    if (state != m_CurrentState)
    {
        m_CurrentState = state;
        m_StateChanged = true;
    }
}

void MouseManager::Update(float elapsedTime)
{
    if (m_StateChanged)
    {

        // Change current drawing Texture
        m_StateChanged = false;
    }
}
void MouseManager::OnMouseMotion(const Point2f& mousePos)
{
    m_LastMousePos = mousePos;
}