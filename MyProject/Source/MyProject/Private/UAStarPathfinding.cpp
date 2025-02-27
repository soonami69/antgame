// Fill out your copyright notice in the Description page of Project Settings.

#include "UAStarPathfinding.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Algo/MinElement.h"


/**
* Constructors/Destructors for the UAPathfinding class
**/
UAStarPathfinding::UAStarPathfinding()
{
}

UAStarPathfinding::UAStarPathfinding(float cellSize) : cellSize(cellSize)
{
}

UAStarPathfinding::~UAStarPathfinding()
{
}

void UAStarPathfinding::SetGrid(AGridManager* gridManager)
{
    grid = gridManager;
}

// FIND PATH
TArray<FGridCell> UAStarPathfinding::FindPath(FGridCell start, FGridCell target, const TArray<FGridCell>& grid)
{
    // initialize all data structures required
    openList.Empty();
    closedSet.Empty();
    pathMap.Empty();

    // add the starting cell to the openList
    openList.Add(start);
    pathMap.Add(start, FPathfindingData(0, CalculateCostToTarget(start, target), FGridCell()));

    while (openList.Num() > 0)
    {
        // find the cell with the lowest total cost
        FGridCell* lowestCostCell = Algo::MinElement(openList, [&](const FGridCell& A, const FGridCell& B) {
            return pathMap[A].getCost() < pathMap[B].getCost();
            });

        if (!lowestCostCell) break;

        FGridCell currentCell = *lowestCostCell;

        // if we reach the goal, reconstruct the path
        if (currentCell == target) 
        {
            TArray<FGridCell> path;
            while (currentCell != start)
            {
                path.Add(currentCell);
                currentCell = pathMap[currentCell].parent;
            }
            //path.Insert(start, 0); // insert start at the beginning of the path (needed? Not so sure.)
            return path;
        }

        // move current cell to closed list
        openList.Remove(currentCell);
        closedSet.Add(currentCell);

        // get neighbors of the current cell
        TArray<FGridCell> neighbors = GetNeighbors(currentCell, grid);

        // loop through neighbors
        for (const FGridCell& neighbor : neighbors) 
        {
            if (closedSet.Contains(neighbor))
            {
                continue;
            }

            // calculate G cost for this neighbor
            int costSoFar = pathMap[currentCell].costSoFar + 1;
            bool bIsNewNode = !pathMap.Contains(neighbor);
            if (bIsNewNode || costSoFar <  pathMap[neighbor].costSoFar)
            {
                // update cost annd parent
                pathMap.Add(neighbor, FPathfindingData(costSoFar, CalculateCostToTarget(neighbor, target), currentCell));

                // add the neighbor to the openlist if it isn't already
                if (!openList.Contains(neighbor)) {
                    openList.Add(neighbor);
                }
            }
        }
    }
    
    // if no path was found, return an empty array
    return TArray<FGridCell>();
}

// Function to calculate distance from cell to target
int UAStarPathfinding::CalculateCostToTarget(FGridCell start, FGridCell target)
{
    // Manhattan distance heuristic
    return FMath::Abs(start.X - target.X) + FMath::Abs(start.Y - target.Y);
}

// Function to enumerate valid neighbors
TArray<FGridCell> UAStarPathfinding::GetNeighbors(const FGridCell& cell, const TArray<FGridCell>& grid)
{
    TArray<FGridCell> neighbors;

    // four possible neighbors
    TArray<FVector2D> directions = {
        FVector2D(cellSize, 0), FVector2D(-cellSize, 0), FVector2D(0, cellSize), FVector2D(0, -cellSize)
    };

    // check all directions
    for (const auto direction : directions) {
        FGridCell neighbor = getGridCellFromLocation(cell.X + direction.X, cell.Y + direction.Y);

        if (neighbor.isWalkable)
        {
            neighbors.Add(neighbor);
        }
    }

    return neighbors;
}

FGridCell UAStarPathfinding::getGridCellFromLocation(int X, int Y)
{
    return grid->GetFromLocation(X, Y);
}