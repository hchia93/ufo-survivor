#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Active,
	Passive
};

UENUM(BlueprintType)
enum class EAbilitySlotType : uint8
{
	Active,
	Passive,
	Innate
};

UENUM(BlueprintType)
enum class EAbilityAddRemoveResult : uint8
{
	Success,
	UnknownAbility,
	InvalidIndex,
	NoEmptySlot,
	IncompatibleType
};


