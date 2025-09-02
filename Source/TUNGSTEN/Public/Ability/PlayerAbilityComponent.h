#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityActive.h"
#include "Ability/AbilityPassive.h"
#include "Ability/AbilityDefine.h"
#include "PlayerAbilityComponent.generated.h"

/*
 *	PlayerAbilityComponent :
 *	- Specialized ability component for player characters
 *	- Has fixed innate abilities: Attack, Special, Dodge
 *	- 5 Active ability slots (empty initially, filled through gameplay)
 *	- 5 Passive ability slots (empty initially, filled through gameplay)
 *	- Auto-triggers abilities when conditions are met
 */
UCLASS(ClassGroup = (Ability), meta = (BlueprintSpawnableComponent))
class TUNGSTEN_API UPlayerAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerAbilityComponent();

	/// Innate Abilities (Fixed)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityBase> AttackAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityBase> SpecialAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityBase> DodgeAbility;

	// Optional innate passive
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Innate Abilities")
	TObjectPtr<UAbilityBase> CharacterPassiveInnate;

	/// Active Ability Slots (5 slots, empty initially)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Active Abilities")
	TArray<TObjectPtr<UAbilityBase>> ActiveAbilitySlots;

	/// Passive Ability Slots (5 slots, empty initially)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passive Abilities")
	TArray<TObjectPtr<UAbilityBase>> PassiveAbilitySlots;

	/// Core Functions
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void InitializeAbilities();

	// Typed getters for convenience
	UFUNCTION(BlueprintPure, Category = "Abilities")
	UAbilityActive* GetActiveAbilityAt(int32 SlotIndex) const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	UAbilityPassive* GetPassiveAbilityAt(int32 SlotIndex) const;

	// Add ability: auto-insert into first empty slot (left-to-right). Returns result enum
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	EAbilityAddRemoveResult AddActiveAbility(UAbilityBase* Ability);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	EAbilityAddRemoveResult AddPassiveAbility(UAbilityBase* Ability);

	// Remove ability by slot index
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	EAbilityAddRemoveResult RemoveActiveAbilityAt(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	EAbilityAddRemoveResult RemovePassiveAbilityAt(int32 SlotIndex);

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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

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
