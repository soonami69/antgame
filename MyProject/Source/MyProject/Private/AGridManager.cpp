// Fill out your copyright notice in the Description page of Project Settings.


#include "AGridManager.h"
#include "DrawDebugHelpers.h"  // Include this for debug drawing

DEFINE_LOG_CATEGORY_STATIC(LogPathfinding, Log, All); // Define a log category

void AGridManager::Initialize(int32 Width, int32 Height, float Size)
{
    this->GridWidth = Width;
    this->GridHeight = Height;
    this->CellSize = Size;
    GenerateGrid();
}

AGridManager::AGridManager() : GridWidth(10), GridHeight(10), CellSize(100.f)
{
    // default constructor just cause
}

AGridManager::AGridManager(int32 Width, int32 Height, float CellSize) : GridWidth(Width), GridHeight(Height), CellSize(CellSize)
{
    this->GridWidth = Width;
    this->GridHeight = Height;
    this->CellSize = CellSize;
}

// GPT mode to draw debug box??
void AGridManager::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FlushPersistentDebugLines(GetWorld());

    float TotalWidth = GridWidth * CellSize;
    float TotalHeight = GridHeight * CellSize;
    FVector GridCenter = FVector(TotalWidth / 2.0f, TotalHeight / 2.0f, 0.0f);
    FVector GridExtent = FVector(TotalWidth / 2.0f, TotalHeight / 2.0f, 0.0f);
    if (GetWorld() && !GetWorld()->IsGameWorld())
    {
        DrawDebugBox(
            GetWorld(),
            GridCenter,
            GridExtent,
            FQuat::Identity,
            FColor::Green,
            true,
            -1.0f,
            0,
            2.0f
        );
    }
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

bool AGridManager::CanActorBePlacedAtIndex(int X, int Y, EGridOccupantType Type) {
    FGridCell& Cell = GetFromIndex(X, Y);
    return CanActorBePlacedAtCell(Cell, Type);
    
}

bool AGridManager::CanActorBePlacedAtLocation(float X, float Y, EGridOccupantType Type)
{
    FGridCell& Cell = GetFromLocation(X, Y);
    return CanActorBePlacedAtCell(Cell, Type);
}

bool AGridManager::CanActorBePlacedAtCell(FGridCell& Cell, EGridOccupantType Type)
{
    switch (Type) {
    case (EGridOccupantType::Wall):
        return Cell.IsEmpty();
    case (EGridOccupantType::Ant):
    case (EGridOccupantType::Enemy):
        return Cell.IsWalkable() && !Cell.HasOccupant(EGridOccupantType::Ant | EGridOccupantType::Enemy);
    case (EGridOccupantType::Interactible):
    case (EGridOccupantType::Trap):
        return Cell.IsWalkable() && !Cell.HasOccupant(EGridOccupantType::Trap | EGridOccupantType::Interactible);
    default:
        return Cell.IsWalkable() && !Cell.HasOccupant(Type);
    }
}

bool AGridManager::OccupyCellAtIndex(int X, int Y, EGridOccupantType Type, TScriptInterface<IPlaceable> Actor)
{
    FGridCell& Cell = GetFromIndex(X, Y);
    // Check if cell has wall/ant on it. If not, check if it has a type already matching itself (for traps?)
    if (!Cell.IsWalkable() || Cell.HasOccupant(Type))
    {
        return false;
    }
    Cell.SetOccupant(Type, Actor);
    return true;
}

bool AGridManager::UnoccupyCellAtIndex(int X, int Y, EGridOccupantType Type)
{ 
    FGridCell& Cell = GetFromIndex(X, Y);
    if (!Cell.HasOccupant(Type)) {
        return false;
    } else {
        Cell.RemoveOccupant(Type);
        return true;
    }
}

bool AGridManager::OccupyCellAtLocation(float X, float Y, EGridOccupantType Type, TScriptInterface<IPlaceable> Actor)
{
    FGridCell& Cell = GetFromLocation(X, Y);
    if (!Cell.IsWalkable() || Cell.HasOccupant(Type)) {
        return false;
    }
    UE_LOG(LogPathfinding, Log, TEXT("Setting Cell at %.3f %.3f to Unwalkable"), X, Y)
    Cell.SetOccupant(Type, Actor);
    return true;
}

bool AGridManager::UnoccupyCellAtLocation(float X, float Y, EGridOccupantType Type)
{
    FGridCell& Cell = GetFromLocation(X, Y);
    if (!Cell.HasOccupant(Type)) {
        return false;
    } else {
        Cell.RemoveOccupant(Type);
        return true;
    }
}

void AGridManager::GenerateGrid()
{
    GridCells.Empty();
    GridCells.Reserve(GridWidth * GridHeight);

    if (GridBlocks.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Populate possible bloocks in editor."));
        return;
    }

    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            GridCells.Add(FGridCell(X, Y)); // is this correct


            // choose random block to spawn
            int32 RandomIndex = FMath::RandRange(0, GridBlocks.Num() - 1);
            TSubclassOf<AActor> SelectedBlock = GridBlocks[RandomIndex];

            FVector SpawnLocation(X * CellSize + (CellSize / 2), Y * CellSize + (CellSize / 2), 0.0f);

            TArray<float> Rotations = { 0.f, 90.f, 180.f, 270.f };
            float RandomYaw = Rotations[FMath::RandRange(0, Rotations.Num() - 1)];
            FRotator SpawnRotation(0.f, RandomYaw, 0.f);
            // Spawn parameters
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            // Spawn the randomly chosen GridBlock
            GetWorld()->SpawnActor<AActor>(SelectedBlock, SpawnLocation, SpawnRotation, SpawnParams);

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

    UE_LOG(LogPathfinding, Log, TEXT("Invalid Index: %d, from %d %d"), Index, X, Y);

    static FGridCell InvalidCell; // Declare a persistent invalid cell
    return InvalidCell;
}

bool AGridManager::IsGridWalkable(float X, float Y)
{
    FGridCell& Cell = GetFromLocation(X, Y);
    return Cell.IsWalkable();
}

TScriptInterface<IPlaceable> AGridManager::GetOccupantAtIndex(int X, int Y, EGridOccupantType Type)
{
    return GetFromIndex(X, Y).GetOccupant(Type);
}

int AGridManager::convertCoordsToArrayIndex(int X, int Y) const {
    if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridHeight) {
        return -1;
    }
    return Y * GridWidth + X;
}
