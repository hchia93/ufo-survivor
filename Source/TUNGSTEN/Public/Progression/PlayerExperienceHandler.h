#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "Progression/PlayerExperienceRequirement.h"
#include "PlayerExperienceHandler.generated.h"

// Forward declarations
class UDataTable;
class AGameStateBase;

// Delegate for player level up event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLevelUp, int32, NewLevel);

/*
 *	PlayerExperienceHandler : 
 *	- Manages player experience and leveling system
 *	- Uses data table for experience requirements
 *	- Broadcasts level up events
 *	- Resets each session
 */
UCLASS(BlueprintType, ClassGroup=(Progression))
class TUNGSTEN_API UPlayerExperienceHandler : public UObject
{
	GENERATED_BODY()

public:
	UPlayerExperienceHandler();

	/// Data Table Configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Experience")
	TObjectPtr<UDataTable> ExperienceDataTable;

	/// Runtime State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	int32 CurrentLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	int32 TotalCollectedExp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	int32 ExpToNextLevel = 0;

	/// Events
	UPROPERTY(BlueprintAssignable, Category = "Experience")
	FOnPlayerLevelUp OnPlayerLevelUp;

	/// Core Functions
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void Initialize(AGameStateBase* GameState);

	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddExperience(int32 ExperienceAmount);

	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddLevel(int32 LevelsToAdd = 1);

	UFUNCTION(BlueprintCallable, Category = "Experience")
	void ResetForNewSession();

	UFUNCTION(BlueprintCallable, Category = "Experience")
	float GetLevelProgress() const;

	UFUNCTION(BlueprintCallable, Category = "Experience")
	bool GetCurrentLevelData(FPlayerExperienceRequirement& OutData) const;

	UFUNCTION(BlueprintCallable, Category = "Experience")
	bool GetNextLevelData(FPlayerExperienceRequirement& OutData) const;

	UFUNCTION(BlueprintCallable, Category = "Experience")
	bool GetLevelData(int32 Level, FPlayerExperienceRequirement& OutData) const;

protected:
	virtual void BeginPlay();

private:
	UPROPERTY()
	TObjectPtr<AGameStateBase> OwningGameState;

	// Helper functions
	void CheckLevelUp();
	void UpdateExperienceRequirements();
	const FPlayerExperienceRequirement* FindLevelData(int32 Level) const;
};
