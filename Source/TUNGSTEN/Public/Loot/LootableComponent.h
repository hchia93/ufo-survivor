#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootableComponent.generated.h"

UENUM(BlueprintType)
enum class ECollectMode : uint8
{
	CollectOnOverlap UMETA(DisplayName = "Collect On Overlap"),   
	CollectUponConfirm UMETA(DisplayName = "Collect Upon Confirm")  
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLooted, AActor*, Looter);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API ULootableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULootableComponent();

	/// Collection Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection")
	ECollectMode CollectMode = ECollectMode::CollectOnOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection")
	float CollectionRadius = 150.0f;

	/// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollectionSphere;

	/// Events
	UPROPERTY(BlueprintAssignable, Category = "Loot")
	FOnLooted OnLooted;

	/// Core Functions
	UFUNCTION(BlueprintCallable, Category = "Loot")
	void TriggerLoot(AActor* Looter);

	UFUNCTION(BlueprintCallable, Category = "Loot")
	bool ShouldAutoCollect() const { return CollectMode == ECollectMode::CollectOnOverlap; }

	UFUNCTION(BlueprintCallable, Category = "Loot")
	void SetupCollectionSphere();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
