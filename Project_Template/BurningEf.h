#pragma once
#include "Effect.h"

class BurningEf : public Effect
{
public:
    BurningEf(Unit* caster);
    BurningEf(Unit* caster,float duration,ActionTarget targets,int power);

    virtual void Apply(Unit* self,Unit* target) override;
    virtual void Tick(Unit* target,float elapsedTime) override;

private:


};

