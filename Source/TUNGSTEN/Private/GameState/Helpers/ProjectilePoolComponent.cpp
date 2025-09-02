#include "GameState/Helpers/ProjectilePoolComponent.h"
#include "Ability/Object/ProjectileBase.h"
#include "Engine/World.h"
#include "TUNGSTEN/TUNGSTEN.h"

UProjectilePoolComponent::UProjectilePoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectilePoolComponent::BeginPlay()
{
	Super::BeginPlay();

	ConfigurePools(PoolConfigs);
}

void UProjectilePoolComponent::ConfigurePools(const TArray<FProjectileClassPool>& InPools)
{
	PoolConfigs = InPools;
	for (const FProjectileClassPool& Cfg : PoolConfigs)
	{
		if (!Cfg.ProjectileClass)
		{
			continue;
		}

		FProjectilePoolBucket& Bucket = ClassToBuckets.FindOrAdd(Cfg.ProjectileClass);
		// Pre-allocate
		for (int32 i = 0; i < Cfg.InitialSize; ++i)
		{
			FActorSpawnParameters Params;
			Params.Owner = GetOwner();
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AProjectileBase* Instance = GetWorldChecked()->SpawnActor<AProjectileBase>(Cfg.ProjectileClass, FTransform::Identity, Params);
			if (Instance)
			{
				Instance->SetActorEnableCollision(false);
				Instance->SetActorHiddenInGame(true);
				Bucket.Free.Add(Instance);
			}
		}
	}
}

AProjectileBase* UProjectilePoolComponent::AcquireProjectile(TSubclassOf<AProjectileBase> ProjectileClass, const FTransform& SpawnTransform)
{
	if (!ProjectileClass)
	{
		return nullptr;
	}

	FProjectilePoolBucket& Bucket = ClassToBuckets.FindOrAdd(ProjectileClass);

	AProjectileBase* Instance = nullptr;
	if (Bucket.Free.Num() > 0)
	{
		Instance = Bucket.Free.Pop(EAllowShrinking::No);
	}
	else
	{
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Instance = GetWorldChecked()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform, Params);
	}

	if (Instance)
	{
		Instance->SetActorTransform(SpawnTransform);
		Instance->SetActorHiddenInGame(false);
		Instance->SetActorEnableCollision(true);
		Bucket.Used.Add(Instance);
	}

	return Instance;
}

void UProjectilePoolComponent::ReleaseProjectile(AProjectileBase* Projectile)
{
	if (!Projectile)
	{
		return;
	}

	TSubclassOf<AProjectileBase> Cls = Projectile->GetClass();
	FProjectilePoolBucket& Bucket = ClassToBuckets.FindOrAdd(Cls);

	Bucket.Used.Remove(Projectile);
	Projectile->SetActorEnableCollision(false);
	Projectile->SetActorHiddenInGame(true);
	Bucket.Free.Add(Projectile);
}

UWorld* UProjectilePoolComponent::GetWorldChecked() const
{
	UWorld* World = GetWorld();
	check(World);
	return World;
}
