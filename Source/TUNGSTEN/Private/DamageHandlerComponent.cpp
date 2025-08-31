#include "DamageHandlerComponent.h"

UDamageHandlerComponent::UDamageHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDamageHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!StatsRowHandle.IsNull())
	{
		FName StatsRowName = StatsRowHandle.RowName;
		FUnitStats* Stats = StatsRowHandle.DataTable->FindRow<FUnitStats>(StatsRowName, "");
		if (Stats != nullptr)
		{
			BaseStats = *Stats;
			RuntimeStats = *Stats;
		}
	}
}

void UDamageHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UDamageHandlerComponent::GetHPInPercent() const
{
	if (UnitLiveState == EUnitLiveState::Alive)
	{
		return BaseStats.HP != 0 ? (RuntimeStats.HP / BaseStats.HP) : 0;
	}
	else
	{
		return 0.0f;
	}
}

void UDamageHandlerComponent::TakeDamage(const TArray<FDamageInstance>& Instances)
{
	if (UnitLiveState == EUnitLiveState::Alive)
	{
		for (auto&& instance : Instances)
		{
			RuntimeStats.HP -= instance.DamageValue;
			OnDamageReceived.Broadcast(GetOwner(), instance.DamageValue);
		}

		// Check if unit should die after taking damage
		if (RuntimeStats.HP <= 0)
		{
			UnitLiveState = EUnitLiveState::Dead;
			OnActorDeath.Broadcast(GetOwner());
		}
	}
}