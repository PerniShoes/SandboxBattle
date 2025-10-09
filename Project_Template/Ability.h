#pragma once
#include <vector>
#include <iostream>
#include "ActionTarget.h"

#include "AllEffects.h"
#include "Effect.h"
#include <memory>

class Unit;
enum class vsTarget
{
    ground = 0,
    air = 1,
    all = 2
};


// Effects
// During initialization of unit, set what targets it can attack, based on abbilities it gets
class Ability
{
public:
    Ability(Unit* caster,ActionTarget castTarget = ActionTarget::enemy, vsTarget possibleTargets = vsTarget::all
        ,bool isAttack=true, bool isProj=false
        ,int damage = 100
        ,float range=100.0f ,float cooldown=5.0f);

    void AddEffect(std::unique_ptr<Effect> effect);

protected:
    Unit* m_Caster;
    ActionTarget m_CastTarget;
    vsTarget m_Targets;   
    bool m_IsAttack;        
    bool m_IsProjectile;
    int m_Damage;
    float m_Range;
    float m_Cooldown;
    std::vector<std::unique_ptr<Effect>>m_Effects;


    virtual void Execute(Unit& self,Unit& target)=0;
private:


};

