#pragma once
#include "Texture.h"
#include <array>
#include <string>
#include "utils.h"
#include <memory>

enum class MouseState
{
    Default = 0,
    Selected,
    HoverEnemy,
    Move,
    HoverAlly,
    
    TotalStates
};

class MouseManager
{
public:

    MouseManager();
    ~MouseManager();

    void Draw() const;
    void Update(float elapsedTime);

    void LoadMouseTextures();
    void SetMouseState(MouseState state);
    void OnMouseMotion(const Point2f& mousePos);

private:
    std::array<std::unique_ptr<Texture>,int(MouseState::TotalStates)> m_Textures;
    std::array<std::string,int(MouseState::TotalStates)> m_Paths;
    MouseState m_CurrentState;
    bool m_StateChanged;
    Point2f m_LastMousePos;

};
