#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Stats/StatsDefine.h"

#include "StatsLoaderComponent.generated.h"

/*
 * StatsLoaderComponent : 
 * - Loads base stats from data tables
 * - Provides base stats to StatsRuntimeComponent
 * - Handles data table integration
 */
UCLASS(ClassGroup=(Stats), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UStatsLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsLoaderComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Data table configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	FDataTableRowHandle StatsRowHandle;

	// Get base stats from data table
	UFUNCTION(BlueprintCallable, Category = "Stats")
	FCharacterStats GetBaseStats() const;

	// Check if data table is properly loaded
	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IsDataTableLoaded() const;

	// Reload stats from data table
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ReloadStats();

private:
	// Cached base stats from data table
	UPROPERTY()
	FCharacterStats CachedBaseStats;

	// Load stats from data table
	void LoadStatsFromDataTable();
};
