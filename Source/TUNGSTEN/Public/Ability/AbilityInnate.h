#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityCooldownHandler.h"
#include "AbilityInnate.generated.h"

/*
 *	AbilityInnate : 
 *	- Innate abilities like basic attack, special attack, and dodge
 *	- These abilities have cooldowns and are vector-based (no target required)
 *	- Can be executed without locking onto enemies
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityInnate : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityInnate();

	/// Cooldown Management
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool GetIsInCooldown() const { return !CooldownHandler->IsCooldownReady(); }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetCooldownPercent() const { return CooldownHandler->GetCooldownPercent(OwnerActor); }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void EnterCooldown() { CooldownHandler->EnterCooldown(OwnerActor); }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TObjectPtr<UAbilityCooldownHandler> CooldownHandler;

	/// Execution Interface
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool CanExecute() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void Execute();

	/// Vector-based execution (direction-based)
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void ExecuteWithDirection(const FVector& Direction);

protected:
	// UAbilityBase inherits from UObject, not AActor, so no BeginPlay
};
