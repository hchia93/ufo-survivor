#include "Ability/AbilityComponent.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityActive.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeAbilities();
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAbilityComponent::InitializeAbilities()
{
	for (TSubclassOf<UAbilityBase> AbilityClass : AbilityClasses)
	{
		if (AbilityClass)
		{
			UAbilityBase* NewAbility = NewObject<UAbilityBase>(this, AbilityClass);
			if (NewAbility)
			{
				NewAbility->SetOwner(GetOwner());
				RuntimeAbilities.Add(NewAbility);
			}
		}
	}
}

void UAbilityComponent::TryExecuteAllAbilities()
{
	for (UAbilityBase* Ability : RuntimeAbilities)
	{
		if (Ability)
		{
			// Execute active abilities if ready
			if (UAbilityActive* ActiveAbility = Cast<UAbilityActive>(Ability))
			{
				if (ActiveAbility->CanExecute())
				{
					ActiveAbility->Execute();
				}
			}
			// Passive abilities don't need execution
		}
	}
}
