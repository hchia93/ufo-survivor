// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageHelper.h"
#include "DamageHandlerComponent.generated.h"


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorReceiveDamageDelegate, AActor*, DamageReceivedActor, float, Value);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDeathDelegate, AActor*, DeathActor);

/*
 * RDamageHandlerComponent : Handles all kind of damage packaging, sending, receiving logics.
 */
UCLASS( ClassGroup=(DamageHandler), meta=(BlueprintSpawnableComponent) )
class TUNGSTEN_API UDamageHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDamageHandlerComponent();

	/* Targetting FRUnitStats Type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FDataTableRowHandle StatsRowHandle;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "TUNGSTEN")
	float GetHPInPercent() const;

	// For ability that may deal compound damage.
	UFUNCTION(BlueprintCallable, Category = "TUNGSTEN")
	void TakeDamage(const TArray<FDamageInstance>& Instances);

	UFUNCTION(BlueprintCallable, Category = "TUNGSTEN")
    FUnitStats GetRuntimeStats() { return RuntimeStats; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EUnitLiveState UnitLiveState = EUnitLiveState::Alive;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FActorReceiveDamageDelegate OnDamageReceived;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FActorDeathDelegate OnActorDeath;


private:

	// Runtime Stats.
	UPROPERTY()
	FUnitStats RuntimeStats;

	// Base values stats. Used to indicates its max values
	UPROPERTY()
	FUnitStats BaseStats;
};
