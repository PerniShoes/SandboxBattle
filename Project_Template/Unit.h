#pragma once
#include <vector>
#include <memory>
#include "Stats.h"
#include "Transform.h"

#include "AllEffects.h"
#include "Effect.h"
#include "SpriteAtlasManager.h"
#include "GameResources.h"
#include "UnitAnimator.h"
#include "PrettyColors.h"

// Useless?
enum class UnitType
{
    ground = 1,
    air = 2
};

class Texture;
class Unit
{
public:
    Unit(std::string unitName, UnitType type = {UnitType::ground},Transform transform = {},Stats baseStats = {},Color4f color = PrettyColors::GetColor(PrettyColors::dGreen));
    Unit(const Unit& other) = delete;
    Unit& operator=(const Unit& other) = delete;
    Unit(Unit&& other) = delete;
    Unit& operator=(Unit&& other) = delete;
    virtual ~Unit();

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

    Transform GetTransform(); // For collision, position and so on // Not implt
    Rectf GetHitBox();
    void ApplyBuff(std::unique_ptr<Effect> buff);
    void ApplyDebuff(std::unique_ptr<Effect> debuff);



    // ORGANIZE ALL NOT USED STUFF SINCE A LOT OF CHANGED, ALSO HAD SCALING 
protected:

    UnitType m_Type;
    Stats m_Stats;
    Unit* m_Target;
    Point2f m_Destination;
    Transform m_Transform;
    float m_HitBoxWidth;
    Color4f m_ModelColor; // For debug if texture doesn't load
    bool m_InRange;
    bool m_IsAlive;
    bool m_GoingLeft;

    // TODO
    std::vector<std::unique_ptr<Effect>>m_Buffs;
    std::vector<std::unique_ptr<Effect>>m_Debuffs;

    /// UA 
    std::unique_ptr<UnitAnimator> m_Animator;
    bool m_UnitAnimatorLoadedCorrectly;



    // Buffs and Debuffs and vector with effects, like buring, slowed, strength and so on
    // Class like "Abbilities" that would allow me to do stuff like: FireBall(this, target) (as in: (caster, target))



};

