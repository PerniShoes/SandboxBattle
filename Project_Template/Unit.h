#pragma once
#include <vector>
#include "utils.h"
#include "Stats.h"
#include "Transform.h"

class Unit
{
public:
    Unit(Transform transform = {},Stats baseStats={},float hitBoxRadius = 10.0f,Color4f color = Color4f{1,1,1,1});
    Unit(const Unit& other) = delete;
    Unit& operator=(const Unit& other) = delete;
    Unit(Unit&& other) = delete;
    Unit& operator=(Unit&& other) = delete;

    ~Unit();

    void Draw() const;
    void DrawSelection()const;
    virtual void Update(float elapsedTime);

    bool MoveTowardsDestination(Point2f destination,float elapsedTime);
    void MoveTo(Point2f destination);
    void Attack();

    void TeleportTo(Point2f position);
    void TakeDamage(int amount);
    void GetExp(int amount);
    void LevelUp();

    Transform GetTransform(); // For collision, position and so on
    Circlef GetHitBox();

private:
    // For manager: Allow unit selection

    Stats m_Stats;
    Unit* m_Target;
    Point2f m_Destination;
    Transform m_Transform;
    float m_HitBoxRadius;
    Color4f m_ModelColor;
    bool inRange;

    // Buffs and Debuffs and vector with effects, like buring, slowed, strength and so on
    // Class like "Abbilities" that would allow me to do stuff like: FireBall(this, target) (as in: (caster, target))



};

