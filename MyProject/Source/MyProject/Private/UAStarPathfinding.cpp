#include "UAStarPathfinding.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Algo/MinElement.h"

DEFINE_LOG_CATEGORY_STATIC(LogAStar, Log, All); // Define a log category

UAStarPathfinding::~UAStarPathfinding() {
}

UAStarPathfinding* UAStarPathfinding::CreateAStarPathfinding(float cellSize) {
    UAStarPathfinding* PathfindingInstance = NewObject<UAStarPathfinding>();
    PathfindingInstance->cellSize = cellSize;
    return PathfindingInstance;
}

void UAStarPathfinding::SetGrid(AGridManager* gridManager) {
    grid = gridManager;
}

// FIND PATH
TArray<FGridCell> UAStarPathfinding::FindPath(FGridCell start, FGridCell target) {
    UE_LOG(LogAStar, Log, TEXT("Starting A* pathfinding from (%d, %d) to (%d, %d)"), start.X, start.Y, target.X, target.Y);

    if (!grid) {
        UE_LOG(LogAStar, Error, TEXT("GridManager is NULL! Ensure SetGrid() is called before FindPath()."));
        return TArray<FGridCell>();
    }

    // initialize all data structures required
    openList.Empty();
    closedSet.Empty();
    pathMap.Empty();

    openList.Add(start);
    pathMap.Add(start, FPathfindingData(0, CalculateCostToTarget(start, target), FGridCell()));

    while (openList.Num() > 0) {
        FGridCell* lowestCostCell = Algo::MinElement(openList, [&](const FGridCell& A, const FGridCell& B) {
            return pathMap[A].getCost() < pathMap[B].getCost();
            });

        if (!lowestCostCell) break;

        FGridCell CurrentCell = *lowestCostCell;
        UE_LOG(LogAStar, Log, TEXT("Processing node: (%d, %d) with cost %d"), CurrentCell.X, CurrentCell.Y, pathMap[CurrentCell].getCost());

        if (CurrentCell == target) {
            UE_LOG(LogAStar, Log, TEXT("Target reached, reconstructing path..."));
            TArray<FGridCell> path;
            while (CurrentCell != start) {
                path.Add(CurrentCell);
                CurrentCell = pathMap[CurrentCell].parent;
            }
            Algo::Reverse(path);
            UE_LOG(LogAStar, Log, TEXT("Path found with %d nodes."), path.Num());
            return path;
        }

        openList.Remove(CurrentCell);
        closedSet.Add(CurrentCell);

        TArray<FGridCell> Neighbors = GetNeighbors(CurrentCell);

        for (const FGridCell& Neighbor : Neighbors) {
            if (closedSet.Contains(Neighbor)) {
                UE_LOG(LogAStar, Log, TEXT("Skipping neighbor (%d, %d) as it's already processed."), Neighbor.X, Neighbor.Y);
                continue;
            }
            if (Neighbor.X == INT_MIN) continue;

            int CostSoFar = pathMap[CurrentCell].costSoFar + 1;
            bool bIsNewNode = !pathMap.Contains(Neighbor);
            if (bIsNewNode || CostSoFar < pathMap[Neighbor].costSoFar) {
                pathMap.Add(Neighbor, FPathfindingData(CostSoFar, CalculateCostToTarget(Neighbor, target), CurrentCell));

                if (!openList.Contains(Neighbor)) {
                    openList.Add(Neighbor);
                    UE_LOG(LogAStar, Log, TEXT("Added neighbor (%d, %d) with cost %d to open list."), Neighbor.X, Neighbor.Y, pathMap[Neighbor].getCost());
                }
            }
        }
    }

    UE_LOG(LogAStar, Warning, TEXT("No path found from (%d, %d) to (%d, %d)."), start.X, start.Y, target.X, target.Y);
    return TArray<FGridCell>();
}

int UAStarPathfinding::CalculateCostToTarget(FGridCell start, FGridCell target) {
    int cost = FMath::Abs(start.X - target.X) + FMath::Abs(start.Y - target.Y);
    UE_LOG(LogAStar, Log, TEXT("Calculated cost from (%d, %d) to (%d, %d) = %d"), start.X, start.Y, target.X, target.Y, cost);
    return cost;
}

TArray<FGridCell> UAStarPathfinding::GetNeighbors(const FGridCell& cell) {
    UE_LOG(LogAStar, Log, TEXT("Checking Neighbors of %d %d"), cell.X, cell.Y)
    TArray<FGridCell> Neighbors;

    TArray<FVector2D> Directions = {
        FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1)
    };

    for (const auto& Direction : Directions) {
        FGridCell& Neighbor = grid->GetFromIndex(cell.X + Direction.X, cell.Y + Direction.Y);
        if (Neighbor.X == INT_MIN)
        {
            continue;
        }
        if (Neighbor.IsWalkable()) {
            Neighbors.Add(Neighbor);
            UE_LOG(LogAStar, Log, TEXT("Neighbor (%d, %d) is walkable."), Neighbor.X, Neighbor.Y);
        } else {
            UE_LOG(LogAStar, Log, TEXT("Neighbor (%d, %d) is NOT walkable."), Neighbor.X, Neighbor.Y);
        }
    }

    return Neighbors;
}