// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridCell.h"
#include "Targetable.generated.h"

UINTERFACE(Blueprintable)
class MYPROJECT_API UTargetable : public UInterface
{
    GENERATED_BODY()
};

/**
 * An interface for all actors/resources that can be targeted
 */
class MYPROJECT_API ITargetable
{
    GENERATED_BODY()
public:
    // Function to get grid coordinates
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Location Getters")
    FGridCell GetOccupiedGrid() const;
};