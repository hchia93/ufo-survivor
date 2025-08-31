#include "GameState/Helpers/ExperienceLootPoolComponent.h"
#include "Loot/ExperienceLoot.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UExperienceLootPoolComponent::UExperienceLootPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// Set default class if not specified
	if (!ExperienceLootClass)
	{
		ExperienceLootClass = AExperienceLoot::StaticClass();
	}
}

void UExperienceLootPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize pool after a short delay to ensure world is ready
	FTimerHandle InitTimer;
	GetWorld()->GetTimerManager().SetTimer(InitTimer, this, &UExperienceLootPoolComponent::InitializePool, 0.1f, false);
}

void UExperienceLootPoolComponent::InitializePool()
{
	if (!ExperienceLootClass || !GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExperienceLootPoolComponent: Cannot initialize pool - missing class or world"));
		return;
	}

	CreatePoolObjects();
}

void UExperienceLootPoolComponent::CreatePoolObjects()
{
	for (int32 i = 0; i < InitialPoolSize; ++i)
	{
		AExperienceLoot* NewLoot = CreateNewLootObject();
		if (NewLoot)
		{
			InactiveLootPool.Add(NewLoot);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("ExperienceLootPoolComponent: Created %d loot objects"), InactiveLootPool.Num());
}

AExperienceLoot* UExperienceLootPoolComponent::CreateNewLootObject()
{
	if (!ExperienceLootClass || !GetWorld())
	{
		return nullptr;
	}

	// Spawn at a hidden location
	FVector HiddenLocation = FVector(0, 0, -10000);
	FRotator HiddenRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AExperienceLoot* NewLoot = GetWorld()->SpawnActor<AExperienceLoot>(ExperienceLootClass, HiddenLocation, HiddenRotation, SpawnParams);
	
	if (NewLoot)
	{
		// Initially deactivated
		NewLoot->SetActorHiddenInGame(true);
		NewLoot->SetActorEnableCollision(false);
		NewLoot->SetActorTickEnabled(false);
	}

	return NewLoot;
}

AExperienceLoot* UExperienceLootPoolComponent::RequestExperienceLoot(const FVector& Location, int32 ExperienceValue, EPlayerExperienceSize ExperienceSize)
{
	// Try to get from inactive pool first
	AExperienceLoot* Loot = nullptr;
	
	if (InactiveLootPool.Num() > 0)
	{
		Loot = InactiveLootPool.Pop();
		ReactivateLootObject(Loot, Location, ExperienceValue, ExperienceSize);
	}
	else if (ActiveLootPool.Num() < MaxPoolSize)
	{
		// Create new object if pool isn't full
		Loot = CreateNewLootObject();
		if (Loot)
		{
			ReactivateLootObject(Loot, Location, ExperienceValue, ExperienceSize);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ExperienceLootPoolComponent: Pool is full, cannot create new loot"));
		return nullptr;
	}

	if (Loot)
	{
		ActiveLootPool.Add(Loot);
	}

	return Loot;
}

void UExperienceLootPoolComponent::ReturnExperienceLoot(AExperienceLoot* Loot)
{
	if (!Loot)
	{
		return;
	}

	// Remove from active pool
	ActiveLootPool.Remove(Loot);
	
	// Deactivate and add to inactive pool
	DeactivateLootObject(Loot);
	InactiveLootPool.Add(Loot);
}

void UExperienceLootPoolComponent::DeactivateLootObject(AExperienceLoot* Loot)
{
	if (!Loot)
	{
		return;
	}

	// Move to hidden location
	Loot->SetActorLocation(FVector(0, 0, -10000));
	
	// Deactivate
	Loot->SetActorHiddenInGame(true);
	Loot->SetActorEnableCollision(false);
	Loot->SetActorTickEnabled(false);
	
	// Reset collection state
	Loot->SetActorHiddenInGame(false);
	Loot->SetActorEnableCollision(true);
}

void UExperienceLootPoolComponent::ReactivateLootObject(AExperienceLoot* Loot, const FVector& Location, int32 ExperienceValue, EPlayerExperienceSize ExperienceSize)
{
	if (!Loot)
	{
		return;
	}

	// Set new properties
	Loot->ExperienceValue = ExperienceValue;
	Loot->ExperienceSize = ExperienceSize;
	
	// Move to target location
	Loot->SetActorLocation(Location);
	
	// Activate
	Loot->SetActorHiddenInGame(false);
	Loot->SetActorEnableCollision(true);
	Loot->SetActorTickEnabled(true);
}

void UExperienceLootPoolComponent::ClearPool()
{
	// Clear active pool
	for (AExperienceLoot* Loot : ActiveLootPool)
	{
		if (IsValid(Loot))
		{
			Loot->Destroy();
		}
	}
	ActiveLootPool.Empty();

	// Clear inactive pool
	for (AExperienceLoot* Loot : InactiveLootPool)
	{
		if (IsValid(Loot))
		{
			Loot->Destroy();
		}
	}
	InactiveLootPool.Empty();
}
