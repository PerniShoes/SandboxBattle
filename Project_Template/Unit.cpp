#include "Unit.h"

Unit::Unit(std::string unitName,UnitType type, Transform transform,Stats baseStats, Color4f color)
    :m_Type{type}
    ,m_Target{nullptr}
    ,m_Stats{baseStats}
    ,m_Destination{}
    ,m_Transform{transform}
    ,m_HitBoxWidth{0.0f}
    ,m_ModelColor{color}
    ,m_InRange{false}
    ,m_IsAlive{true}
    ,m_GoingLeft{false}
    ,m_Animator{
    std::make_unique<UnitAnimator>(GameResources::m_AtlasManager.GetAtlas(unitName)
        ,std::make_unique<Texture>(GameResources::m_AtlasManager.GetAtlas(unitName)->pngPath))
    
    }
    ,m_UnitAnimatorLoadedCorrectly{true}

{
    
    if (GameResources::m_AtlasManager.GetAtlas(unitName)->name != unitName)
    {
        m_UnitAnimatorLoadedCorrectly = false;
    }

    m_HitBoxWidth = m_Animator->GetHitBoxWidth();
    m_Transform.hitboxWidth = m_HitBoxWidth;

    // FIX (debug purpose)
    m_Animator->Play("breathing");
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
            
    if (m_UnitAnimatorLoadedCorrectly)
    {
        m_Animator->DrawShade();
    }
    else
    {
        // Body
        SetColor(m_ModelColor);
        FillRect(0,0,m_HitBoxWidth,m_HitBoxWidth);

        // Outline
        SetColor(GetColor(black)); 
        DrawRect(0,0,m_HitBoxWidth,m_HitBoxWidth);

    }
    m_Transform.Pop();
}
void Unit::DrawHighlight() const
{
    using namespace utils;
    using namespace PrettyColors;
    m_Transform.Push(); 
    m_Transform.Apply();

    float offset{5.0f + m_HitBoxWidth/20.0f};
    offset = std::min(offset,8.0f);
    float width{m_HitBoxWidth + offset*2};
    float lineWidth{1.0f};

    Color4f highLight{GetColor(green)};
    // highLight.a = 1.0f;
    SetColor(highLight);
    DrawRect(0-offset,0-offset,width,width,lineWidth);

    m_Transform.Pop();
}

void Unit::Update(float elapsedTime)
{
    m_Animator->Update(elapsedTime);

}

void Unit::TeleportTo(Point2f position)
{
    m_Transform.position = position;
}

Rectf Unit::GetHitBox()
{
    // (scale used only for uniform scaling)
    return m_Transform.GetHitbox();
}

// Probably DELETE
void Unit::MoveTo(Point2f destination)
{
    m_Destination = destination;

}
//

bool Unit::MoveTowardsDestination(Point2f destination, float elapsedTime)
{
    // if one cord changes most likely both will (if only y changes, it's fine to not flip the unit)
    if (m_Destination.x != destination.x)
    {
        m_Animator->Play("run");
        if (m_Destination.x > destination.x && !m_GoingLeft)
        {
            m_GoingLeft = true;
            m_Transform.FlipX();
        }
        else if (m_Destination.x < destination.x && m_GoingLeft)
        {
            m_GoingLeft = false;
            m_Transform.FlipX();
        }
        m_Destination = destination;
    }
    Vector2f direction{destination - m_Transform.position};
    float tolerance{3.0f};

    // Arrived at destination
    if (std::abs(direction.x) < tolerance && std::abs(direction.y) < tolerance)
    {
        m_Animator->Play("breathing");
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