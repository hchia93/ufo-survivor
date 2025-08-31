#include "Ability/AbilityInnate.h"

UAbilityInnate::UAbilityInnate()
{
	AbilityType = EAbilityType::Innate;
}

bool UAbilityInnate::CanExecute_Implementation() const
{
	return CooldownHandler && CooldownHandler->IsCooldownReady();
}
