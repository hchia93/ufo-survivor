#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Loot/ExperienceLoot.h"
#include "ExperienceLootPoolComponent.generated.h"

/*
 *	ExperienceLootPoolComponent : 
 *	- Manages a pool of ExperienceLoot actors to avoid spawning too many objects
 *	- Reuses collected experience loot objects by reactivating them
 *	- Placed in GameState for global management
 */
UCLASS(ClassGroup=(GameState), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UExperienceLootPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExperienceLootPoolComponent();

	/// Pool Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TSubclassOf<AExperienceLoot> ExperienceLootClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	int32 InitialPoolSize = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	int32 MaxPoolSize = 100;

	/// Core Functions
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void InitializePool();

	UFUNCTION(BlueprintCallable, Category = "Pool")
	AExperienceLoot* RequestExperienceLoot(const FVector& Location, int32 ExperienceValue, EPlayerExperienceSize ExperienceSize);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void ReturnExperienceLoot(AExperienceLoot* Loot);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void ClearPool();

protected:
	virtual void BeginPlay() override;

private:
	// Pool management
	UPROPERTY()
	TArray<TObjectPtr<AExperienceLoot>> ActiveLootPool;

	UPROPERTY()
	TArray<TObjectPtr<AExperienceLoot>> InactiveLootPool;

	// Helper functions
	void CreatePoolObjects();
	AExperienceLoot* CreateNewLootObject();
	void DeactivateLootObject(AExperienceLoot* Loot);
	void ReactivateLootObject(AExperienceLoot* Loot, const FVector& Location, int32 ExperienceValue, EPlayerExperienceSize ExperienceSize);
};
