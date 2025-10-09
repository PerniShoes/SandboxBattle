#pragma once
#include "Command.h"
#include "utils.h"

class Unit;
class MoveCommand : public Command
{
public:
    MoveCommand(Point2f destination);

    void Execute(Unit* unit, float elapsedtime) override;
    bool IsFinished() const override;

private:

    Point2f m_Destination;
    bool m_IsFinished;
};

