// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGridCell.h"
#include "AGridManager.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_API AGridManager : public AActor
{
    GENERATED_BODY()

public:
    // Method so that game state can create grids of a certain size
    UFUNCTION(BlueprintCallable, Category="Configure")
    void Initialize(int32 Width, int32 Height, float Size);

    AGridManager();
    AGridManager(int32 Width, int32 Height, float CellSize);

    // Grid settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Configuration")
    int32 GridWidth;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Configuration")
    int32 GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Configuration")
    float CellSize;

    // Storing the grid
    // I am storing it as a 1D array for simplicity's sake... no one has to know
    TArray<FGridCell> GridCells;

    // Get grid world location from an index
    UFUNCTION(BlueprintPure, Category="GridCell Getters")
    FVector GetWorldLocationOfGrid(int X, int Y) const;

    // Get the coordinates of the nearest cell to a certain location
    UFUNCTION(BlueprintPure, Category="GridCell Getters")
    FVector GetGridNearestToWorldLocation(float X, float Y, float Z) const;

    // Convert world location to grid cell
    UFUNCTION(BlueprintCallable, Category="GridCell Getters")
    FGridCell& GetFromLocation(float X, float Y);

    UFUNCTION(BlueprintCallable, Category="GridCell Getters")
    FGridCell& GetFromIndex(int X, int Y);

    UFUNCTION(BlueprintCallable, Category="GridCell Occupant")
    bool CanActorBePlacedAtIndex(int X, int Y, EGridOccupantType Type);

    UFUNCTION(BlueprintCallable, Category="GridCell Occupant")
    bool CanActorBePlacedAtLocation(float X, float Y, EGridOccupantType Type);

    bool CanActorBePlacedAtCell(FGridCell& Cell, EGridOccupantType Type);

    UFUNCTION(BlueprintCallable, Category="GridCell Occupant")
    bool OccupyCellAtIndex(int X, int Y, EGridOccupantType Type, TScriptInterface<IPlaceable> Actor);

    UFUNCTION(BlueprintCallable, Category="GridCell Occupant")
    bool UnoccupyCellAtIndex(int X, int Y, EGridOccupantType Type);

    UFUNCTION(BlueprintCallable, Category="GridCell Occupant")
    bool OccupyCellAtLocation(float X, float Y, EGridOccupantType Type, TScriptInterface<IPlaceable> Actor);

    UFUNCTION(BlueprintCallable, Category = "GridCell Occupant")
    bool UnoccupyCellAtLocation(float X, float Y, EGridOccupantType Type);

    UFUNCTION(BlueprintCallable, Category="GridCell Occupant")
    bool IsGridWalkable(float X, float Y);

    UFUNCTION(BlueprintCallable, Category = "GridCell Occupant")
    TScriptInterface<IPlaceable> GetOccupantAtIndex(int X, int Y, EGridOccupantType Type);

private:
    // initializes an empty grid
    void GenerateGrid();

    int convertCoordsToArrayIndex(int X, int Y) const;

protected:
    virtual void BeginPlay() override;
};
