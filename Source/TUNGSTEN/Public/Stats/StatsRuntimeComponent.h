#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stats/StatsDefine.h"

#include "StatsRuntimeComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorReceiveDamageDelegate, AActor*, DamageReceivedActor, float, Value);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDeathDelegate, AActor*, DeathActor);

/*
 * StatsRuntimeComponent : 
 * - Manages runtime HP, ATK, DEF stats
 * - Integrates with Unreal's damage system via TakeDamage()
 * - Fires damage and death events for UI and game logic
 * - Works with StatsLoaderComponent for base stats
 */
UCLASS(ClassGroup=(Stats), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UStatsRuntimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsRuntimeComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Forwarding into unreal damage system.
    float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// Runtime stats management
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseStats(const FCharacterStats& NewBaseStats);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetRuntimeStats(const FCharacterStats& NewRuntimeStats);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	FCharacterStats GetRuntimeStats() const { return RuntimeStats; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	FCharacterStats GetBaseStats() const { return BaseStats; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	FCharacterStats GetPowerUpStats() const { return PowerUpStats; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHPInPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IsAlive() const { return UnitLiveState == EDeathState::Alive; }

	// Power-up management
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddPowerUp(const FCharacterStats& PowerUpStats);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void RemovePowerUp(const FCharacterStats& PowerUpStats);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ClearAllPowerUps();

	// Stat modifier management (for future implementation)
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddStatModifier(const FStatModifier& Modifier);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void RemoveStatModifier(const FString& ModifierID);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ClearAllStatModifiers();

	// Manual damage application (for compatibility with old system)
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void TakeDamageManual(const TArray<FDamageInstance>& Instances);

	// Events
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FActorReceiveDamageDelegate OnDamageReceived;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FActorDeathDelegate OnActorDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	EDeathState UnitLiveState = EDeathState::Alive;

private:
	// Base stats (from data table)
	UPROPERTY()
	FCharacterStats BaseStats;

	// Power-up stats (collected during gameplay)
	UPROPERTY()
	FCharacterStats PowerUpStats;

	// Runtime stats (current values = base + power-ups + modifiers)
	UPROPERTY()
	FCharacterStats RuntimeStats;

	// Stat modifiers (temporary effects)
	UPROPERTY()
	TArray<FStatModifier> StatModifiers;

	// Internal damage processing
	void ProcessDamage(float DamageAmount, AActor* DamageCauser);
	void CheckDeath();
	void RecalculateRuntimeStats();
};
