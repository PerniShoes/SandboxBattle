#include "neutral_arrowwhistler.h"
#include "Texture.h"

neutral_arrowwhistler::neutral_arrowwhistler()
    : Unit("neutral_arrowwhistler")
{
    Stats newStats{this->GetStats()};
    newStats.m_AttackRangeTiles = 1000;
    this->SetStats(newStats);
}

neutral_arrowwhistler::~neutral_arrowwhistler()
{

}
