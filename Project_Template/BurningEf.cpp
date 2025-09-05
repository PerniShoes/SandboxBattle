#include "BurningEf.h"
#include "Unit.h"

BurningEf::BurningEf(Unit* caster)
    :Effect(caster, 10.0f, ActionTarget::enemy, 10)
{

}

BurningEf::BurningEf(Unit* caster,float duration,ActionTarget targets,int power)
    :Effect(caster,duration,targets,power)
{

}

void BurningEf::Apply(Unit* self,Unit* target)
{
   // target->ApplyDebuff(std::make_unique<BurningEf>(m_Caster,m_Duration,m_Targets,m_Power));

}
void BurningEf::Tick(Unit* target,float elapsedTime)
{
    target->TakeDamage(m_Power);
}