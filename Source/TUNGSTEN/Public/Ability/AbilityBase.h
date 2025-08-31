#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/TimerHandle.h"
#include "AbilityBase.generated.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Active,
	Passive,
	Innate
};

// Forward declaration
class UAbilityCooldownHandler;

/*
 *	AbilityBase : 
 *	- Base class for all abilities
 *	- Provides common interfaces for world context and owner actor
 *	- Does NOT handle cooldown, targeting, or execution logic
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	UAbilityBase();

	// Ability Type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	EAbilityType AbilityType = EAbilityType::Active;

	// World Context Management
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetWorldContext(UWorld* Context);

	UFUNCTION(BlueprintPure, Category = "Ability")
	UWorld* GetWorld() const override { return WorldContext; }

	// Owner Actor Management
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetOwnerActor(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Ability")
	AActor* GetOwnerActor() const { return OwnerActor; }

protected:
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	TObjectPtr<UWorld> WorldContext;
};
