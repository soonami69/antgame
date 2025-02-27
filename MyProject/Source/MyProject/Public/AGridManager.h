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
    AGridManager();
    AGridManager(int32 Width, int32 Height, float CellSize);

    // Grid settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Settings")
    int32 GridWidth = 10;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Settings")
    int32 GridHeight = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid Settings")
    float CellSize = 100.f;

    // Storing the grid
    // I am storing it as a 1D array for simplicity's sake... no one has to know
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    TArray<FGridCell> GridCells;

    // Convert world location to grid cell
    UFUNCTION(BlueprintCallable, Category="Grid")
    FGridCell GetFromLocation(float X, float Y) const;

    UFUNCTION(BlueprintCallable, Category="Grid")
    FGridCell GetFromIndex(int X, int Y) const;

private:
    // initializes an empty grid
    void GenerateGrid();

    int convertCoordsToArrayIndex(int X, int Y);

protected:
    virtual void BeginPlay() override;
};
