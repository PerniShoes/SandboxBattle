#pragma once

struct Stats
{

    Stats(int hp = 10,int maxHp = 10,int dmg = 10.0f,int attackRange = 1,int ms = 100,int moveRange = 2);
    
    void Heal(int amount);
    bool TakeDamage(int amount);

    int m_MaxHealth;
    int m_CurrentHealth;

    int m_CurrentDamage;
    int m_InitialDamage;
    int m_AttackRangeTiles;

    int m_MoveSpeed;
    int m_MoveRangeTiles;

};

