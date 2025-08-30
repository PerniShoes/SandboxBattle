#include <cmath>
#include <iostream>
#include <print>

#include "Time.h"
#include "Texture.h"
#include "OrientationManager.h"

#include "ProjectName.h" // Project

ProjectName::ProjectName()
    :m_LastMousePos{0.0f,0.0f}
    ,m_LBHeldTimer{std::make_unique<Time>(0.1f)}
{

}

ProjectName::~ProjectName()
{
    Cleanup();
}
void ProjectName::Cleanup()
{

}

void ProjectName::Draw() const
{
    glPushMatrix();

    glTranslatef(0.0f,0.0f,0.0f);
    glScalef(1.0f,1.0f,0.0f);


    glPopMatrix();
}
void ProjectName::DrawUI(Rectf viewport) const
{

}

void ProjectName::Update(float elapsedTime)
{
    m_LBHeldTimer->Update(elapsedTime);
   
}
void ProjectName::UpdateTextures() 
{

}

void ProjectName::OnMouseDown()
{
    m_LBHeldTimer->Start();

}
void ProjectName::OnMouseUp()
{
    m_LBHeldTimer->RestartAndPause();

}
void ProjectName::OnMouseMotion(const Point2f& mousePos)
{
    m_LastMousePos = mousePos;

}
