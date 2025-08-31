#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityCooldownHandler.generated.h"

class AActor;

/*
 *	AbilityCooldownHandler : 
 *	- Handles cooldown logic for abilities
 *	- Manages cooldown timers and state
 *	- Provides cooldown status and progress information
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityCooldownHandler : public UObject
{
	GENERATED_BODY()

public:
	UAbilityCooldownHandler();

	/// Cooldown Configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cooldown")
	float CooldownTime = 1.0f;

	/// Cooldown State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cooldown")
	bool bIsInCooldown = false;

	/// Cooldown Functions
	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	bool IsCooldownReady() const;

	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	float GetCooldownPercent(AActor* OwnerActor) const;

	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	void EnterCooldown(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	void ResetCooldown(AActor* OwnerActor);

protected:
	// UObject, not AActor, so no BeginPlay

private:
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	UFUNCTION()
	void OnExitCooldown();
};
