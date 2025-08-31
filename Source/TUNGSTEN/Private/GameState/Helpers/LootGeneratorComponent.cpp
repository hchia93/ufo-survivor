#include "GameState/Helpers/LootGeneratorComponent.h"
#include "GameState/Helpers/ExperienceLootPoolComponent.h"
#include "Loot/ExperienceLoot.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ULootGeneratorComponent::ULootGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULootGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULootGeneratorComponent::Initialize(UExperienceLootPoolComponent* ExpPool)
{
	ExperiencePool = ExpPool;
	
	if (!ExperiencePool)
	{
		UE_LOG(LogTemp, Warning, TEXT("LootGeneratorComponent: Experience pool is null"));
	}
}

void ULootGeneratorComponent::OnActorDeath(AActor* DeadActor, const FLootParam& LootParam)
{
	if (!DeadActor)
	{
		return;
	}

	FVector DeathLocation = DeadActor->GetActorLocation();

	// Generate experience loot if any
	if (LootParam.DroppedExp > 0)
	{
		GenerateExperienceLoot(DeathLocation, LootParam.DroppedExp);
	}

	// Generate item loot if any
	if (LootParam.DroppedItems.Num() > 0)
	{
		GenerateItemLoot(DeathLocation, LootParam.DroppedItems);
	}
}

void ULootGeneratorComponent::GenerateExperienceLoot(const FVector& Location, int32 ExperienceValue)
{
	if (!ExperiencePool)
	{
		UE_LOG(LogTemp, Warning, TEXT("LootGeneratorComponent: Cannot generate experience loot - no pool available"));
		return;
	}

	// Determine experience size based on value
	EPlayerExperienceSize ExperienceSize = EPlayerExperienceSize::Small;
	if (ExperienceValue >= 100)
	{
		ExperienceSize = EPlayerExperienceSize::Ultra;
	}
	else if (ExperienceValue >= 75)
	{
		ExperienceSize = EPlayerExperienceSize::Large;
	}
	else if (ExperienceValue >= 50)
	{
		ExperienceSize = EPlayerExperienceSize::Medium;
	}

	// Get spread location
	FVector LootLocation = GetRandomSpreadLocation(Location);
	
	// Request loot from pool
	AExperienceLoot* Loot = ExperiencePool->RequestExperienceLoot(LootLocation, ExperienceValue, ExperienceSize);
	
	if (Loot)
	{
		UE_LOG(LogTemp, Log, TEXT("LootGeneratorComponent: Generated %d exp at location %s"), ExperienceValue, *LootLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LootGeneratorComponent: Failed to generate experience loot"));
	}
}

void ULootGeneratorComponent::GenerateItemLoot(const FVector& Location, const TArray<TSubclassOf<AActor>>& ItemClasses)
{
	if (!GetWorld())
	{
		return;
	}

	for (TSubclassOf<AActor> ItemClass : ItemClasses)
	{
		if (!ItemClass)
		{
			continue;
		}

		// Get spread location
		FVector LootLocation = GetRandomSpreadLocation(Location);
		
		// Spawn item
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass, LootLocation, FRotator::ZeroRotator, SpawnParams);
		
		if (SpawnedItem)
		{
			UE_LOG(LogTemp, Log, TEXT("LootGeneratorComponent: Generated item %s at location %s"), *ItemClass->GetName(), *LootLocation.ToString());
		}
	}
}

FVector ULootGeneratorComponent::GetRandomSpreadLocation(const FVector& CenterLocation)
{
	FVector SpreadLocation = CenterLocation;
	int32 Attempts = 0;
	
	do
	{
		// Generate random offset within spread radius
		float RandomAngle = FMath::RandRange(0.0f, 360.0f);
		float RandomDistance = FMath::RandRange(0.0f, LootSpreadRadius);
		
		FVector Offset = FVector(
			FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
			FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
			0.0f
		);
		
		SpreadLocation = CenterLocation + Offset;
		Attempts++;
		
	} while (!IsValidLootLocation(SpreadLocation) && Attempts < MaxLootSpreadAttempts);
	
	return SpreadLocation;
}

bool ULootGeneratorComponent::IsValidLootLocation(const FVector& Location)
{
	// Simple validation - can be expanded with more sophisticated checks
	// For now, just check if the location is not too close to the ground or other obstacles
	
	FHitResult HitResult;
	FVector Start = Location + FVector(0, 0, 50);
	FVector End = Location + FVector(0, 0, -50);
	
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
	
	// Valid if we hit something (not too high) and not too close to ground
	return HitResult.bBlockingHit && HitResult.Distance > 10.0f;
}
