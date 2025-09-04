#include "Stats.h"

Stats::Stats(int hp,int dmg,float as,int range,int ms,int exp,int lvl)
    :m_MaxHealth{hp}
    ,m_CurrentHealth{hp}
    ,m_Damage{dmg}
    ,m_AttackSpeed{as}
    ,m_Range{range}
    ,m_MoveSpeed{ms}
    ,m_Experience{exp}
    ,m_Level{lvl}
{


}




