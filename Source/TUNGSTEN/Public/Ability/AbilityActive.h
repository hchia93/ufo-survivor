#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "AbilityCooldownHandler.h"
#include "AbilityActive.generated.h"

class UAbilityCooldownHandler;

/*
 *	AbilityActive :
 *	- Active abilities that require player input to trigger
 *	- Includes cooldown management via AbilityCooldownHandler
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Ability), meta = (BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityActive : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityActive();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TObjectPtr<UAbilityCooldownHandler> CooldownHandler;

	/// Override CanExecute to include cooldown check
	virtual bool CanExecute_Implementation() const override;
};
