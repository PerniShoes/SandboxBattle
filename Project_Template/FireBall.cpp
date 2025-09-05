#include "FireBall.h"

FireBall::FireBall(Unit* caster)
    :Ability(caster,ActionTarget::enemy,vsTarget::ground,false,true,300,500.0f,10.0f)
{
    auto burning = std::make_unique<BurningEf>(caster);
    AddEffect(std::move(burning));


}