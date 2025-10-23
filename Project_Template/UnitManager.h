#pragma once
#include <vector>
#include <memory>
#include "utils.h"
#include "Unit.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

class Grid;
class Command;
struct SoundPack;
class UnitManager
{
public:
	using UnitPtr = std::unique_ptr<Unit>;
	using CommandPtr = std::unique_ptr<Command>;

	UnitManager(Rectf screenRect);
	UnitManager(const UnitManager& other) = delete;
	UnitManager& operator=(const UnitManager& other) = delete;
	UnitManager(UnitManager&& other) = delete;
	UnitManager& operator=(UnitManager&& other) = delete;
	~UnitManager();

	void DrawAll()const;
	void DrawGrid()const;
	void UpdateAll(float elapsedTime);

	void TeleportAllTo(Point2f location);
	void TeleportTo(int unitIndex,Point2f location); // FIX unitIndex should be changed. Now you have to know/use valid numbers, debug
	void PlaceOnGrid(int unitId, int tileId,bool randomFreeTile = false);

	void AddUnit(std::unique_ptr<Unit> unit);

	void SelectClickedUnit();
	void OnLeftButtonDown();
	void OnRightButtonDown();
	void OnMouseMotion(const Point2f& mousePos);

	void DeSellectAll(); // Debug
	
	void IssueCommand(Unit* unit,CommandPtr command);
	void EnableQueuing();
	void DisableQueuing();

	void SetDefaultTeam(int teamID);
	void ChangeUnitTeam(int unitIndex, int newTeamID);
	int GetUnitCount() const;
	void ScaleAllUnits(float x, float y);
	std::vector<int> GetReachableTilesId(Unit* unitToMove);
	void SetSoundPack(const SoundPack* unitSoundPack);

	// Probably could be better:
	bool IsAnySelected() const;
	bool GetHoverAlly() const;
	bool GetHoverEnemy() const;
	bool GetHoverGround() const;
	//

	bool IsTileInRange(int targetTileId,int currentTileId,int range);
	Unit* GetUnit(int unitIndex,bool lastAdded = false) const;

	void SetFrameTimeAll(float frameTimeTarget);

private:

	Point2f m_LastMousePos;
	bool m_QueuingEnabled;
	Rectf m_ScreenRect;

	std::vector<Unit*> m_SelectedUnits;
	std::vector<std::unique_ptr<Unit>> m_Units;
	std::unordered_map<Unit*,std::queue<CommandPtr>> m_Queues;
	const SoundPack* m_UnitSounds;
	int m_DefaultTeam;

	bool m_HoverEnemy;
	bool m_HoverAlly;
	bool m_HoverGround;

	int m_TilesPerRow;
	int m_TilesPerColumn;
	std::unique_ptr<Grid> m_Grid;
	std::vector<Unit*> m_ObjectsInTiles; 

	// Can sort based on tiles instead of Y
	// Called after a unit moves
	// Sorts Units by Y coordinate in order to draw units from back to front

	void BucketSortByY(std::vector<std::unique_ptr<Unit>>& units);
	
};

