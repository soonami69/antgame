// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAStarPathfinding.h"
#include "WallBreakerAStarPathfinding.generated.h"

/**
 * Pathfinding algorithm utility that factors in wall destruction. Will return a path that passes through destructible walls.
 * It is up to the actor that inherits this class to check for the wall ahead and destroy it.
 * This is so that multiple actors can destroy walls together before resuming their pathfinding.
 */
UCLASS()
class MYPROJECT_API UWallBreakerAStarPathfinding : public UAStarPathfinding
{
	GENERATED_BODY()
public:
	// Overload of the parent class, UAStarPathfinding::FindPath
	TArray<FGridCell> FindPath(FGridCell Start, FGridCell Target);
};
