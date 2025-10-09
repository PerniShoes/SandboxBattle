#include "Ability.h"

Ability::Ability(Unit* caster,ActionTarget castTarget,vsTarget possibleTargets
    ,bool isAttack ,bool isProj,int damage, float range
    ,float cooldown)
    :m_Caster{caster}
    ,m_CastTarget{castTarget}
    ,m_Targets{possibleTargets}
    ,m_IsAttack{isAttack}
    ,m_IsProjectile{isProj}
    ,m_Damage{damage}
    ,m_Range{range}
    ,m_Cooldown{cooldown}
    ,m_Effects{}
{


}

void Ability::AddEffect(std::unique_ptr<Effect> effect)
{
    m_Effects.push_back(std::move(effect));
}