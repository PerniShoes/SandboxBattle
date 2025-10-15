#include "Unit.h"

Unit::Unit(std::string unitName,UnitType type, Transform transform,Stats baseStats, Color4f color)
    :m_Type{type}
    ,m_Target{nullptr}
    ,m_IsAttacking{false}
    ,m_Stats{baseStats}
    ,m_Destination{}
    ,m_Transform{transform}
    ,m_HitBoxWidth{0.0f}
    ,m_ModelColor{color}
    ,m_InRange{false}
    ,m_IsAlive{true}
    ,m_FacingLeft{false}
    ,m_Animator{
    std::make_unique<UnitAnimator>(GameResources::m_AtlasManager.GetAtlas(unitName)
        ,std::make_unique<Texture>(GameResources::m_AtlasManager.GetAtlas(unitName)->pngPath))
    
    }
    ,m_UnitAnimatorLoadedCorrectly{true}
    ,m_TeamNumber{0}
    ,m_SelectionRect{}

{
    
    if (GameResources::m_AtlasManager.GetAtlas(unitName)->name != unitName)
    {
        m_UnitAnimatorLoadedCorrectly = false;
    }

    m_HitBoxWidth = m_Animator->GetSpriteWidth();
    m_Transform.hitboxWidth = m_HitBoxWidth;
    m_Transform.hitboxHeight = m_HitBoxWidth;
    // Offset so units and their highlight center is at cursor, not left bottom
    m_Transform.offsetY = -20.0f;
    m_Transform.offsetX = -m_HitBoxWidth/2.0f;

    m_SelectionRect = {
    Rectf{0.0f+(m_HitBoxWidth/2.0f)-25.0f
        ,10.0f
        ,50.0f // (left*2)(-25.0f * 2) to make it centered
        ,30.0f
        }
    };


    // FIX (debug purpose)
    m_Animator->Play("breathing");
    //

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

    glPushMatrix();
    glTranslatef(m_HitBoxWidth / 2.0f,m_HitBoxWidth/2.0f, 0); // Translate to offset initial Unit to origin translate
    glScalef(0.5f,0.5f,0.0f);
    glTranslatef(-m_CircleShadow->GetWidth() / 2.0f,-m_CircleShadow->GetHeight() / 2.0f,0);
    m_CircleShadow->Draw();
    glPopMatrix();

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
    // Has to be outside since unit flips and UI doesn't
    DrawUi();

}
void Unit::DrawUi() const
{
    glPushMatrix();
    // General transform
    float offsetY{-10.0f};
    // FIX FIX FIX, very hacky, either refactor Transform to now have such hacky code in Apply or give it a helper (to give translate or fix it or smth)
    // It might be pretty good actually, but I can improve it greatly:
    
    // Code from transform:
    // 
 
    // I could add a "MoveFromOrigin()" here (would be the removed hitbox translate from line bellow)
 /*   glTranslatef(position.x + (hitboxWidth / 2.0f * abs(scale.x) + (offsetX * abs(scale.x))) // Here I could remove the hitbox translate
        ,position.y + (hitboxHeight / 2.0f * abs(scale.y) + (offsetY * abs(scale.y))),0);
    glScalef(scale.x,scale.y,0);
    glRotatef(angle,0,0,1);

    glTranslatef(-hitboxWidth / 2.0f,-hitboxHeight / 2.0f,0);*/ // This line can be "MoveToOrigin();


    // So transforming a unit would be:
    // ( READ BOTTOM UP )
    // 
    // MoveFromOrigin(); // Fixes initial MoveToOrigin() (just Move to origin but opposite numbers)
    // m_Transform.Apply(); // rotation scale and moving it to target position (but translate would be too small which is fixed above)
    // MoveToOrigin();   (glTranslatef(-hitboxWidth / 2.0f,-hitboxHeight / 2.0f,0);)


    // Then if I want something like Attack UI
    // (bottom up)
    // 
    // I would be able to do:
    // 
    // glPushMatrix();
    // Translate to position (Position relative to parent, since Apply was used before)
    // m_Transform.Apply(); (Scales, rotates and moves the same as the "parent" (Unit, here)
    // Scale (so I can adjust it's own size)
    // Translate to origin
    // glPopMatrix();

    //

    glTranslatef(m_Transform.position.x + (0.0f * abs(m_Transform.scale.x))
        ,m_Transform.position.y + (offsetY * abs(m_Transform.scale.y)),0);

        glPushMatrix();
            glTranslatef(-m_HitBoxWidth / 4.0f,0.0f,0);
            glScalef(0.5f,0.5f,0.0f);
            glTranslatef(-m_AttackUi->GetWidth() / 2.0f,-m_AttackUi->GetHeight() / 2.0f,0);
            m_AttackUi->Draw();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(m_HitBoxWidth / 4.0f,0.0f,0);
            glScalef(0.5f,0.5f,0.0f);
            glTranslatef(-m_HealthUi->GetWidth() / 2.0f,-m_HealthUi->GetHeight() / 2.0f,0);
            m_HealthUi->Draw();
        glPopMatrix();

   
    //
    glPopMatrix();
}
void Unit::DrawHighlight() const
{
    using namespace utils;
    using namespace PrettyColors;
    m_Transform.Push(); 
    m_Transform.Apply();

    Color4f highLight{GetColor(green)};
    // highLight.a = 1.0f;
    switch (m_TeamNumber)
    {
    case 0:
        highLight = GetColor(white);
        break;
    case 1:
        highLight = GetColor(blue);
        break;
    case 2:
        highLight = GetColor(purple);
        break;
    case 3:
        highLight = GetColor(orange);
        break;

    case 4:
        highLight = GetColor(yellow);
        break;
    default:
        highLight = GetColor(green);
        break;
    }
    SetColor(highLight);

    float lineWidth{1.0f};
    DrawRect(m_SelectionRect,lineWidth);
    //DrawRect(Rectf{0.0f,0.0f,m_HitBoxWidth,m_HitBoxWidth},lineWidth);

    m_Transform.Pop();
}

