#include <cmath>
#include <iostream>
#include <print>

#include "Time.h"
#include "Texture.h"
#include "OrientationManager.h"

#include "SandboxBattler.h" // Project

SandboxBattler::SandboxBattler()
    :m_LastMousePos{0.0f,0.0f}
    ,m_LBHeldTimer{std::make_unique<Time>(0.1f)}
{

}

SandboxBattler::~SandboxBattler()
{
    Cleanup();
}
void SandboxBattler::Cleanup()
{

}

void SandboxBattler::Draw() const
{
    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.0f);
    glScalef(1.0f,1.0f,0.0f);



    glPopMatrix();
}
void SandboxBattler::DrawUI(Rectf viewport) const
{

}

void SandboxBattler::Update(float elapsedTime)
{

    m_LBHeldTimer->Update(elapsedTime);
   
}
void SandboxBattler::UpdateTextures() 
{

}

void SandboxBattler::OnMouseDown()
{
    m_LBHeldTimer->Start();

}
void SandboxBattler::OnMouseUp()
{
    m_LBHeldTimer->RestartAndPause();

}
void SandboxBattler::OnMouseMotion(const Point2f& mousePos)
{
    m_LastMousePos = mousePos;

}
