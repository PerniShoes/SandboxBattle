#include "MoveCommand.h"
#include "Unit.h"

MoveCommand::MoveCommand(Point2f destination)
    :m_Destination{destination}
    ,m_IsFinished{false}
{

}

void MoveCommand::Execute(Unit* unit,float elapsedTime)
{
    m_IsFinished = unit->MoveTowardsDestination(m_Destination,elapsedTime);
}

bool MoveCommand::IsFinished() const
{ 
    return m_IsFinished;
}