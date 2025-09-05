#pragma once
#include <vector>
#include <memory>
#include "utils.h"
#include "Unit.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

class Command;
class UnitManager
{
public:
	using UnitPtr = std::unique_ptr<Unit>;
	using CommandPtr = std::unique_ptr<Command>;

	UnitManager();
	UnitManager(const UnitManager& other) = delete;
	UnitManager& operator=(const UnitManager& other) = delete;
	UnitManager(UnitManager&& other) = delete;
	UnitManager& operator=(UnitManager&& other) = delete;
	~UnitManager();

	void DrawAll()const;
	void UpdateAll(float elapsedTime);

	void TeleportAllTo(Point2f location);
	void TeleportTo(int unitIndex,Point2f location); // FIX unitIndex should be changed. Now you have to use valid numbers, debug

	void AddUnit(std::unique_ptr<Unit> unit);

	void SelectClickedUnit();
	void OnLeftButtonDown();
	void OnRightButtonDown();
	void OnMouseMotion(const Point2f& mousePos);

	void DeSellectAll(); // Debug FIX
	
	void IssueCommand(Unit* unit,CommandPtr command);
	void EnableQueuing();
	void DisableQueuing();

private:

	Point2f m_LastMousePos;
	bool m_QueuingEnabled;

	std::vector<Unit*> m_SelectedUnits;
	std::vector<std::unique_ptr<Unit>> m_Units;
	std::unordered_map<Unit*,std::queue<CommandPtr>> m_Queues;


	// Should be called every few/dozen or so frames (since units don't move THAT fast anyways)
	// Sorts Units by Y coordinate and UnitType (flying) in order to draw units from back to front

	std::vector<std::unique_ptr<Unit>> bucketSortByTypeAndY(std::vector<std::unique_ptr<Unit>>& units,int screenHeight)
	{
		int bucketCount = screenHeight / 5 + 1;

		std::vector<std::vector<std::unique_ptr<Unit>>> groundBuckets(bucketCount);
		std::vector<std::vector<std::unique_ptr<Unit>>> airBuckets(bucketCount);

		for (auto& u : units)
		{
			int index = int(u->GetTransform().position.y / 5);
			if (u->GetType() == UnitType::ground)
			{
				groundBuckets[index].push_back(std::move(u));
			}
			else
			{ 
				airBuckets[index].push_back(std::move(u));
			}
		}

		std::vector<std::unique_ptr<Unit>> sorted;
		for (int i = bucketCount - 1; i >= 0; --i)
		{
			for (auto& u : groundBuckets[i])
			{
				sorted.push_back(std::move(u));
			}
		}
		for (int i = bucketCount - 1; i >= 0; --i)
		{
			for (auto& u : airBuckets[i])
			{
				sorted.push_back(std::move(u));
			}
		}
		return sorted;
	}
};

