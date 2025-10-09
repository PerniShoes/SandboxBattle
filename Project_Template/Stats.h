#pragma once

struct Stats
{

    Stats(int hp = 100, int maxHp = 100,int dmg = 10.0f, float castSpeed = 1.0f, int ms = 1000);
    
    void Heal(int amount);
    bool TakeDamage(int amount);

    int m_MaxHealth;
    int m_CurrentHealth;
    int m_Damage;
    float m_CastSpeed;
    int m_MoveSpeed;

};

