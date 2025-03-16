// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBreakerAStarPathfinding.h"

DEFINE_LOG_CATEGORY_STATIC(LogAStar, Log, All)

TArray<FGridCell> UWallBreakerAStarPathfinding::FindPath(FGridCell Start, FGridCell Target)
{
    UE_LOG(LogAStar, Log, TEXT("Starting A* wallbreaker pathfinding from (%d, %d) to (%d, %d)"), Start.X, Start.Y, Target.X, Target.Y);

    // check that grid exists first
    if (!grid)
    {
        UE_LOG(LogAStar, Error, TEXT("GridManager is NULL! Ensure SetGrid() is called before FindPath()."));
        return TArray<FGridCell>();
    }

    // initialize all data structures required
    openList.Empty();
    closedSet.Empty();
    pathMap.Empty();
    openList.Add(Start);
    pathMap.Add(Start, FPathfindingData(0, CalculateCostToTarget(Start, Target), FGridCell()));

    while (openList.Num() > 0)
    {
        FGridCell* LowestCostCell = Algo::MinElement(openList, [&](const FGridCell& A, const FGridCell& B) {
            return pathMap[A].getCost() < pathMap[B].getCost();
            });

        if (!LowestCostCell) break;

        FGridCell CurrentCell = *LowestCostCell;
        UE_LOG(LogAStar, Log, TEXT("Processing node: (%d, %d) with cost %d"), CurrentCell.X, CurrentCell.Y, pathMap[CurrentCell].getCost());

        if (CurrentCell == Target)
        {
            UE_LOG(LogAStar, Log, TEXT("Target reached, reconstructing path..."));
            TArray<FGridCell> Path = TArray<FGridCell>();
            while (CurrentCell != Start)
            {
                Path.Add(CurrentCell);
                CurrentCell = pathMap[CurrentCell].parent;
            }
            Algo::Reverse(Path);
            UE_LOG(LogAStar, Log, TEXT("Path found with %d nodes."), Path.Num());
            return Path;
        }

        openList.Remove(CurrentCell);
        closedSet.Add(CurrentCell);

        TArray<FGridCell> Neighbors = GetNeighbors(CurrentCell);

        for (const FGridCell& Neighbor: Neighbors)
        {
            if(closedSet.Contains(Neighbor))
            {
                UE_LOG(LogAStar, Log, TEXT("Skipping already processed neighbor at (%d, %d)"), Neighbor.X, Neighbor.Y);
                continue;
            }

            // redundancy check for an invalid neighbor cell
            if (Neighbor.X == INT_MIN) continue;

            int CostSoFar = pathMap[CurrentCell].costSoFar + Neighbor.GetAdditionalCostToEnter();

            bool bIsNewNode = !pathMap.Contains(Neighbor);
            // if it is a new node, we add it to the openlist, if not we check the existing cost.
            // We can just compare cost to get there because we can always safely assume the cost to destination will always be the same for the same node.
            if (bIsNewNode || CostSoFar < pathMap[Neighbor].costSoFar)
            {
                pathMap.Add(Neighbor, FPathfindingData(CostSoFar, CalculateCostToTarget(Neighbor, Target), CurrentCell));

                if (!openList.Contains(Neighbor))
                {
                    openList.Add(Neighbor);
                    UE_LOG(LogAStar, Log, TEXT("Added Neighbor (%d, %d) with cost %d to open list."), Neighbor.X, Neighbor.Y, pathMap[Neighbor].getCost());
                }
            }
        }
    }

    UE_LOG(LogAStar, Log, TEXT("No path found from (%d, %d) to (%d, %d)."),
        Start.X, Start.Y, Target.X, Target.Y);

    return TArray<FGridCell>();
}

TArray<FGridCell> UWallBreakerAStarPathfinding::GetNeighbors(const FGridCell& Cell)
{
    TArray<FGridCell> Neighbors;

    TArray<FVector2D> Directions = {
        FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1)
    };

    // enumerate neighbors and check if they are walkable WITH breaking
    for (const auto& Direction : Directions)
    {
        FGridCell& Neighbor = grid->GetFromIndex(Cell.X + Direction.X, Cell.Y + Direction.Y);

        if (Neighbor.X == INT_MIN)
        {
            continue;
        }
        if (Neighbor.IsWalkableByBreaking())
        {
            Neighbors.Add(Neighbor);
            UE_LOG(LogAStar, Log, TEXT("Neighbor (%d, %d) is walkable by breaking."), Neighbor.X, Neighbor.Y);
        } else {
            UE_LOG(LogAStar, Log, TEXT("Neighbor (%d, %d) is NOT walkable by breaking."), Neighbor.X, Neighbor.Y);
        }
    }

    return Neighbors;
}