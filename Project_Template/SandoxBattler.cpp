#include <cmath>
#include <iostream>
#include <print>

#include "Time.h"
#include "Texture.h"
#include "OrientationManager.h"

#include "SandoxBattler.h" // Project

SandoxBattler::SandoxBattler()
    :m_LastMousePos{0.0f,0.0f}
    ,m_LBHeldTimer{std::make_unique<Time>(0.1f)}
{

}

SandoxBattler::~SandoxBattler()
{
    Cleanup();
}
void SandoxBattler::Cleanup()
{

}

void SandoxBattler::Draw() const
{
    glPushMatrix();

    glTranslatef(0.0f,0.0f,0.0f);
    glScalef(1.0f,1.0f,0.0f);


    glPopMatrix();
}
void SandoxBattler::DrawUI(Rectf viewport) const
{

}

void SandoxBattler::Update(float elapsedTime)
{
    m_LBHeldTimer->Update(elapsedTime);
   
}
void SandoxBattler::UpdateTextures() 
{

}

void SandoxBattler::OnMouseDown()
{
    m_LBHeldTimer->Start();

}
void SandoxBattler::OnMouseUp()
{
    m_LBHeldTimer->RestartAndPause();

}
void SandoxBattler::OnMouseMotion(const Point2f& mousePos)
{
    m_LastMousePos = mousePos;

}
