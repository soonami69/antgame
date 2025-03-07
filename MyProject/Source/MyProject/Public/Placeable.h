// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FGridCell.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Location Getters")
	FGridCell GetCurrentGrid() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Location Setters")
	void SetCurrentGridAndMove(FGridCell cell);

	// Function to check the type of object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Type Setters")
	EGridOccupantType GetOccupantType() const;
};
