#include "Ability/PlayerAbilityComponent.h"
#include "Ability/AbilityInnate.h"
#include "Ability/AbilityActive.h"
#include "Ability/AbilityPassive.h"
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
	SetupInnateAbilities();
	SetupAbilitySlots();
}

void UPlayerAbilityComponent::SetupInnateAbilities()
{
	// Create innate abilities
	AttackAbility = NewObject<UAbilityInnate>(this);
	SpecialAbility = NewObject<UAbilityInnate>(this);
	DodgeAbility = NewObject<UAbilityInnate>(this);
	
	// Set up innate abilities (these should be configured in Blueprint)
	if (AttackAbility)
	{
		AttackAbility->SetWorldContext(GetWorld());
		AttackAbility->SetOwnerActor(GetOwner());
	}
	
	if (SpecialAbility)
	{
		SpecialAbility->SetWorldContext(GetWorld());
		SpecialAbility->SetOwnerActor(GetOwner());
	}
	
	if (DodgeAbility)
	{
		DodgeAbility->SetWorldContext(GetWorld());
		DodgeAbility->SetOwnerActor(GetOwner());
	}
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

bool UPlayerAbilityComponent::AddActiveAbility(UAbilityActive* Ability, int32 SlotIndex)
{
	if (!Ability)
	{
		return false;
	}
	
	// If no specific slot specified, find first empty slot
	if (SlotIndex == -1)
	{
		for (int32 i = 0; i < MAX_ACTIVE_SLOTS; ++i)
		{
			if (!ActiveAbilitySlots[i])
			{
				SlotIndex = i;
				break;
			}
		}
	}
	
	// Validate slot index
	if (!IsValidSlotIndex(SlotIndex, true))
	{
		return false;
	}
	
	// Set up ability
	Ability->SetWorldContext(GetWorld());
	Ability->SetOwnerActor(GetOwner());
	
	// Add to slot
	ActiveAbilitySlots[SlotIndex] = Ability;
	
	return true;
}

bool UPlayerAbilityComponent::AddPassiveAbility(UAbilityPassive* Ability, int32 SlotIndex)
{
	if (!Ability)
	{
		return false;
	}
	
	// If no specific slot specified, find first empty slot
	if (SlotIndex == -1)
	{
		for (int32 i = 0; i < MAX_PASSIVE_SLOTS; ++i)
		{
			if (!PassiveAbilitySlots[i])
			{
				SlotIndex = i;
				break;
			}
		}
	}
	
	// Validate slot index
	if (!IsValidSlotIndex(SlotIndex, false))
	{
		return false;
	}
	
	// Set up ability
	Ability->SetWorldContext(GetWorld());
	Ability->SetOwnerActor(GetOwner());
	
	// Add to slot
	PassiveAbilitySlots[SlotIndex] = Ability;
	
	return true;
}

bool UPlayerAbilityComponent::RemoveActiveAbility(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex, true))
	{
		return false;
	}
	
	ActiveAbilitySlots[SlotIndex] = nullptr;
	return true;
}

bool UPlayerAbilityComponent::RemovePassiveAbility(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex, false))
	{
		return false;
	}
	
	PassiveAbilitySlots[SlotIndex] = nullptr;
	return true;
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
	if (!AttackAbility || !AttackAbility->CanExecute())
	{
		return false;
	}
	
	AttackAbility->ExecuteWithDirection(TargetLocation);
	return true;
}

bool UPlayerAbilityComponent::ExecuteSpecial(const FVector& TargetLocation)
{
	if (!SpecialAbility || !SpecialAbility->CanExecute())
	{
		return false;
	}
	
	SpecialAbility->ExecuteWithDirection(TargetLocation);
	return true;
}

bool UPlayerAbilityComponent::ExecuteDodge(const FVector& Direction)
{
	if (!DodgeAbility || !DodgeAbility->CanExecute())
	{
		return false;
	}
	
	DodgeAbility->ExecuteWithDirection(Direction);
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
	for (UAbilityPassive* PassiveAbility : PassiveAbilitySlots)
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
