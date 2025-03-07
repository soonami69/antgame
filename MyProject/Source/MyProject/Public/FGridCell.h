// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridCell.generated.h"

#define WALL 0
#define ANT 1
#define TRAP 2
#define ENEMY 3
#define TOTAL_TYPES 4

// say no to circular depedencies
class IPlaceable;

/**
 * Enum that represents the current types of objects that are on this grid
 */
UENUM(BlueprintType)
enum class EGridOccupantType : uint8 {
	None = 0 UMETA(DisplayName="None"),
	Wall = 1 << WALL UMETA(DisplayName = "Wall"),
	Ant = 1 << ANT UMETA(DisplayName = "Ant"),
	Trap = 1 << TRAP UMETA(DisplayName = "Trap"),
	// can add more here
};
ENUM_CLASS_FLAGS(EGridOccupantType);

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

	EGridOccupantType Occupants = EGridOccupantType::None;
	TArray<AActor*> OccupantActors; // to collect sugar/whatever when walked on, might change to another type if I ever get to it

	// constructor for an "invalid" gridCell
	FGridCell() : X(INT_MIN), Y(INT_MIN) {}

	// normal constructor
	FGridCell(int32 inX, int32 inY) : X(inX), Y(inY) {
		OccupantActors.SetNum(TOTAL_TYPES);
	}

	// equality operator for mapping
	bool operator==(const FGridCell& other) const {
		return X == other.X && Y == other.Y;
	}

	// Quick check to see if it is walkable (no ant or wall)
	bool IsWalkable() const
	{
		return (Occupants & (EGridOccupantType::Wall | EGridOccupantType::Ant)) == EGridOccupantType::None; // NONE HAS TO BE ZERO
	}

	bool HasOccupant(EGridOccupantType Type) const
	{
		return (Occupants & Type) != EGridOccupantType::None;
	}

	void SetOccupant(EGridOccupantType Type, AActor* Actor)
	{
		int32 Index = static_cast<int32>(Type);
		if (Index < OccupantActors.Num())
		{
			Occupants |= Type;
			OccupantActors[Index] = Actor;
		}
	}

	void RemoveOccupant(EGridOccupantType Type)
	{
		int32 Index = static_cast<int32>(Type);
		if (Index < OccupantActors.Num())
		{
			Occupants &= ~Type;
			OccupantActors[Index] = nullptr;
		}
	}

	AActor* GetOccupant(EGridOccupantType Type) const
	{
		int32 Index = static_cast<int32>(Type);
		return (Index < OccupantActors.Num()) ? OccupantActors[Index] : nullptr;
	}
};

// ChatGPT gave me this hash function
FORCEINLINE uint32 GetTypeHash(const FGridCell& Cell) {
	return HashCombineFast(GetTypeHash(Cell.X), GetTypeHash(Cell.Y));
}

