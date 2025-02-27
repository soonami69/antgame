// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridCell.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Y;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isWalkable;

	// constructor for an "invalid" gridCell
	FGridCell() : X(INT_MIN), Y(INT_MIN), isWalkable(true) {}

	// normal constructor
	FGridCell(int32 inX, int32 inY) : X(inX), Y(inY), isWalkable(true) {}

	// equality operator for mapping
	bool operator==(const FGridCell& other) const {
		return X == other.X && Y == other.Y;
	}
};

