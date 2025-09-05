#pragma once

struct Stats
{

    Stats(int hp = 100, int maxHp = 100, float as = 1.0f, float castSpeed = 1.0f, int ms = 1000);
    
    void Heal(int amount);
    bool TakeDamage(int amount);

    int m_MaxHealth;
    int m_CurrentHealth;
    float m_AttackSpeed;
    float m_CastSpeed;
    int m_MoveSpeed;

};

