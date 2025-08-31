#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Loot/Loot.h"
#include "LootGeneratorComponent.generated.h"

class UExperienceLootPoolComponent;
class AExperienceLoot;

/*
 *	LootGeneratorComponent : 
 *	- Handles loot generation when actors die
 *	- Receives death notifications and generates appropriate loot
 *	- Uses ExperienceLootPoolComponent for efficient loot management
 *	- Placed in GameState for global management
 */
UCLASS(ClassGroup=(GameState), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API ULootGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULootGeneratorComponent();

	/// Loot Generation Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Generation")
	float LootSpreadRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Generation")
	int32 MaxLootSpreadAttempts = 10;

	/// Core Functions
	UFUNCTION(BlueprintCallable, Category = "Loot Generation")
	void Initialize(UExperienceLootPoolComponent* ExpPool);

	UFUNCTION(BlueprintCallable, Category = "Loot Generation")
	void OnActorDeath(AActor* DeadActor, const FLootParam& LootParam);

	UFUNCTION(BlueprintCallable, Category = "Loot Generation")
	void GenerateExperienceLoot(const FVector& Location, int32 ExperienceValue);

	UFUNCTION(BlueprintCallable, Category = "Loot Generation")
	void GenerateItemLoot(const FVector& Location, const TArray<TSubclassOf<AActor>>& ItemClasses);

protected:
	virtual void BeginPlay() override;

private:
	// Reference to experience pool
	UPROPERTY()
	TObjectPtr<UExperienceLootPoolComponent> ExperiencePool;

	// Helper functions
	FVector GetRandomSpreadLocation(const FVector& CenterLocation);
	bool IsValidLootLocation(const FVector& Location);
};
