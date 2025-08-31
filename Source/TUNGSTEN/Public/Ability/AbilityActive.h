#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "Ability/AbilityCooldownHandler.h"
#include "AbilityActive.generated.h"

UENUM(BlueprintType)
enum class EAbilityAcquireTarget : uint8
{
	NoTargetRequired,
	SelfOnly,
	SingleAllyOnly,
	SingleEnemyOnly,
	MultipleAlly,
	MultipleEnemy
};

/*
 *	AbilityActive : 
 *	- Active abilities that may or may not require targets
 *	- These abilities have cooldowns and can be target-locked or free-form
 *	- Handles target acquisition and validation
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityActive : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityActive();

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

	/// Target Management
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool HasValidTarget() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void AcquireTargets();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	EAbilityAcquireTarget AcquireTarget = EAbilityAcquireTarget::NoTargetRequired;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<TObjectPtr<AActor>> Targets;

protected:
	// UAbilityBase inherits from UObject, not AActor, so no BeginPlay
};