void Unit::Update(float elapsedTime)
{
    if (m_IsAttacking)
    {
        // Finished attack anim
        if (m_Animator->GetCurrentAnimation() != "attack")
        {
            m_Target->TakeDamage(m_Stats.m_Damage);
            m_Target = nullptr;
            m_IsAttacking = false;
        }
    }

    m_Animator->Update(elapsedTime);
}

void Unit::TeleportTo(Point2f position)
{
    m_Transform.position = position;
}

// Probably DELETE
void Unit::MoveTo(Point2f destination)
{
    m_Destination = destination;

}
//

bool Unit::MoveTowardsDestination(Point2f destination, float elapsedTime)
{
    float xTolerance{5.0f};
    if (m_Destination.x != destination.x || m_Destination.y != destination.y)
    {
        m_Animator->Play("run");
        // Flips based on X change
        if (m_Transform.position.x - xTolerance > destination.x && !m_FacingLeft)
        {
            m_FacingLeft = true;
            m_Transform.FlipX();
        }
        else if (m_Transform.position.x + xTolerance < destination.x && m_FacingLeft)
        {
            m_FacingLeft = false;
            m_Transform.FlipX();
        }
        m_Destination = destination;
    }
    Vector2f direction{destination - m_Transform.position};
    float tolerance{5.0f};

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

   if (!m_IsAlive)
   {
       m_Animator->Play("death");
   }
   else
   {
       m_Animator->Play("hit");
   }

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
int Unit::GetTeamID()const
{
    return m_TeamNumber;
}
void Unit::ChangeTeam(int newID)
{
    m_TeamNumber = newID;
}
void Unit::Attack(Unit* target)
{
    if (m_Transform.position.x > target->GetTransform().position.x && !m_FacingLeft)
    {
        m_FacingLeft = true;
        m_Transform.FlipX();
    }
    else if(m_Transform.position.x < target->GetTransform().position.x && m_FacingLeft)
    {
        m_FacingLeft = false;
        m_Transform.FlipX();
    }
    m_Animator->Play("attack");
    m_Target = target;
    m_IsAttacking = true;
}
Transform Unit::GetTransform()const
{
    return m_Transform;
}
void Unit::Scale(float x,float y)
{
    m_Transform.Scale(x, y);
}
Rectf Unit::GetHitBox() const
{
    return m_Transform.GetHitbox();
}
Rectf Unit::GetSelectionBox() const
{
    Rectf hitBox{m_Transform.GetHitbox()};
    hitBox.left += (m_SelectionRect.left) * abs(m_Transform.scale.x);
    hitBox.bottom += (m_SelectionRect.bottom) * abs(m_Transform.scale.y);
    hitBox.width = (m_SelectionRect.width) * abs(m_Transform.scale.x);
    hitBox.height = (m_SelectionRect.height)* abs(m_Transform.scale.y);

    // Adjust hitbox with selectionReeeect
    return hitBox;
}

void Unit::SetFrameTime(float frameTimeTarget)
{
    m_Animator->SetFrameTime(frameTimeTarget);
}
// Add setters for Stats
void Unit::SetStats(Stats newStats)
{
    m_Stats = newStats;
}

// Called after team number is set
void Unit::LoadTextures()
{
    // For debug purposes team 0 is ally, team 1 enemy (also commented in game.cpp under m_UnitManager funcs)
    
    // UI
    if (m_TeamNumber == 0)
    {
        m_AttackUi = std::make_unique<Texture>("../Resources/DuelystResc/ui/icon_atk.png");
        m_HealthUi = std::make_unique<Texture>("../Resources/DuelystResc/ui/icon_hp.png");
    }
    else
    {
        m_AttackUi = std::make_unique<Texture>("../Resources/DuelystResc/ui/icon_atk_bw.png");
        m_HealthUi = std::make_unique<Texture>("../Resources/DuelystResc/ui/icon_hp_bw.png");
    }

    // VFX
    // Same pos as unit? Has to be scaled to match unit
    m_CircleShadow = std::make_unique<Texture>("../Resources/DuelystResc/ui/unit_shadow.png");

    // actual shadow (later)

}