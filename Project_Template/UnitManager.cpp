#include "UnitManager.h"
#include "utils.h"
#include "AllUnitCommands.h"
#include <print>

UnitManager::UnitManager()
    :m_LastMousePos{}
    ,m_QueuingEnabled{false}
    ,m_SelectedUnits{}
    ,m_Units{}
    ,m_Queues{}
    ,m_DefaultTeam{0}
{


}

UnitManager::~UnitManager()
{

}

void UnitManager::DrawAll()const
{

    for (const auto unitPtr : m_SelectedUnits)
    {
        unitPtr->DrawHighlight();
    }

    for (const auto& unitPtr : m_Units)
    {
        unitPtr->Draw();
    }
  
}

void UnitManager::UpdateAll(float elapsedTime)
{
    
    for (const auto& unitPtr : m_Units)
    {
        unitPtr->Update(elapsedTime);
    }

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
    unit->ChangeTeam(m_DefaultTeam);
    m_Units.push_back(std::move(unit));

}

void UnitManager::SelectClickedUnit()
{
    // Prolly just dissallow selecting various team units, or selecting more than one

    for (const auto& unitPtr : m_Units)
    {
        if (utils::IsPointInRect(m_LastMousePos,unitPtr->GetSelectionBox()))
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
    using namespace std;
    // Later test for various scenarios, like clicking on ground or unit etc. FIX

    // attack command
    // attack beheviour
    // is enemy check

    // Check if clicked on a Unit
    for (const auto& unitPtr : m_Units)
    {
        // Unit clicked found
        if (utils::IsPointInRect(m_LastMousePos,unitPtr->GetSelectionBox())) // FIX
        {
            for (const auto& unitSelectedPtr : m_SelectedUnits)
            {
                if (unitSelectedPtr->GetTeamID() != unitPtr->GetTeamID())
                {
                    // Issue attack
                    // FIX
                    // Fix to check range/if can attack etc. ... For now it *just* attacks
                    unitSelectedPtr->Attack(unitPtr.get());

                    println("Unit with teamNr: {}, dealt: {} damage to unit with teamNr: {}, health of both: first one: {}, second one: {}"
                        ,unitSelectedPtr->GetTeamID()
                        ,unitSelectedPtr->GetStats().m_Damage
                        ,unitPtr->GetTeamID()
                        ,unitSelectedPtr->GetStats().m_CurrentHealth
                        ,unitPtr->GetStats().m_CurrentHealth);
                    
                }
            }
            // Skip move command since clicked on unit
            return;
        }
    }

    // If clicked on ground
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
    
   m_Queues[unit].emplace(std::move(command));

}
void UnitManager::EnableQueuing()
{
    m_QueuingEnabled = true;
}

void UnitManager::DisableQueuing()
{
    m_QueuingEnabled = false;
}
void UnitManager::SetDefaultTeam(int teamID)
{
    m_DefaultTeam = teamID;
}
void UnitManager::ChangeUnitTeam(int unitIndex,int newTeamID)
{
    m_Units[unitIndex]->ChangeTeam(newTeamID);
}
int UnitManager::GetUnitCount()const
{
    return int(m_Units.size());
}

void UnitManager::ScaleAllUnits(float x,float y)
{
    for (const auto& unitPtr : m_Units)
    {
        unitPtr->Scale(x,y);
    }
}