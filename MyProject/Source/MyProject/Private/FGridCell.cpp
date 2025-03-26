#include "FGridCell.h"
#include "Placeable.h"

FGridCell::FGridCell() : X(INT_MIN), Y(INT_MIN) {}

FGridCell::FGridCell(int32 inX, int32 inY) : X(inX), Y(inY) {
	OccupantActors.SetNum(TOTAL_TYPES);
}

bool FGridCell::operator==(const FGridCell& other) const {
	return X == other.X && Y == other.Y;
}

int FGridCell::ConvertTypeToInteger(EGridOccupantType Type) const
{
	switch (Type)
	{
	case (EGridOccupantType::Wall):
		return WALL;
	case (EGridOccupantType::Ant):
		return ANT;
	case (EGridOccupantType::Trap):
		return TRAP;
	case (EGridOccupantType::Enemy):
		return ENEMY;
	case (EGridOccupantType::Interactible):
		return INTERACTIBLE;
	default:
		return INT_MAX;
	}
}

bool FGridCell::HasOccupant(EGridOccupantType Type) const
{
	return (Occupants & Type) != EGridOccupantType::None;
}

bool FGridCell::IsWalkable() const {
	return (Occupants & (EGridOccupantType::Wall)) == EGridOccupantType::None;
}

bool FGridCell::IsWalkableByBreaking() const {
	if (IsWalkable()) {
		return true;
	}

	TScriptInterface<IPlaceable> Wall = GetOccupant(EGridOccupantType::Wall);
	return Wall && Wall->GetHealth() < 0;
}

bool FGridCell::IsEmpty() const {
	return Occupants == EGridOccupantType::None;
}

int FGridCell::GetAdditionalCostToEnter() const {
	if (!HasOccupant(EGridOccupantType::Wall)) {
		return 0;
	}
	return GetOccupant(EGridOccupantType::Wall)->GetHealth();
}

void FGridCell::SetOccupant(EGridOccupantType Type, TScriptInterface<IPlaceable> Actor) {
	int Index = ConvertTypeToInteger(Type);
	if (Index < OccupantActors.Num()) {
		Occupants |= Type;
		OccupantActors[Index] = Actor;
	}
}

void FGridCell::RemoveOccupant(EGridOccupantType Type) {
	int Index = ConvertTypeToInteger(Type);
	if (Index < OccupantActors.Num()) {
		Occupants &= ~Type;
		OccupantActors[Index] = nullptr;
	}
}

TScriptInterface<IPlaceable> FGridCell::GetOccupant(EGridOccupantType Type) const {
	int Index = ConvertTypeToInteger(Type);
	return (Index < OccupantActors.Num()) ? OccupantActors[Index] : nullptr;
}
