#include "Stats.h"

Stats::Stats(int hp,int maxHp,int dmg, int attackRange,int ms, int moveRange)
    :m_CurrentHealth{hp}
    ,m_MaxHealth{maxHp}
    ,m_CurrentDamage{dmg}
    ,m_InitialDamage{dmg}
    ,m_AttackRangeTiles{attackRange}
    ,m_MoveSpeed{ms}
    ,m_MoveRangeTiles{moveRange}
 
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
    if (m_CurrentHealth <= 0)
    {
        m_CurrentHealth = 0;
        return false;
    }
    return true;
}



