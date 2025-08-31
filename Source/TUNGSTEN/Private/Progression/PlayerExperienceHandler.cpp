#include "Progression/PlayerExperienceHandler.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/Engine.h"

UPlayerExperienceHandler::UPlayerExperienceHandler()
{
	CurrentLevel = 1;
	TotalCollectedExp = 0;
	ExpToNextLevel = 0;
}

void UPlayerExperienceHandler::BeginPlay()
{
	// This is a UObject, not an Actor, so BeginPlay might not be called
	// Initialization should be done through Initialize() function
}

void UPlayerExperienceHandler::Initialize(AGameStateBase* GameState)
{
	OwningGameState = GameState;
	ResetForNewSession();
}

void UPlayerExperienceHandler::AddExperience(int32 ExperienceAmount)
{
	if (ExperienceAmount <= 0)
	{
		return;
	}

	TotalCollectedExp += ExperienceAmount;
	
	// Check if we can level up
	CheckLevelUp();
	
	// Update requirements for next level
	UpdateExperienceRequirements();
}

void UPlayerExperienceHandler::AddLevel(int32 LevelsToAdd)
{
	if (LevelsToAdd <= 0)
	{
		return;
	}

	// Get the target level
	int32 TargetLevel = CurrentLevel + LevelsToAdd;
	
	// Find the target level data
	FPlayerExperienceRequirement TargetLevelData;
	if (GetLevelData(TargetLevel, TargetLevelData))
	{
		// Set current level to target level
		CurrentLevel = TargetLevel;
		
		// Set TotalCollectedExp to the accumulative value for this level
		// This represents the total experience collected from level 1
		TotalCollectedExp = TargetLevelData.RequiredEXP;
		
		// Update ExpToNextLevel to the direct required exp for this level
		UpdateExperienceRequirements();
		
		// Broadcast level up event
		OnPlayerLevelUp.Broadcast(CurrentLevel);
	}
	else
	{
		// If target level doesn't exist, just add the levels without changing exp
		CurrentLevel += LevelsToAdd;
		UpdateExperienceRequirements();
		
		// Broadcast level up event
		OnPlayerLevelUp.Broadcast(CurrentLevel);
	}
}

void UPlayerExperienceHandler::ResetForNewSession()
{
	CurrentLevel = 1;
	TotalCollectedExp = 0;
	UpdateExperienceRequirements();
}

float UPlayerExperienceHandler::GetLevelProgress() const
{
	if (ExpToNextLevel <= 0)
	{
		return 1.0f; // Max level reached
	}

	// Calculate progress within current level
	FPlayerExperienceRequirement CurrentLevelData;
	if (GetCurrentLevelData(CurrentLevelData))
	{
		// Progress = (TotalCollectedExp - CurrentLevelRequired) / (NextLevelRequired - CurrentLevelRequired)
		int32 ExpInCurrentLevel = TotalCollectedExp - CurrentLevelData.RequiredEXP;
		return FMath::Clamp((float)ExpInCurrentLevel / (float)ExpToNextLevel, 0.0f, 1.0f);
	}

	return 0.0f;
}

bool UPlayerExperienceHandler::GetCurrentLevelData(FPlayerExperienceRequirement& OutData) const
{
	const FPlayerExperienceRequirement* Data = FindLevelData(CurrentLevel);
	if (Data)
	{
		OutData = *Data;
		return true;
	}
	return false;
}

bool UPlayerExperienceHandler::GetNextLevelData(FPlayerExperienceRequirement& OutData) const
{
	const FPlayerExperienceRequirement* Data = FindLevelData(CurrentLevel + 1);
	if (Data)
	{
		OutData = *Data;
		return true;
	}
	return false;
}

bool UPlayerExperienceHandler::GetLevelData(int32 Level, FPlayerExperienceRequirement& OutData) const
{
	const FPlayerExperienceRequirement* Data = FindLevelData(Level);
	if (Data)
	{
		OutData = *Data;
		return true;
	}
	return false;
}

void UPlayerExperienceHandler::CheckLevelUp()
{
	FPlayerExperienceRequirement NextLevelData;
	if (!GetNextLevelData(NextLevelData))
	{
		return; // No more levels
	}

	// Check if we have enough experience for next level
	if (TotalCollectedExp >= NextLevelData.RequiredEXP)
	{
		CurrentLevel++;
		
		// Broadcast level up event
		OnPlayerLevelUp.Broadcast(CurrentLevel);
		
		// Check for additional level ups (in case we gained multiple levels at once)
		CheckLevelUp();
	}
}

void UPlayerExperienceHandler::UpdateExperienceRequirements()
{
	FPlayerExperienceRequirement CurrentLevelData;
	FPlayerExperienceRequirement NextLevelData;

	if (GetCurrentLevelData(CurrentLevelData))
	{
		// Calculate ExpToNextLevel
		if (GetNextLevelData(NextLevelData))
		{
			// ExpToNextLevel = NextLevelRequired - CurrentLevelRequired
			// This gives us the direct experience needed for the next level
			ExpToNextLevel = NextLevelData.RequiredEXP - CurrentLevelData.RequiredEXP;
		}
		else
		{
			ExpToNextLevel = 0; // Max level reached
		}
	}
}

const FPlayerExperienceRequirement* UPlayerExperienceHandler::FindLevelData(int32 Level) const
{
	if (!ExperienceDataTable)
	{
		return nullptr;
	}

	// Find row by level
	FString ContextString;
	FPlayerExperienceRequirement* RowData = ExperienceDataTable->FindRow<FPlayerExperienceRequirement>(
		*FString::FromInt(Level), ContextString);

	return RowData;
}
