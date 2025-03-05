// Fill out your copyright notice in the Description page of Project Settings.


#include "AGridManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogPathfinding, Log, All); // Define a log category

void AGridManager::Initialize(int32 Width, int32 Height, float Size)
{
    this->GridWidth = Width;
    this->GridHeight = Height;
    this->CellSize = Size;
    GenerateGrid();
}

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

FVector AGridManager::GetWorldLocationOfGrid(int X, int Y) const
{
    return FVector(X * CellSize + (CellSize / 2), Y * CellSize + (CellSize / 2), 0.f);
}

FVector AGridManager::GetGridNearestToWorldLocation(float X, float Y, float Z) const
{
    // p sure this is poor OOP
    int indexX = FMath::RoundToInt((X - (CellSize / 2)) / CellSize);
    float newX = indexX * CellSize + (CellSize / 2);
    int indexY = FMath::RoundToInt((Y - (CellSize / 2)) / CellSize);
    float newY = indexY * CellSize + (CellSize / 2);

    return FVector(newX, newY, 0.f);
}

bool AGridManager::OccupyCellAtIndex(int X, int Y)
{
    FGridCell cell = GetFromIndex(X, Y);
    if (!cell.isWalkable) {
        return false;
    } else {
        cell.isWalkable = false;
        return true;
    }
}

bool AGridManager::UnoccupyCellAtIndex(int X, int Y)
{
    FGridCell cell = GetFromIndex(X, Y);
    if (cell.isWalkable) {
        return false;
    } else {
        cell.isWalkable = true;
        return true;
    }
}

bool AGridManager::OccupyCellAtLocation(float X, float Y)
{
    FGridCell& cell = GetFromLocation(X, Y);
    if (!cell.isWalkable)
    {
        return false;
    } else {
        cell.isWalkable = false;
        return true;
    }
}

bool AGridManager::UnoccupyCellAtLocation(float X, float Y)
{
    FGridCell& cell = GetFromLocation(X, Y);
    if (cell.isWalkable) {
        return false;
    } else {
        cell.isWalkable = true;
        return true;
    }
}

void AGridManager::GenerateGrid()
{
    GridCells = TArray<FGridCell>();

    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            GridCells.Add(FGridCell(X, Y)); // is this correct
        }
    }
}

FGridCell& AGridManager::GetFromLocation(float X, float Y) {
    float halfTile = CellSize / 2;

    // Get closest index
    // make center as tile location, then divide by tileSize and round off
    int xIndex = FMath::RoundToInt32((X - halfTile) / CellSize);
    int yIndex = FMath::RoundToInt32((Y - halfTile) / CellSize);

    // return tile
    return GetFromIndex(xIndex, yIndex);
}

FGridCell& AGridManager::GetFromIndex(int X, int Y) {
    int Index = convertCoordsToArrayIndex(X, Y);
    if (Index != -1 && GridCells.IsValidIndex(Index)) {
        return GridCells[Index];
    }

    UE_LOG(LogPathfinding, Warning, TEXT("Invalid Index: %d, from %d %d"), Index, X, Y);

    static FGridCell InvalidCell; // Declare a persistent invalid cell
    return InvalidCell;
}

int AGridManager::convertCoordsToArrayIndex(int X, int Y) const {
    if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridHeight) {
        return -1;
    }
    return Y * GridWidth + X;
}
