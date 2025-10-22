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
    Unit(std::string unitName,UnitType type = {UnitType::ground},Transform transform = {},Stats baseStats = {},Color4f color = PrettyColors::GetColor(PrettyColors::dGreen));
    Unit(const Unit& other) = delete;
    Unit& operator=(const Unit& other) = delete;
    Unit(Unit&& other) = delete;
    Unit& operator=(Unit&& other) = delete;
    virtual ~Unit();
    
    void LoadTextures();

    void Draw() const;
    void DrawUi() const;
    void DrawHighlight()const;
    virtual void Update(float elapsedTime);
    void UpdateAttackTexture();
    void UpdateHealthTexture();

    bool MoveTowardsDestination(Point2f destination,float elapsedTime);
    void MoveTo(Point2f destination);
    void Attack(Unit* target, bool canBeCounterAttacked);
    void CounterAttack(Unit* target);

    void TeleportTo(Point2f position);
    void TakeDamage(int amount);
    void Heal(int amount);
    Stats GetStats() const;
    void SetStats(Stats newStats);
    UnitType GetType() const;
    int GetTeamID() const;
    void ChangeTeam(int newID);

    Transform GetTransform()const; // For collision, position and so on // Not implt
    Rectf GetHitBox() const;
    Rectf GetSelectionBox() const;

    void ApplyBuff(std::unique_ptr<Effect> buff);
    void ApplyDebuff(std::unique_ptr<Effect> debuff);
    void Scale(float x,float y);
    void SetFrameTime(float frameTimeTarget);
    void PlayAnim(std::string animName);
    bool IsMoving() const;
    // ORGANIZE, quite a but of not used stuff LATER
protected:

    UnitType m_Type;
    Stats m_Stats;
    Unit* m_Target;
    bool m_IsAttacking;
    bool m_IsCounterAttacking;
    bool m_WillCounterAttack;
    bool m_IsCounterAttackable;

    Point2f m_Destination;
    Transform m_Transform;
    float m_HitBoxWidth;  // Default hitbox (before scaling, rotating etc.)
    Color4f m_ModelColor; // For debug if texture doesn't load
    bool m_InRange;
    bool m_IsAlive;
    bool m_FacingLeft;
    int m_TeamNumber;
    Rectf m_SelectionRect;
    bool m_IsMoving;

    // TODO // Might be scracthed
    std::vector<std::unique_ptr<Effect>>m_Buffs;
    std::vector<std::unique_ptr<Effect>>m_Debuffs;

    /// UA 
    std::unique_ptr<UnitAnimator> m_Animator;
    bool m_UnitAnimatorLoadedCorrectly;

    std::unique_ptr<Texture>m_AttackUi;
    std::unique_ptr<Texture>m_AttackUiNumber;
    std::unique_ptr<Texture>m_HealthUi;
    std::unique_ptr<Texture>m_HealthUiNumber;
    std::unique_ptr<Texture>m_CircleShadow;



    // Buffs and Debuffs and vector with effects, like buring, slowed, strength and so on
    // Class like "Abbilities" that would allow me to do stuff like: FireBall(this, target) (as in: (caster, target))



};

