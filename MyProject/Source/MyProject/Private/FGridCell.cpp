#include "FGridCell.h"
#include "Placeable.h"

FGridCell::FGridCell() : X(INT_MIN), Y(INT_MIN) {}

FGridCell::FGridCell(int32 inX, int32 inY) : X(inX), Y(inY) {
	OccupantActors.SetNum(1<<TOTAL_TYPES);
}

bool FGridCell::operator==(const FGridCell& other) const {
	return X == other.X && Y == other.Y;
}

bool FGridCell::HasOccupant(EGridOccupantType Type) const
{
	return (Occupants & Type) != EGridOccupantType::None;
}

bool FGridCell::IsWalkable() const {
	return (Occupants & (EGridOccupantType::Wall | EGridOccupantType::Ant)) == EGridOccupantType::None;
}

bool FGridCell::IsWalkableByBreaking() const {
	if (IsWalkable()) {
		return true;
	}

	TScriptInterface<IPlaceable> Wall = GetOccupant(EGridOccupantType::Wall);
	return Wall && Wall->GetHealth() < 0;
}

int FGridCell::GetAdditionalCostToEnter() const {
	if (!HasOccupant(EGridOccupantType::Wall)) {
		return 0;
	}
	return GetOccupant(EGridOccupantType::Wall)->GetHealth();
}

void FGridCell::SetOccupant(EGridOccupantType Type, TScriptInterface<IPlaceable> Actor) {
	int32 Index = static_cast<int32>(Type);
	if (Index < OccupantActors.Num()) {
		Occupants |= Type;
		OccupantActors[Index] = Actor;
	}
}

void FGridCell::RemoveOccupant(EGridOccupantType Type) {
	int32 Index = static_cast<int32>(Type);
	if (Index < OccupantActors.Num()) {
		Occupants &= ~Type;
		OccupantActors[Index] = nullptr;
	}
}

TScriptInterface<IPlaceable> FGridCell::GetOccupant(EGridOccupantType Type) const {
	int32 Index = static_cast<int32>(Type);
	return (Index < OccupantActors.Num()) ? OccupantActors[Index] : nullptr;
}
