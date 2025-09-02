#include "Ability/AbilityPassive.h"
#include "Engine/World.h"
#include "TimerManager.h"

UAbilityPassive::UAbilityPassive()
{
	AbilityType = EAbilityType::Passive;
	SlotType = EAbilitySlotType::Passive;
	bIsAlwaysActive = false;
	UpdateInterval = 0.1f;
}

bool UAbilityPassive::CanExecute_Implementation() const
{
	return true;
}

void UAbilityPassive::StartPassiveMonitoring()
{
	if (!OwnerActor || !GetWorld())
	{
		return;
	}

	// Start the passive update timer
	GetWorld()->GetTimerManager().SetTimer(PassiveUpdateTimer, this, &UAbilityPassive::Execute, UpdateInterval, true);
}

void UAbilityPassive::StopPassiveMonitoring()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PassiveUpdateTimer);
	}
}

