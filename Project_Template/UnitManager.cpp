#include "UnitManager.h"
#include "utils.h"
#include "AllUnitCommands.h"

UnitManager::UnitManager()
    :m_LastMousePos{}
    ,m_QueuingEnabled{false}
    ,m_SelectedUnits{}
    ,m_Units{}
    ,m_Queues{}
{


}

UnitManager::~UnitManager()
{

}

void UnitManager::DrawAll()const
{

    for (const auto unitPtr : m_SelectedUnits)
    {
        unitPtr->DrawSelection();
    }

    for (const auto& unitPtr : m_Units)
    {
        unitPtr->Draw();
    }
  
}

void UnitManager::UpdateAll(float elapsedTime)
{
    for (auto& [unitPtr,cmdQueue] : m_Queues)
    {
        if (cmdQueue.empty()) continue;
        auto& cmd = cmdQueue.front();

        cmd->Execute(unitPtr, elapsedTime);
        if (cmd->IsFinished())
        {
            cmdQueue.pop();
        }
    }

}

void UnitManager::AddUnit(std::unique_ptr<Unit> unit) 
{
    m_Units.push_back(std::move(unit));
}

void UnitManager::SelectClickedUnit()
{
    for (const auto& unitPtr : m_Units)
    {
        if (utils::IsPointInCircle(m_LastMousePos,unitPtr->GetHitBox()))
        {
            auto unit = unitPtr.get();
            if (std::find(m_SelectedUnits.begin(),m_SelectedUnits.end(),unit) == m_SelectedUnits.end())
            {
                m_SelectedUnits.push_back(unit);
            }
            break;
        }
    }

    // Remove raw pointers when units die 
}

void UnitManager::OnMouseMotion(const Point2f& mousePos)
{
    m_LastMousePos = mousePos;
}
void UnitManager::OnLeftButtonDown()
{
    // Later test for various scenarios, like clicking on ground or unit etc. FIX
    SelectClickedUnit();

}
void UnitManager::OnRightButtonDown()
{
    // Later test for various scenarios, like clicking on ground or unit etc. FIX

    for (const auto& unitPtr : m_SelectedUnits)
    {
        IssueCommand(unitPtr,std::make_unique<MoveCommand>(m_LastMousePos));
    }

}

void UnitManager::TeleportAllTo(Point2f location)
{
    for (const auto& unitPtr : m_Units)
    {
        unitPtr->TeleportTo(location);
    }
}
void UnitManager::TeleportTo(int unitIndex,Point2f location)
{
    m_Units[unitIndex]->TeleportTo(location);
}

void UnitManager::DeSellectAll()
{
    m_SelectedUnits.clear();
}
void UnitManager::IssueCommand(Unit* unit, CommandPtr command)
{
    if (m_QueuingEnabled)
    {
        m_Queues[unit].emplace(std::move(command));
    }

}
void UnitManager::EnableQueuing()
{
    m_QueuingEnabled = true;
}

void UnitManager::DisableQueuing()
{
    m_QueuingEnabled = false;
}