#include "Ability/PlayerAbilityComponent.h"
#include "Ability/AbilityActive.h"
#include "Ability/AbilityPassive.h"
#include "Ability/AbilityBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

UPlayerAbilityComponent::UPlayerAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize arrays
	ActiveAbilitySlots.SetNum(MAX_ACTIVE_SLOTS);
	PassiveAbilitySlots.SetNum(MAX_PASSIVE_SLOTS);
}

void UPlayerAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilities();
}

void UPlayerAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check and trigger abilities automatically
	CheckAndTriggerAbilities();
}

void UPlayerAbilityComponent::InitializeAbilities()
{
	// Innate abilities are assigned via Blueprint; no code-side creation
	SetupAbilitySlots();
}

void UPlayerAbilityComponent::SetupInnateAbilities()
{
	// Deprecated: no-op; kept for backward compatibility if called
}

void UPlayerAbilityComponent::SetupAbilitySlots()
{
	// Initialize all slots as empty
	for (int32 i = 0; i < MAX_ACTIVE_SLOTS; ++i)
	{
		ActiveAbilitySlots[i] = nullptr;
	}

	for (int32 i = 0; i < MAX_PASSIVE_SLOTS; ++i)
	{
		PassiveAbilitySlots[i] = nullptr;
	}
}

EAbilityAddRemoveResult UPlayerAbilityComponent::AddActiveAbility(UAbilityBase* Ability)
{
	if (!Ability)
	{
		return EAbilityAddRemoveResult::UnknownAbility;
	}

	// Validate ability type
	if (Ability->AbilityType != EAbilityType::Active)
	{
		return EAbilityAddRemoveResult::IncompatibleType;
	}

	// Find first empty slot
	for (int32 i = 0; i < MAX_ACTIVE_SLOTS; ++i)
	{
		if (!ActiveAbilitySlots[i])
		{
			Ability->SetOwner(GetOwner());
			Ability->SlotType = EAbilitySlotType::Active;
			ActiveAbilitySlots[i] = Ability;
			return EAbilityAddRemoveResult::Success;
		}
	}

	return EAbilityAddRemoveResult::NoEmptySlot;
}

EAbilityAddRemoveResult UPlayerAbilityComponent::AddPassiveAbility(UAbilityBase* Ability)
{
	if (!Ability)
	{
		return EAbilityAddRemoveResult::UnknownAbility;
	}

	// Validate ability type
	if (Ability->AbilityType != EAbilityType::Passive)
	{
		return EAbilityAddRemoveResult::IncompatibleType;
	}

	// Find first empty slot
	for (int32 i = 0; i < MAX_PASSIVE_SLOTS; ++i)
	{
		if (!PassiveAbilitySlots[i])
		{
			Ability->SetOwner(GetOwner());
			Ability->SlotType = EAbilitySlotType::Passive;
			PassiveAbilitySlots[i] = Ability;
			return EAbilityAddRemoveResult::Success;
		}
	}

	return EAbilityAddRemoveResult::NoEmptySlot;
}

EAbilityAddRemoveResult UPlayerAbilityComponent::RemoveActiveAbilityAt(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex, true))
	{
		return EAbilityAddRemoveResult::InvalidIndex;
	}
	if (!ActiveAbilitySlots[SlotIndex])
	{
		return EAbilityAddRemoveResult::UnknownAbility;
	}
	ActiveAbilitySlots[SlotIndex] = nullptr;
	return EAbilityAddRemoveResult::Success;
}

EAbilityAddRemoveResult UPlayerAbilityComponent::RemovePassiveAbilityAt(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex, false))
	{
		return EAbilityAddRemoveResult::InvalidIndex;
	}
	if (!PassiveAbilitySlots[SlotIndex])
	{
		return EAbilityAddRemoveResult::UnknownAbility;
	}
	PassiveAbilitySlots[SlotIndex] = nullptr;
	return EAbilityAddRemoveResult::Success;
}

void UPlayerAbilityComponent::ClearAllAbilities()
{
	// Clear active abilities
	for (int32 i = 0; i < MAX_ACTIVE_SLOTS; ++i)
	{
		ActiveAbilitySlots[i] = nullptr;
	}

	// Clear passive abilities
	for (int32 i = 0; i < MAX_PASSIVE_SLOTS; ++i)
	{
		PassiveAbilitySlots[i] = nullptr;
	}
}

