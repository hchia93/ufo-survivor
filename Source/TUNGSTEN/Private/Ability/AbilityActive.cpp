#include "Ability/AbilityActive.h"

UAbilityActive::UAbilityActive()
{
	AbilityType = EAbilityType::Active;
}

bool UAbilityActive::CanExecute_Implementation() const
{
	return CooldownHandler && CooldownHandler->IsCooldownReady() && HasValidTarget();
}

bool UAbilityActive::HasValidTarget() const
{
	switch (AcquireTarget)
	{
	case EAbilityAcquireTarget::NoTargetRequired:
		return true;
	case EAbilityAcquireTarget::SelfOnly:
		return Targets.Num() > 0 && IsValid(Targets[0]) && Targets[0] == OwnerActor;
	case EAbilityAcquireTarget::SingleAllyOnly:
	case EAbilityAcquireTarget::SingleEnemyOnly:
		return Targets.Num() > 0 && IsValid(Targets[0]);
	case EAbilityAcquireTarget::MultipleAlly:
	case EAbilityAcquireTarget::MultipleEnemy:
		return Targets.Num() > 0;
	default:
		return false;
	}
}

