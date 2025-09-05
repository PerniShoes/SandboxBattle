#include "Effect.h"


Effect::Effect(Unit* caster, float duration, ActionTarget targets, int power)
    :m_Caster{caster}
    ,m_Duration{duration}
    ,m_Power{power}
    ,m_Targets{targets}
    ,m_ElapsedTime{0.0f}
    ,m_IsExpired{false}

{

}

bool Effect::IsExpired()const
{
    return m_IsExpired;
}