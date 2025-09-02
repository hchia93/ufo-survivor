#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Ability/AbilityDefine.h"
#include "AbilityBase.generated.h"

class AActor;
class UExecutionModule;

/*
 *	AbilityBase :
 *	- Base class for all abilities (Active and Passive)
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Ability), meta = (BlueprintSpawnableComponent))
class TUNGSTEN_API UAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	UAbilityBase();

public:
	/// Basic Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	EAbilityType AbilityType = EAbilityType::Active;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	EAbilitySlotType SlotType = EAbilitySlotType::Active;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	FString AbilityName = TEXT("DBG NO TEXT");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	FString AbilityDescription = TEXT("DBG NO TEXT");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<AActor> OwnerActor;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivate();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetOwner(AActor* NewOwner);

	UFUNCTION(BlueprintPure, Category = "Ability")
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Ability Execution")
	bool CanExecute() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Ability Execution")
	bool Commit();

	UFUNCTION(BlueprintNativeEvent, Category = "Ability Execution")
	void Execute();
};
