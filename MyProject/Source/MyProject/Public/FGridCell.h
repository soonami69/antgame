#pragma once

#include "CoreMinimal.h"
#include "FGridCell.generated.h"

#define WALL 0
#define ANT 1
#define TRAP 2
#define ENEMY 3
#define INTERACTIBLE 4
#define TOTAL_TYPES 5

// Forward declaration to avoid circular dependency
class IPlaceable;

UENUM(BlueprintType)
enum class EGridOccupantType : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	Wall = 1 << WALL UMETA(DisplayName = "Wall"),
	Ant = 1 << ANT UMETA(DisplayName = "Ant"),
	Trap = 1 << TRAP UMETA(DisplayName = "Trap"),
	Enemy = 1 << ENEMY UMETA(DisplayName = "Enemy"),
	Interactible = 1 << INTERACTIBLE UMETA(DisplayName = "Interactible"),
};

ENUM_CLASS_FLAGS(EGridOccupantType);

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()
private:
	int ConvertTypeToInteger(EGridOccupantType Type) const;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Y;

	EGridOccupantType Occupants = EGridOccupantType::None;
	TArray<TScriptInterface<IPlaceable>> OccupantActors;

	FGridCell();
	FGridCell(int32 inX, int32 inY);

	bool operator==(const FGridCell& other) const;

	bool HasOccupant(EGridOccupantType Type) const;

	bool IsEmpty() const;

	// Function declarations (implementation moved to cpp)
	bool IsWalkable() const;

	bool IsWalkableByBreaking() const;

	int GetAdditionalCostToEnter() const;

	void SetOccupant(EGridOccupantType Type, TScriptInterface<IPlaceable> Actor);
	void RemoveOccupant(EGridOccupantType Type);
	TScriptInterface<IPlaceable> GetOccupant(EGridOccupantType Type) const;
};

FORCEINLINE uint32 GetTypeHash(const FGridCell& Cell) {
	return HashCombineFast(GetTypeHash(Cell.X), GetTypeHash(Cell.Y));
}
