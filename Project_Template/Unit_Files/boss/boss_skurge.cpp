#include "boss_skurge.h"
#include "Texture.h"

boss_skurge::boss_skurge()
    : Unit("boss_skurge")
{
    Stats newStats{this->GetStats()};
    newStats.m_AttackRangeTiles=3;
    this->SetStats(newStats);
}

boss_skurge::~boss_skurge()
{

}
