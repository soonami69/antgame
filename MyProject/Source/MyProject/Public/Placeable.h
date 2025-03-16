#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FGridCell.h" // Full include of FGridCell
#include "Placeable.generated.h"

UINTERFACE(Blueprintable)
class MYPROJECT_API UPlaceable : public UInterface
{
	GENERATED_BODY()
};

/**
 * An interface for all walls/traps that occupy a spot on the grid
 */
class MYPROJECT_API IPlaceable
{
	GENERATED_BODY()
public:
	// Function to get current grid cell coordinates
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Placeable Parameters")
	FGridCell GetCurrentGrid() const; // Non-const reference for the return value

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Placeable Parameters")
	void SetCurrentGridAndMove(const FGridCell& cell); // Pass-by-reference

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Placeable Parameters")
	int GetHealth();

	// Function to check the type of object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Placeable Parameters")
	EGridOccupantType GetOccupantType() const;
};
