#pragma once

struct Stats
{

    Stats(int hp = 100, int dmg = 10, float as = 1.0f, int range = 10, int ms = 1000, int exp = 0, int lvl = 1);
    
    int m_MaxHealth;
    int m_CurrentHealth;
    int m_Damage;
    float m_AttackSpeed;
    int m_Range;
    int m_MoveSpeed;

    int m_Experience;
    int m_Level;

};

