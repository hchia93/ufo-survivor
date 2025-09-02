#include "Stats/StatsRuntimeComponent.h"
#include "Stats/StatsLoaderComponent.h"
#include "Stats/StatsDefine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UStatsRuntimeComponent::UStatsRuntimeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	UnitLiveState = EDeathState::Alive;
}

void UStatsRuntimeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Try to get base stats from StatsLoaderComponent if available
	if (AActor* Owner = GetOwner())
	{
		if (UStatsLoaderComponent* StatsLoader = Owner->FindComponentByClass<UStatsLoaderComponent>())
		{
			SetBaseStats(StatsLoader->GetBaseStats());
		}
	}
}

float UStatsRuntimeComponent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Process damage through our system
	ProcessDamage(DamageAmount, DamageCauser);

	// Return the actual damage taken (for Unreal's damage system)
	return DamageAmount;
}

void UStatsRuntimeComponent::SetBaseStats(const FCharacterStats& NewBaseStats)
{
	BaseStats = NewBaseStats;
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::SetRuntimeStats(const FCharacterStats& NewRuntimeStats)
{
	RuntimeStats = NewRuntimeStats;
	CheckDeath();
}

float UStatsRuntimeComponent::GetHPInPercent() const
{
	if (UnitLiveState == EDeathState::Alive && BaseStats.HP > 0)
	{
		return (float)RuntimeStats.HP / (float)BaseStats.HP;
	}
	return 0.0f;
}

void UStatsRuntimeComponent::AddPowerUp(const FCharacterStats& NewPowerUpStats)
{
	this->PowerUpStats += NewPowerUpStats;
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::RemovePowerUp(const FCharacterStats& PowerUpStatsToRemove)
{
	this->PowerUpStats -= PowerUpStatsToRemove;
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::ClearAllPowerUps()
{
	PowerUpStats = FCharacterStats();
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::AddStatModifier(const FStatModifier& Modifier)
{
	StatModifiers.Add(Modifier);
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::RemoveStatModifier(const FString& ModifierID)
{
	for (int32 i = StatModifiers.Num() - 1; i >= 0; i--)
	{
		if (StatModifiers[i].ModifierID == ModifierID)
		{
			StatModifiers.RemoveAt(i);
		}
	}
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::ClearAllStatModifiers()
{
	StatModifiers.Empty();
	RecalculateRuntimeStats();
}

void UStatsRuntimeComponent::TakeDamageManual(const TArray<FDamageInstance>& Instances)
{
	if (UnitLiveState == EDeathState::Alive)
	{
		for (const FDamageInstance& Instance : Instances)
		{
			ProcessDamage(Instance.DamageValue, nullptr);
		}
	}
}

void UStatsRuntimeComponent::ProcessDamage(float DamageAmount, AActor* DamageCauser)
{
	if (UnitLiveState == EDeathState::Alive)
	{
		// Apply damage to HP
		RuntimeStats.HP = FMath::Max(0, RuntimeStats.HP - (int32)DamageAmount);

		// Broadcast damage event for UI
		OnDamageReceived.Broadcast(GetOwner(), DamageAmount);

		// Check if unit died
		CheckDeath();
	}
}

void UStatsRuntimeComponent::CheckDeath()
{
	if (RuntimeStats.HP <= 0 && UnitLiveState == EDeathState::Alive)
	{
		UnitLiveState = EDeathState::Dead;
		OnActorDeath.Broadcast(GetOwner());
	}
}

void UStatsRuntimeComponent::RecalculateRuntimeStats()
{
	// Start with base stats
	FCharacterStats NewRuntimeStats = BaseStats;

	// Add power-up stats
	NewRuntimeStats += PowerUpStats;

	// Apply stat modifiers (for future implementation)
	// This is where we'll handle additive, multiplicative, and temporary modifiers
	// For now, we'll just add all modifiers as additive
	for (const FStatModifier& Modifier : StatModifiers)
	{
		switch (Modifier.ModifierType)
		{
		case EStatModifierType::Additive:
			NewRuntimeStats += Modifier.StatModification;
			break;
		case EStatModifierType::Multiplicative:
			// TODO: Implement multiplicative modifiers
			// For now, just add them as additive
			NewRuntimeStats += Modifier.StatModification;
			break;
		case EStatModifierType::Temporary:
			// TODO: Implement temporary modifiers
			// For now, just add them as additive
			NewRuntimeStats += Modifier.StatModification;
			break;
		}
	}

	// Preserve current HP if it's higher than the new max HP
	int32 CurrentHP = RuntimeStats.HP;
	RuntimeStats = NewRuntimeStats;

	// Don't reduce HP below 0, but allow it to be reduced if it exceeds new max
	if (CurrentHP > 0 && CurrentHP <= RuntimeStats.HP)
	{
		RuntimeStats.HP = CurrentHP;
	}

	// Check for death after recalculation
	CheckDeath();
}
