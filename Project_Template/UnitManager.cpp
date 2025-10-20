#include "UnitManager.h"
#include "utils.h"
#include "AllUnitCommands.h"
#include <print>
#include "Grid.h"
#include <cstdlib>

UnitManager::UnitManager(Rectf screenRect)
    :m_LastMousePos{}
    ,m_QueuingEnabled{false}
    ,m_ScreenRect{screenRect}
    ,m_SelectedUnits{}
    ,m_Units{}
    ,m_Queues{}
    ,m_DefaultTeam{0}
    ,m_HoverEnemy{false}
    ,m_HoverAlly{false}
    ,m_HoverGround{false}
    ,m_TilesPerColumn{5}
    ,m_TilesPerRow{9}

{
    float uniformSize{83.0f};
    Point2f defaultTileSize{uniformSize,uniformSize};
    Point2f defaultResolution{1920.0f,1080.0f};
    Point2f scale{screenRect.width/ defaultResolution.x,screenRect.height / defaultResolution.y};

    defaultTileSize.x *= scale.x;
    defaultTileSize.y *= scale.y;

    Point2f startingPointGrid{screenRect.width*0.29f,screenRect.height*0.29f};
    m_Grid = std::make_unique<Grid>(startingPointGrid,m_TilesPerColumn,m_TilesPerRow,Rectf{screenRect},defaultTileSize);
    m_TilesTaken.resize(m_Grid->GetTileAmount(),false);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

UnitManager::~UnitManager()
{

}

void UnitManager::DrawAll()const
{

    m_Grid->DrawGrid();
    for (const auto unitPtr : m_SelectedUnits)
    {
        unitPtr->DrawHighlight();
    }

    for (const auto& unitPtr : m_Units)
    {
        unitPtr->Draw();
    }
    // Seperate so UI is above no matter the position
    for (const auto& unitPtr : m_Units)
    {
        unitPtr->DrawUi();
    }
  
}

void UnitManager::UpdateAll(float elapsedTime)
{
    
    for (const auto& unitPtr : m_Units)
    {
        unitPtr->Update(elapsedTime);
    }

    // All commands are move commands right now (had a different system in mind and scrapped it)
    // If rewriting this, remember to call BuckerSortByY somewhere to sort draw order
    for (auto& [unitPtr,cmdQueue] : m_Queues)
    {
        if (cmdQueue.empty()) continue;
        auto& cmd = cmdQueue.front();

        cmd->Execute(unitPtr, elapsedTime);
        if (cmd->IsFinished())
        {
            cmdQueue.pop();
        }
        BucketSortByY(m_Units); // Probably should happen a bit less often
    }
}

void UnitManager::AddUnit(std::unique_ptr<Unit> unit) 
{
    unit->ChangeTeam(m_DefaultTeam);
    unit->LoadTextures();
    m_Units.push_back(std::move(unit));

}

void UnitManager::SelectClickedUnit()
{
    // Prolly just dissallow selecting various team units, or selecting more than one

    for (const auto& unitPtr : m_Units)
    {
        // If m_TilesTaken is rewriten to store what Unit/obstacle is in it, I can just check for tile polygon, not unit selectionbox
        // Makes it easier since otherwise I would have to update Unit selc boxes to match the tiles they stand on


        if (utils::IsPointInRect(m_LastMousePos,unitPtr->GetSelectionBox()))
        {
            auto unit = unitPtr.get();
            if (std::find(m_SelectedUnits.begin(),m_SelectedUnits.end(),unit) == m_SelectedUnits.end())
            {
                unit->PlayAnim("idle");
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

    // Make it not possible to select many units or check only first or smth
    // Doesn't work exaaaactly if there are many units selected (but it won't be possible to do that anyways)

    // Also set to false on right button down
    m_HoverAlly = false;
    m_HoverEnemy = false;
    m_HoverGround = false;

    for (const auto& unitPtr : m_Units)
    {
        if (m_SelectedUnits.size() == 0)return;

        // Hovering some unit
        if (utils::IsPointInRect(m_LastMousePos,unitPtr->GetSelectionBox()))
        {
            auto unit = unitPtr.get();
            if (m_SelectedUnits[0] == unit)
            {
                // Hovering over itself
                return;
            }
            if (m_SelectedUnits[0]->GetTeamID() != unitPtr->GetTeamID())
            {
                m_HoverEnemy = true;
            }
            else
            {
                m_HoverAlly = true;
            }
            return;
        }
    }
    if (m_Grid->GetHoverTileId(m_LastMousePos)!=-1)
    {
        m_HoverGround = true;
    }

}
void UnitManager::OnLeftButtonDown()
{
    // Later test for various scenarios, like clicking on ground or unit etc.
    SelectClickedUnit();

}
void UnitManager::OnRightButtonDown()
{
    using namespace std;
    // Since on right click I deselect a Unit
    m_HoverAlly = false;
    m_HoverEnemy = false;
    m_HoverGround = false;
    // Later test for various scenarios, like clicking on ground or unit etc. 

    // Add checks for things like range (attack range, move range)
    bool unitAttacked{false};
    std::vector<Unit*> toRemove;
    // Check if clicked on a Unit
    for (const auto& unitPtr : m_Units)
    {
        // Unit clicked found
        if (utils::IsPointInRect(m_LastMousePos,unitPtr->GetSelectionBox())) 
        {
            for (const auto& unitSelectedPtr : m_SelectedUnits)
            {
                if (unitSelectedPtr->GetTeamID() != unitPtr->GetTeamID())
                {
                    // Issue attack
                    // LATER
                    // LATER to check range/if can attack etc. ... For now it *just* attacks
                    unitSelectedPtr->Attack(unitPtr.get());

                    println("Unit with teamNr: {}, dealt: {} damage to unit with teamNr: {}, health of both: first one: {}, second one: {}"
                        ,unitSelectedPtr->GetTeamID()
                        ,unitSelectedPtr->GetStats().m_CurrentDamage
                        ,unitPtr->GetTeamID()
                        ,unitSelectedPtr->GetStats().m_CurrentHealth
                        ,unitPtr->GetStats().m_CurrentHealth);
                    toRemove.push_back(unitSelectedPtr);
                }
            }
            // Skip move command since clicked on unit
            unitAttacked = true;
        }
    }

    if (!unitAttacked)
    {
        // If clicked on ground
        int tileClicked = {m_Grid->GetHoverTileId(m_LastMousePos)};
        if (tileClicked <= -1)
        {
            // Clicked not in tiles
            return;
        }
        Point2f targetTileCenter = {m_Grid->GetTileCenter(tileClicked)};
        for (const auto& unitPtr : m_SelectedUnits)
        {
            IssueCommand(unitPtr,std::make_unique<MoveCommand>(targetTileCenter));
            toRemove.push_back(unitPtr);
            // FIX debug
            int tileOccupied{m_Grid->GetHoverTileId(unitPtr->GetTransform().position)};
            if (tileOccupied <= -1)
            {

            }
            else
            {
                m_TilesTaken[tileOccupied] = false;
            }
            //
        }

        // Debug
        m_TilesTaken[tileClicked] = true;
        //
    }
    for (auto* ptr : toRemove)
    {
        auto it = std::find(m_SelectedUnits.begin(),m_SelectedUnits.end(),ptr);
        if (it != m_SelectedUnits.end())
            m_SelectedUnits.erase(it);
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
    m_Units[unitIndex-1]->TeleportTo(location);
}

void UnitManager::DeSellectAll()
{
    for (auto& unitPtr : m_SelectedUnits)
    {
        unitPtr->PlayAnim("breathing");
    }
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

bool UnitManager::IsAnySelected()const
{
    return int(m_SelectedUnits.size());
}
bool UnitManager::GetHoverAlly() const
{
    return m_HoverAlly;
}
bool UnitManager::GetHoverEnemy() const
{
    return m_HoverEnemy;
}

void UnitManager::SetFrameTimeAll(float frameTimeTarget)
{
    for (auto& unit : m_Units)
    {
        unit->SetFrameTime(frameTimeTarget);
    }
}
Unit* UnitManager::GetUnit(int unitIndex,bool lastAdded) const
{
    if (unitIndex <= 0)
        return nullptr;
    if (lastAdded)
    {
        return m_Units.back().get();
    }
    return m_Units[unitIndex-1].get();
}

void UnitManager::PlaceOnGrid(int unitId, int tileId,bool randomFreeTile)
{
    if (unitId < 0 || unitId > m_Units.size())
        return;

    int tileTaken = tileId;
    if (tileId > m_Grid->GetTileAmount() || randomFreeTile)
    {
        int randomId{-1};
        while (true)
        {
            // FIX debug (poorly optimaized)
            randomId = {std::rand() % (m_Grid->GetTileAmount() - 1)};
            if (!m_TilesTaken[randomId])
            {
                break;
            }
        }
        //
        TeleportTo(unitId,m_Grid->GetTileCenter(randomId));

        tileTaken = randomId;
    }
    else
    {
        TeleportTo(unitId,m_Grid->GetTileCenter(tileId));
    }
    // FIX debug (whole fun is poorly written)
    m_TilesTaken[tileTaken] = true;
    //
}

bool UnitManager::GetHoverGround() const
{
    return m_HoverGround;
}

void UnitManager::BucketSortByY(std::vector<std::unique_ptr<Unit>>& units) 
{
    int heightDivision{10};
    int bucketCount = int(m_ScreenRect.height) / heightDivision + 1;

    std::vector<std::vector<std::unique_ptr<Unit>>> groundBuckets(bucketCount);

    for (auto& u : units)
    {
        int index = std::clamp(int(u->GetTransform().position.y / heightDivision)
            ,0
            ,bucketCount - 1);
   
            groundBuckets[index].push_back(std::move(u));
        
    }

    std::vector<std::unique_ptr<Unit>> sorted;
    units.clear();

    for (int i = bucketCount - 1; i >= 0; --i)
    {
        std::vector<std::unique_ptr<Unit>> movingUnits; // (So they are in front while on the same height)
        for (auto& u : groundBuckets[i])
        {
            if (!u->IsMoving())
            {
                units.push_back(std::move(u));
            }
            else
            {
                movingUnits.push_back(std::move(u));
            }
        }
        for (auto& movingU : movingUnits)
        {
            units.push_back(std::move(movingU));
        }
    }

}
