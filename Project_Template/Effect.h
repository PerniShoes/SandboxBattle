#pragma once
#include "ActionTarget.h"
#include <memory>

class Unit;
class Effect
{
public:
    Effect(Unit* caster, float duration=5.0f,ActionTarget targets=ActionTarget::enemy, int power=10);

    virtual void Apply(Unit* self,Unit* target)=0;
    virtual void Tick(Unit* target,float elapsedTime)=0;
    bool IsExpired() const;

protected:

    Unit* m_Caster;
    float m_Duration;
    int m_Power;

    ActionTarget m_Targets;
    float m_ElapsedTime;
    bool m_IsExpired;

};
