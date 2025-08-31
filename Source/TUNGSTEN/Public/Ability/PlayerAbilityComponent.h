#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityInnate.h"
#include "Ability/AbilityActive.h"
#include "Ability/AbilityPassive.h"
#include "PlayerAbilityComponent.generated.h"

/*
 *	PlayerAbilityComponent : 
 *	- Specialized ability component for player characters
 *	- Has fixed innate abilities: Attack, Special, Dodge
 *	- 5 Active ability slots (empty initially, filled through gameplay)
 *	- 5 Passive ability slots (empty initially, filled through gameplay)
 *	- Auto-triggers abilities when conditions are met
 */
UCLASS(ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UPlayerAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerAbilityComponent();

	/// Innate Abilities (Fixed)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityInnate> AttackAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityInnate> SpecialAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityInnate> DodgeAbility;

	/// Active Ability Slots (5 slots, empty initially)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Active Abilities")
	TArray<TObjectPtr<UAbilityActive>> ActiveAbilitySlots;

	/// Passive Ability Slots (5 slots, empty initially)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passive Abilities")
	TArray<TObjectPtr<UAbilityPassive>> PassiveAbilitySlots;

	/// Core Functions
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void InitializeAbilities();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool AddActiveAbility(UAbilityActive* Ability, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool AddPassiveAbility(UAbilityPassive* Ability, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool RemoveActiveAbility(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool RemovePassiveAbility(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ClearAllAbilities();

	/// Ability Execution
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ExecuteAttack(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ExecuteSpecial(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ExecuteDodge(const FVector& Direction);

	/// Auto-trigger System
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void CheckAndTriggerAbilities();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Constants
	static const int32 MAX_ACTIVE_SLOTS = 5;
	static const int32 MAX_PASSIVE_SLOTS = 5;

	// Helper functions
	void SetupInnateAbilities();
	void SetupAbilitySlots();
	bool IsValidSlotIndex(int32 SlotIndex, bool bIsActive) const;
	void UpdatePassiveAbilities();
};
