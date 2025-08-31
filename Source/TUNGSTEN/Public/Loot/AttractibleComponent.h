#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttractibleComponent.generated.h"

// Forward declarations
class AActor;
class UPrimitiveComponent;

/*
 *	AttractibleComponent : 
 *	- Component that attracts the owner actor to the player when they get close
 *	- Uses overlap detection to trigger attraction
 *	- Moves the owner actor towards the player position
 *	- Designed to work with LootableComponent for collection
 */
UCLASS(ClassGroup=(Loot), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UAttractibleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttractibleComponent();

	/// Attraction Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attraction")
	float AttractionRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attraction")
	float AttractionSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attraction")
	float MinDistanceToPlayer = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attraction")
	bool bEnableAttraction = true;

	/// Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Attraction")
	void OnAttractionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attraction")
	void OnAttractionCompleted();

protected:
	virtual void BeginPlay() override;

	/// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* AttractionSphere;

	/// Runtime State
	UPROPERTY()
	bool bIsAttracting = false;

	UPROPERTY()
	AActor* TargetPlayer = nullptr;

	UPROPERTY()
	FTimerHandle AttractionTimerHandle;

	/// Helper Functions
	void SetupAttractionSphere();
	void OnAttractionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnAttractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void StartAttraction(AActor* Player);
	void StopAttraction();
	void UpdateAttraction();
	bool IsValidPlayer(AActor* Actor) const;
};
