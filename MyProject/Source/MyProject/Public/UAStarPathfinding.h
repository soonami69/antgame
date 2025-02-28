// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGridManager.h"
#include "FGridCell.h"
#include "UAStarPathfinding.generated.h"
/**
 * The struct for pathfinding data based of cliffany's gridcell struct.
 */


/**
 * Pathfinding Data struct that contains the cell, the cost, and the parent of that node
**/
USTRUCT(BlueprintType)
struct FPathfindingData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int costSoFar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int costToGoal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGridCell parent;

	FPathfindingData(int costSoFar, int costToGoal, FGridCell parent) : costSoFar(costSoFar), costToGoal(costToGoal),
		parent(parent) {}

	FPathfindingData() : costSoFar(0), costToGoal(0), parent(FGridCell()) {}

	int getCost() const { return costSoFar + costToGoal; }
};

UCLASS(Blueprintable)
class MYPROJECT_API UAStarPathfinding : public UObject
{

	GENERATED_BODY()

public:
	UAStarPathfinding(float cellSize) : cellSize(cellSize) {}
	UAStarPathfinding() : cellSize(100.f) {}


	// function to find path from start to goal
	UFUNCTION(BlueprintCallable, Category="Pathfinding")
	TArray<FGridCell> FindPath(FGridCell start, FGridCell target);

	~UAStarPathfinding();

	UFUNCTION(BlueprintCallable, Category="Grid Settings")
	void SetGrid(AGridManager* gridManager);

	UFUNCTION(BlueprintCallable, Category="Pathfinding")
	static UAStarPathfinding* CreateAStarPathfinding(float cellSize);

	// cell sizes
	UPROPERTY(BlueprintReadWrite)
	float cellSize;
private:

	UPROPERTY()
	// GridManager instance
	AGridManager* grid;

	// PQ of cells to explore
	TArray<FGridCell> openList;

	// Set of processed nodes
	TSet<FGridCell> closedSet;

	// Maps for tracking lowest costs
	TMap<FGridCell, FPathfindingData> pathMap;

	// Function to get neighbouring cells
	TArray<FGridCell> GetNeighbors(const FGridCell& cell);

	// Cost to Goal (Manhattan Distance)
	int CalculateCostToTarget(FGridCell start, FGridCell target);
};
