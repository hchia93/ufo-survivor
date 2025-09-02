#include "Stats/StatsLoaderComponent.h"
#include "Stats/StatsDefine.h"
#include "Engine/DataTable.h"

UStatsLoaderComponent::UStatsLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatsLoaderComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadStatsFromDataTable();
}

FCharacterStats UStatsLoaderComponent::GetBaseStats() const
{
	return CachedBaseStats;
}

bool UStatsLoaderComponent::IsDataTableLoaded() const
{
	return !StatsRowHandle.IsNull() && StatsRowHandle.DataTable != nullptr;
}

void UStatsLoaderComponent::ReloadStats()
{
	LoadStatsFromDataTable();
}

void UStatsLoaderComponent::LoadStatsFromDataTable()
{
	if (!StatsRowHandle.IsNull() && StatsRowHandle.DataTable != nullptr)
	{
		FName StatsRowName = StatsRowHandle.RowName;
		FCharacterStats* Stats = StatsRowHandle.DataTable->FindRow<FCharacterStats>(StatsRowName, "");
		if (Stats != nullptr)
		{
			CachedBaseStats = *Stats;
		}
	}
}
