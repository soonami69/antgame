// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridCell.generated.h"

// say no to circular depedencies
class IPlaceable;
/**
 * Struct that represents a grid cell.
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TScriptInterface<IPlaceable> Placeable;

	// constructor for an "invalid" gridCell
	FGridCell() : X(INT_MIN), Y(INT_MIN), isWalkable(true), Placeable(nullptr) {}

	// normal constructor
	FGridCell(int32 inX, int32 inY) : X(inX), Y(inY), isWalkable(true), Placeable(nullptr) {}

	// equality operator for mapping
	bool operator==(const FGridCell& other) const {
		return X == other.X && Y == other.Y;
	}

	// function to check if there is a valid placeable object
	bool HasPlaceable() const
	{
		return Placeable.GetObject() != nullptr;
	}
};

// ChatGPT gave me this hash function
FORCEINLINE uint32 GetTypeHash(const FGridCell& Cell) {
	return HashCombineFast(GetTypeHash(Cell.X), GetTypeHash(Cell.Y));
}

