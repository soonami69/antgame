// Fill out your copyright notice in the Description page of Project Settings.


#include "AGridManager.h"

AGridManager::AGridManager()
{
    
}

AGridManager::AGridManager(int32 Width, int32 Height, float CellSize) : GridWidth(Width), GridHeight(Height), CellSize(CellSize)
{
    
}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();
    GenerateGrid();
}

void AGridManager::GenerateGrid()
{
    GridCells.Empty();

    for (int32 X = 0; X < GridWidth; X++)
    {
        for (int32 Y = 0; Y < GridHeight; Y++)
        {
            GridCells.Add(FGridCell(X, Y)); // is this correct
        }
    }
}

FGridCell AGridManager::GetFromIndex(int X, int Y)
{
    int Index = convertCoordsToArrayIndex(X, Y);
    if (Index != -1 && GridCells.IsValidIndex(Index))
    {
        return GridCells[Index];
    }
    return FGridCell();
}

FGridCell AGridManager::GetFromLocation(float X, float Y) const 
{
    float halfTile = CellSize / 2;

    // Get closest index
    // make center as tile location, then divide by tileSize and round off
    int xIndex = FMath::RoundToInt32((X - halfTile) / CellSize);
    int yIndex = FMath::RoundToInt32((Y - halfTile) / CellSize);

    // return tile
    return Get(xIndex, yIndex);
}

int AGridManager::convertCoordsToArrayIndex(int X, int Y) {
    if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridHeight) {
        return -1;
    }
    return Y * GridWidth + X;
}