bool UPlayerAbilityComponent::ExecuteAttack(const FVector& TargetLocation)
{
	UAbilityActive* AttackActive = Cast<UAbilityActive>(AttackAbility);
	if (!AttackActive || !AttackActive->CanExecute())
	{
		return false;
	}

	AttackActive->Execute();
	return true;
}

bool UPlayerAbilityComponent::ExecuteSpecial(const FVector& TargetLocation)
{
	UAbilityActive* SpecialActive = Cast<UAbilityActive>(SpecialAbility);
	if (!SpecialActive || !SpecialActive->CanExecute())
	{
		return false;
	}

	SpecialActive->Execute();
	return true;
}

bool UPlayerAbilityComponent::ExecuteDodge(const FVector& Direction)
{
	UAbilityActive* DodgeActive = Cast<UAbilityActive>(DodgeAbility);
	if (!DodgeActive || !DodgeActive->CanExecute())
	{
		return false;
	}

	DodgeActive->Execute();
	return true;
}

void UPlayerAbilityComponent::CheckAndTriggerAbilities()
{
	// Update passive abilities
	UpdatePassiveAbilities();

	// Check active abilities for auto-trigger conditions
	// This would be implemented based on specific game mechanics
}

void UPlayerAbilityComponent::UpdatePassiveAbilities()
{
	// Update all passive abilities
	for (UAbilityBase* PassiveAbility : PassiveAbilitySlots)
	{
		if (PassiveAbility)
		{
			// Passive abilities are typically always active
			// They might need periodic updates or checks
		}
	}
}

bool UPlayerAbilityComponent::IsValidSlotIndex(int32 SlotIndex, bool bIsActiveSlot) const
{
	if (bIsActiveSlot)
	{
		return SlotIndex >= 0 && SlotIndex < MAX_ACTIVE_SLOTS;
	}
	else
	{
		return SlotIndex >= 0 && SlotIndex < MAX_PASSIVE_SLOTS;
	}
}

UAbilityActive* UPlayerAbilityComponent::GetActiveAbilityAt(int32 SlotIndex) const
{
	if (!IsValidSlotIndex(SlotIndex, true))
	{
		return nullptr;
	}
	return Cast<UAbilityActive>(ActiveAbilitySlots[SlotIndex]);
}

UAbilityPassive* UPlayerAbilityComponent::GetPassiveAbilityAt(int32 SlotIndex) const
{
	if (!IsValidSlotIndex(SlotIndex, false))
	{
		return nullptr;
	}
	return Cast<UAbilityPassive>(PassiveAbilitySlots[SlotIndex]);
}

#if WITH_EDITOR
void UPlayerAbilityComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Enforce slot types and ability types in editor
	for (int32 i = 0; i < ActiveAbilitySlots.Num(); ++i)
	{
		if (UAbilityBase* Ability = ActiveAbilitySlots[i])
		{
			if (Ability->AbilityType != EAbilityType::Active)
			{
				ActiveAbilitySlots[i] = nullptr;
			}
			else
			{
				Ability->SlotType = EAbilitySlotType::Active;
			}
		}
	}

	for (int32 i = 0; i < PassiveAbilitySlots.Num(); ++i)
	{
		if (UAbilityBase* Ability = PassiveAbilitySlots[i])
		{
			if (Ability->AbilityType != EAbilityType::Passive)
			{
				PassiveAbilitySlots[i] = nullptr;
			}
			else
			{
				Ability->SlotType = EAbilitySlotType::Passive;
			}
		}
	}

	if (AttackAbility)
	{
		AttackAbility->AbilityType = EAbilityType::Active;
		AttackAbility->SlotType = EAbilitySlotType::Innate;
	}
	if (SpecialAbility)
	{
		SpecialAbility->AbilityType = EAbilityType::Active;
		SpecialAbility->SlotType = EAbilitySlotType::Innate;
	}
	if (DodgeAbility)
	{
		DodgeAbility->AbilityType = EAbilityType::Active;
		DodgeAbility->SlotType = EAbilitySlotType::Innate;
	}
	if (CharacterPassiveInnate)
	{
		CharacterPassiveInnate->AbilityType = EAbilityType::Passive;
		CharacterPassiveInnate->SlotType = EAbilitySlotType::Innate;
	}
}
#endif
