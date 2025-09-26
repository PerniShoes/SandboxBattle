#include "Stats.h"

Stats::Stats(int hp,int maxHp,int dmg,float castSpeed,int ms)
    :m_CurrentHealth{hp}
    ,m_MaxHealth{maxHp}
    ,m_Damage{dmg}
    ,m_CastSpeed{castSpeed}
    ,m_MoveSpeed{ms}
 
{


}

void Stats::Heal(int amount)
{
    m_CurrentHealth += amount;
    if (m_CurrentHealth > m_MaxHealth)
    {
        m_CurrentHealth = m_MaxHealth;
    }
}
bool Stats::TakeDamage(int amount)
{
    m_CurrentHealth -= amount;
    if (m_CurrentHealth < 0)
    {
        m_CurrentHealth = 0;
        return false;
    }
    return true;
}



