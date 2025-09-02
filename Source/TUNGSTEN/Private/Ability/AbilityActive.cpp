#include "Ability/AbilityActive.h"
#include "Ability/AbilityCooldownHandler.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAbilityActive::UAbilityActive()
{
	AbilityType = EAbilityType::Active;
	SlotType = EAbilitySlotType::Active;
	CooldownHandler = nullptr;
}

bool UAbilityActive::CanExecute_Implementation() const
{
	// First check base class conditions (owner and modules)
	if (!Super::CanExecute())
	{
		return false;
	}

	// Then check cooldown
	return CooldownHandler && CooldownHandler->IsCooldownReady();
}