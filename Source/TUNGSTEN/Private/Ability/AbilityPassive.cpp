#include "Ability/AbilityPassive.h"

UAbilityPassive::UAbilityPassive()
{
	AbilityType = EAbilityType::Passive;
}

void UAbilityPassive::ActivatePassive()
{
	if (!bIsPassiveActive)
	{
		bIsPassiveActive = true;
		OnPassiveActivated();
	}
}

void UAbilityPassive::DeactivatePassive()
{
	if (bIsPassiveActive)
	{
		bIsPassiveActive = false;
		OnPassiveDeactivated();
	}
}
