#pragma once
#include <vector>
#include <memory>
#include "Stats.h"
#include "Transform.h"

#include "AllEffects.h"
#include "Effect.h"

enum class UnitType
{
    ground = 1,
    air = 2
};

class Unit
{
public:
    Unit(UnitType type = {UnitType::ground},Transform transform = {},Stats baseStats = {},float hitBoxRadius = 10.0f,Color4f color = Color4f{1,1,1,1});
    Unit(const Unit& other) = delete;
    Unit& operator=(const Unit& other) = delete;
    Unit(Unit&& other) = delete;
    Unit& operator=(Unit&& other) = delete;

    ~Unit();

    void Draw() const;
    void DrawHighlight()const;
    virtual void Update(float elapsedTime);

    bool MoveTowardsDestination(Point2f destination,float elapsedTime);
    void MoveTo(Point2f destination);
    void Attack();

    void TeleportTo(Point2f position);
    void TakeDamage(int amount);
    void Heal(int amount);
    Stats GetStats() const;
    UnitType GetType() const;

    Transform GetTransform(); // For collision, position and so on
    Circlef GetHitBox();
    void ApplyBuff(std::unique_ptr<Effect> buff);
    void ApplyDebuff(std::unique_ptr<Effect> debuff);

private:

    UnitType m_Type;
    Stats m_Stats;
    Unit* m_Target;
    Point2f m_Destination;
    Transform m_Transform;
    float m_HitBoxRadius;
    Color4f m_ModelColor;
    bool m_InRange;
    bool m_IsAlive;

    std::vector<std::unique_ptr<Effect>>m_Buffs;
    std::vector<std::unique_ptr<Effect>>m_Debuffs;

    // Buffs and Debuffs and vector with effects, like buring, slowed, strength and so on
    // Class like "Abbilities" that would allow me to do stuff like: FireBall(this, target) (as in: (caster, target))



};

