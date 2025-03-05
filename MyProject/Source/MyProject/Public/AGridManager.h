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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Settings")
    int32 GridWidth;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Settings")
    int32 GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Settings")
    float CellSize;

    // Storing the grid
    // I am storing it as a 1D array for simplicity's sake... no one has to know
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    TArray<FGridCell> GridCells;

    // Get grid world location from an index
    UFUNCTION(BlueprintPure, Category="Grid")
    FVector GetWorldLocationOfGrid(int X, int Y) const;

    // Get the coordinates of the nearest cell to a certain location
    UFUNCTION(BlueprintPure, Category="Grid")
    FVector GetGridNearestToWorldLocation(float X, float Y, float Z) const;

    // Convert world location to grid cell
    UFUNCTION(BlueprintCallable, Category="Grid")
    FGridCell& GetFromLocation(float X, float Y);

    UFUNCTION(BlueprintCallable, Category="Grid")
    FGridCell& GetFromIndex(int X, int Y);

    UFUNCTION(BlueprintCallable, Category="Grid")
    bool OccupyCellAtIndex(int X, int Y);

    UFUNCTION(BlueprintCallable, Category="Grid")
    bool UnoccupyCellAtIndex(int X, int Y);

    UFUNCTION(BlueprintCallable, Category="Grid")
    bool OccupyCellAtLocation(float X, float Y);

    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool UnoccupyCellAtLocation(float X, float Y);

private:
    // initializes an empty grid
    void GenerateGrid();

    int convertCoordsToArrayIndex(int X, int Y) const;

protected:
    virtual void BeginPlay() override;
};
