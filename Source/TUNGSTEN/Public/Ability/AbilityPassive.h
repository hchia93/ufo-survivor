#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityCooldownHandler.h"
#include "AbilityPassive.generated.h"

/*
 *	AbilityPassive : 
 *	- Passive abilities that provide buffs or modify values
 *	- These abilities typically don't have cooldowns or targets
 *	- They modify stats, provide passive effects, or trigger on certain conditions
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityPassive : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityPassive();

	/// Passive Effect Interface
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnPassiveActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void OnPassiveDeactivated();

	/// Activation/Deactivation
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ActivatePassive();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void DeactivatePassive();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bIsPassiveActive = false;

	/// Optional Cooldown Support
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cooldown")
	bool bUseCooldown = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cooldown", meta = (EditCondition = "bUseCooldown"))
	TObjectPtr<UAbilityCooldownHandler> CooldownHandler;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cooldown", meta = (EditCondition = "bUseCooldown"))
	float CooldownTime = 1.0f;

protected:
	// UAbilityBase inherits from UObject, not AActor, so no BeginPlay
};
