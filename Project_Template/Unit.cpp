#include "Unit.h"


Unit::Unit(std::string unitName,UnitType type, Transform transform,Stats baseStats, Color4f color)
    :m_Type{type}
    ,m_Target{nullptr}
    ,m_IsAttacking{false}
    ,m_IsCounterAttacking{false}
    ,m_WillCounterAttack{false}
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
    ,m_IsMoving{false}

{
    using namespace PrettyColors;
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

    // DEBUG for testing 
    Stats temp = GetStats();
    temp.m_CurrentHealth = 10;
    temp.m_MaxHealth = 7;
    temp.m_CurrentDamage = 3;
    SetStats(temp);

    m_AttackUiNumber = std::make_unique<Texture>("Attack","../Resources/Fonts/consola.ttf",24,GetColor(white));
    m_HealthUiNumber = std::make_unique<Texture>("Health","../Resources/Fonts/consola.ttf",24,GetColor(white));
    UpdateHealthTexture();
    UpdateAttackTexture();
}

Unit::~Unit()
{ 


}

void Unit::Draw() const
{
    using namespace utils;
    using namespace PrettyColors;

    glPushMatrix();
    m_Transform.Translate();
    glTranslatef(m_HitBoxWidth/2.0f*abs(m_Transform.scale.x),
        15.0f * abs(m_Transform.scale.y),0);
    glScalef(abs(m_Transform.scale.x),abs(m_Transform.scale.y),0.0f);
    glScalef(0.5f,0.5f,0.0f);
    glTranslatef(-m_CircleShadow->GetWidth() / 2.0f,-m_CircleShadow->GetHeight() / 2.0f,0);
    m_CircleShadow->Draw();
    glPopMatrix();

    m_Transform.Push();

    m_Transform.MoveFromOrigin();
    m_Transform.Apply();
    m_Transform.MoveToOrigin();

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
void Unit::DrawUi() const
{
    // General
    float sidewaysOffset{25.0f};
    float downOffset{5.0f};
    float uiScale{0.35f};
    // Numbers UI
    float numberUIOffset{23.0f};
    float multipliedOffset{numberUIOffset/3.2f};
    float upOffsetNumber{21.0f};

    // ATTACK UI
        glPushMatrix();
            glTranslatef(-sidewaysOffset,-downOffset,0);
            // Unit translates:
            glTranslatef(m_HitBoxWidth / 2.0f * abs(m_Transform.scale.x),
                15.0f * abs(m_Transform.scale.y),0);
            m_Transform.Translate();
            glScalef(abs(m_Transform.scale.x),abs(m_Transform.scale.y),0.0f);
            //
            glScalef(uiScale,uiScale,0.0f);
            glTranslatef(-m_AttackUi->GetWidth() / 2.0f,-m_AttackUi->GetHeight() / 2.0f,0);
            m_AttackUi->Draw();

        glPushMatrix();

        // Count digits to center text
        int numAtk{m_Stats.m_CurrentDamage};
        int lengthAtk{0};
        if (numAtk < 0) numAtk *= -1;
        for (int temp{numAtk}; temp > 0; temp /= 10)
        {
            ++lengthAtk;
        }
        if (numAtk == 0) lengthAtk = 1;

        glTranslatef((sidewaysOffset / 2.0f)+ numberUIOffset - (multipliedOffset * float(lengthAtk)),upOffsetNumber,0);
        m_AttackUiNumber->Draw();
        glPopMatrix();

        glPopMatrix();

    //
        
    // HEALTH UI
        glPushMatrix();
            glTranslatef(sidewaysOffset,-downOffset,0);
            // Unit translates:
            glTranslatef(m_HitBoxWidth / 2.0f * abs(m_Transform.scale.x),
                15.0f * abs(m_Transform.scale.y),0);
            m_Transform.Translate();
            glScalef(abs(m_Transform.scale.x),abs(m_Transform.scale.y),0.0f);
            //
            glScalef(uiScale,uiScale,0.0f);
            glTranslatef(-m_HealthUi->GetWidth() / 2.0f,-m_HealthUi->GetHeight() / 2.0f,0);
            m_HealthUi->Draw();

    glPushMatrix();

    // Count digits to center text
    int numHP{m_Stats.m_CurrentHealth};
    int lengthHP{0};
    if (numHP < 0) numHP *= -1;
    for (int temp{numHP}; temp > 0; temp /= 10)
    {
        ++lengthHP;
    }
    if (numHP == 0) lengthHP = 1;

    glTranslatef(sidewaysOffset/2.0f + numberUIOffset - (multipliedOffset * float(lengthHP)),upOffsetNumber,0);
    m_HealthUiNumber->Draw();
    glPopMatrix();
        glPopMatrix();

    //
}
void Unit::DrawHighlight() const
{
    using namespace utils;
    using namespace PrettyColors;
    m_Transform.Push(); 
    m_Transform.Translate();
    glScalef(abs(m_Transform.scale.x),abs(m_Transform.scale.y),0.0f);

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
            m_Target->TakeDamage(m_Stats.m_CurrentDamage);

            if (!this->m_IsCounterAttacking)
            {
                m_Target->CounterAttack(this);
            }
            else
            {
                // After counter attacking check if alive (can counterattack before dying)
                if (!m_IsAlive)
                {
                    m_Animator->Play("death");
                }
                if (m_Target->m_IsAlive == false)
                {
                    m_Target->PlayAnim("death");
                }
            }

            m_IsCounterAttacking = false;
            m_Target = nullptr;
            m_IsAttacking = false;
        }
    }
    if (m_WillCounterAttack)
    {
        if (m_Animator->GetCurrentAnimation() != "hit")
        {
            Attack(m_Target);
            m_WillCounterAttack = false;
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

// Probably a confusing name, why return bool?
bool Unit::MoveTowardsDestination(Point2f destination, float elapsedTime)
{
    float xTolerance{2.0f};
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
    float tolerance{2.0f};

    // Arrived at destination
    if (std::abs(direction.x) < tolerance && std::abs(direction.y) < tolerance)
    {
        m_Animator->Play("breathing");
        m_IsMoving = false;
        return true;
    }

    direction = direction.Normalized();
    m_Transform.position += direction *float(m_Stats.m_MoveSpeed) * elapsedTime;
    m_IsMoving = true;
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

   UpdateHealthTexture();

   m_Animator->Play("hit");
   
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
void Unit::CounterAttack(Unit* target)
{
    m_IsCounterAttacking = true;
    m_WillCounterAttack = true;
    m_Target = target;
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
void Unit::PlayAnim(std::string animName)
{
    m_Animator->Play(animName);
}
void Unit::UpdateAttackTexture()
{
    using namespace PrettyColors;
    int pixelSize{24};
    int currentAttack = m_Stats.m_CurrentDamage;
    Color4f attackColor{GetColor(white)};

    if (m_Stats.m_CurrentDamage > m_Stats.m_InitialDamage)
    {
        attackColor = GetColor(green);
    }

    m_AttackUiNumber = std::make_unique<Texture>(std::to_string(currentAttack),"../Resources/Fonts/consola.ttf",pixelSize,attackColor);
}
void Unit::UpdateHealthTexture()
{
    using namespace PrettyColors;
    int pixelSize{24};
    int currentHP = m_Stats.m_CurrentHealth;
    Color4f healthColor{GetColor(white)};

    if (m_Stats.m_CurrentHealth < m_Stats.m_MaxHealth)
    {
        healthColor = GetColor(red);
    }
    else if (m_Stats.m_CurrentHealth > m_Stats.m_MaxHealth)
    {
        healthColor = GetColor(green);
    }

    m_HealthUiNumber = std::make_unique<Texture>(std::to_string(currentHP),"../Resources/Fonts/consola.ttf",pixelSize,healthColor);
}
bool Unit::IsMoving() const
{
    return m_IsMoving;
}