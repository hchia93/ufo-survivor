#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability/AbilityBase.h"
#include "AbilityComponent.generated.h"

/*
 * AbilityComponent : 
 * - Handles activation of abilities on runtime.
 * - Manages different types of abilities (Active, Passive, Innate)
 */
UCLASS(ClassGroup=(Ability), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	// List of ability class types to spawn at BeginPlay
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<UAbilityBase>> AbilityClasses;

	// Runtime spawned ability objects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<TObjectPtr<UAbilityBase>> RuntimeAbilities;

	// Execute all abilities if possible
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ExecuteAllAbilities();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	// Spawn & initialize all abilities
	void InitializeAbilities();
};
