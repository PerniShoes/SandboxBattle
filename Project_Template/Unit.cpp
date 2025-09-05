#include "Unit.h"
#include "PrettyColors.h"

Unit::Unit(UnitType type, Transform transform,Stats baseStats,float hitBoxRadius, Color4f color)
    :m_Type{type}
    ,m_Target{nullptr}
    ,m_Stats{baseStats}
    ,m_Destination{}
    ,m_Transform{transform}
    ,m_HitBoxRadius{hitBoxRadius}
    ,m_ModelColor{color}
    ,m_InRange{false}
    ,m_IsAlive{true}
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
void Unit::DrawHighlight() const
{
    using namespace utils;
    using namespace PrettyColors;
    m_Transform.Push(); 
    m_Transform.Apply();

    float offset{5.0f+ m_HitBoxRadius/30.0f};
    float radius{m_HitBoxRadius + offset};
    float lineWidth{1.0f+ m_HitBoxRadius / 50.0f};
    lineWidth = std::min(lineWidth,5.0f);

    SetColor(GetColor(green));
    DrawEllipse(0,0,radius,radius,lineWidth);

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

void Unit::ApplyBuff(std::unique_ptr<Effect> buff)
{
    m_Buffs.push_back(std::move(buff));
}
void Unit::ApplyDebuff(std::unique_ptr<Effect> debuff)
{
    m_Debuffs.push_back(std::move(debuff));
}
void Unit::TakeDamage(int amount)
{
   m_IsAlive = m_Stats.TakeDamage(amount);
}
void Unit::Heal(int amount)
{
    m_Stats.Heal(amount);
}
Stats Unit::GetStats() const
{
    return m_Stats;
}
UnitType Unit::GetType() const
{
    return m_Type;
}