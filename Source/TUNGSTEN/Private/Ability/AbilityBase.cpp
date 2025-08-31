#include "Ability/AbilityBase.h"
#include "Engine/World.h"

// UAbilityBase Implementation
UAbilityBase::UAbilityBase()
{
	AbilityType = EAbilityType::Active;
}

void UAbilityBase::SetWorldContext(UWorld* Context)
{
	WorldContext = Context;
}

void UAbilityBase::SetOwnerActor(AActor* Actor)
{
	OwnerActor = Actor;
}
