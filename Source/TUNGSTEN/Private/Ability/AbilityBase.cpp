#include "Ability/AbilityBase.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAbilityBase::UAbilityBase()
{
	AbilityType = EAbilityType::Active;
	SlotType = EAbilitySlotType::Active;
	AbilityName = TEXT("DEBUG NO TEXT");
	AbilityDescription = TEXT("DEBUG NO TEXT");
}

void UAbilityBase::SetOwner(AActor* NewOwner)
{
	OwnerActor = NewOwner;
}

UWorld* UAbilityBase::GetWorld() const
{
	// Try to get world from owner actor first
	if (OwnerActor)
	{
		return OwnerActor->GetWorld();
	}

	// Fallback to default UObject implementation
	return Super::GetWorld();
}

bool UAbilityBase::TryActivate()
{
	// Check if we can execute
	if (!CanExecute())
	{
		return false;
	}

	if (!Commit())
	{
		return false;
	}
	// Execute the ability lifecycle
	Execute();
	return true;
}

void UAbilityBase::Execute_Implementation()
{
	
}

bool UAbilityBase::Commit_Implementation()
{
	return true;
}

bool UAbilityBase::CanExecute_Implementation() const
{
	return true;
}