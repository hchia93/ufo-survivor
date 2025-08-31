#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Loot/LootableComponent.h"
#include "Loot/AttractibleComponent.h"
#include "ExperienceLoot.generated.h"

UENUM(BlueprintType)
enum class EPlayerExperienceSize : uint8
{
	Small,
	Medium,
	Large,
	Ultra
};

/*
 *	AExperienceLoot : 
 *	- Actor that provides experience when collected
 *	- Uses OnActorOverlap for collection detection
 *	- Binds to OnLoot event for experience handling
 *	- Has AttractibleComponent for player attraction
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Loot))
class TUNGSTEN_API AExperienceLoot : public AActor
{
	GENERATED_BODY()

public:
	AExperienceLoot();

	/// Experience Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	EPlayerExperienceSize ExperienceSize = EPlayerExperienceSize::Small;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int32 ExperienceValue = 10;

	/// Visual Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSubclassOf<UUserWidget> ExperienceWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float WidgetScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FVector WidgetOffset = FVector(0.0f, 0.0f, 100.0f);

	/// Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Experience")
	void OnExperienceCollected(int32 CollectedExperience);

	/// Pool Management
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void ReturnToPool();

protected:
	virtual void BeginPlay() override;

	/// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* ExperienceWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class ULootableComponent* LootableComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAttractibleComponent* AttractibleComponent;

	/// Runtime State
	UPROPERTY()
	bool bIsCollected = false;

	UPROPERTY()
	UUserWidget* ExperienceWidgetInstance = nullptr;

	/// Helper Functions
	void SetupExperienceWidget();
	void UpdateWidgetRotation();
	void HandleExperienceCollection(AActor* Looter);

	/// Overlap Events
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
