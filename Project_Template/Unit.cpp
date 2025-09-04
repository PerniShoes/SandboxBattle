#include "Unit.h"
#include "Stats.h"
#include "PrettyColors.h"

Unit::Unit(Transform transform,Stats baseStats,float hitBoxRadius, Color4f color)
    :m_Stats{baseStats}
    ,m_Target{nullptr}
    ,m_Destination{}
    ,m_Transform{transform}
    ,m_HitBoxRadius{hitBoxRadius}
    ,m_ModelColor{color}
    ,inRange{false}
{


}

Unit::~Unit()
{


}

void Unit::Draw() const
{
    using namespace utils;
    using namespace PrettyColors;
    m_Transform.Push();
    m_Transform.Apply();

    // Body
    SetColor(GetColor(dGreen));
    FillEllipse(0,0,m_HitBoxRadius,m_HitBoxRadius);

    // Outline
    SetColor(GetColor(black)); 
    DrawEllipse(0,0,m_HitBoxRadius,m_HitBoxRadius);


    m_Transform.Pop();
}
void Unit::DrawSelection() const
{
    using namespace utils;
    using namespace PrettyColors;
    m_Transform.Push(); 
    m_Transform.Apply();

    float offset{5.0f};
    float radius{m_HitBoxRadius + offset};

    SetColor(GetColor(green));
    DrawEllipse(0,0,radius,radius);

    m_Transform.Pop();
}

void Unit::Update(float elapsedTime)
{
   


}

void Unit::TeleportTo(Point2f position)
{
    m_Transform.position = position;
}

Circlef Unit::GetHitBox()
{
    return Circlef{m_Transform.position,m_HitBoxRadius};
}

void Unit::MoveTo(Point2f destination)
{
    m_Destination = destination;
}
bool Unit::MoveTowardsDestination(Point2f destination, float elapsedTime)
{
    Vector2f direction{destination - m_Transform.position};
    float tolerance{3.0f};

    if (std::abs(direction.x) < tolerance && std::abs(direction.y) < tolerance)
    {
        return true;
    }

    direction = direction.Normalized();
    m_Transform.position += direction *float(m_Stats.m_MoveSpeed) * elapsedTime;
    return false; 

}