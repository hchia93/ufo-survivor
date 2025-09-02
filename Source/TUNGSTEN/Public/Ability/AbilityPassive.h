#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "TimerManager.h"
#include "AbilityPassive.generated.h"

/*
 *	AbilityPassive :
 *	- No cooldowns - designed for continuous or event-driven effects
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Ability), meta = (BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityPassive : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityPassive();

	/// Passive-Specific Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Passive Ability")
	bool bIsAlwaysActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Passive Ability")
	float UpdateInterval = 0.1f; // How often to check/update passive effects

	/// Passive-Specific Functions
	UFUNCTION(BlueprintCallable, Category = "Passive Ability")
	void StartPassiveMonitoring();

	UFUNCTION(BlueprintCallable, Category = "Passive Ability")
	void StopPassiveMonitoring();

	/// Override CanExecute for passive abilities
	virtual bool CanExecute_Implementation() const override;

protected:
	// Timer handle for passive updates
	FTimerHandle PassiveUpdateTimer;
};
