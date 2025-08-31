#include "Ability/AbilityComponent.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityInnate.h"
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
				NewAbility->SetOwnerActor(GetOwner());
				NewAbility->SetWorldContext(GetWorld());
				RuntimeAbilities.Add(NewAbility);
			}
		}
	}
}

void UAbilityComponent::ExecuteAllAbilities()
{
	for (UAbilityBase* Ability : RuntimeAbilities)
	{
		if (Ability)
		{
			// Check if it's an innate ability that can execute
			if (UAbilityInnate* InnateAbility = Cast<UAbilityInnate>(Ability))
			{
				if (InnateAbility->CanExecute())
				{
					InnateAbility->Execute();
				}
			}
			// Check if it's an active ability that can execute
			else if (UAbilityActive* ActiveAbility = Cast<UAbilityActive>(Ability))
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
