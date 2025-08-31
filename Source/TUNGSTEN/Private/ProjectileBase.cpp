#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageHandlerComponent.h"
#include "GameState/Helpers/ProjectilePoolComponent.h"
#include "TUNGSTEN/TUNGSTEN.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(8.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetNotifyRigidBodyCollision(false);
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCollisionBeginOverlap);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

void AProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsActive)
	{
		return;
	}

	if (Config.MovementMode == EProjectileMovementMode::Linear)
	{
		const FVector Delta = LinearDirection * Config.Speed * DeltaSeconds;
		AddActorWorldOffset(Delta, true);
	}
	else // Parabola
	{
		Velocity.Z += Config.GravityZ * DeltaSeconds;
		AddActorWorldOffset(Velocity * DeltaSeconds, true);
		if (Config.bRotateToVelocity && !Velocity.IsNearlyZero())
		{
			SetActorRotation(Velocity.Rotation());
		}
	}

	const float DistanceThisFrame = FVector::Dist(GetActorLocation(), StartLocation);
	TraveledDistance = DistanceThisFrame;
	if (Config.MaxTravelDistance > 0.f && TraveledDistance >= Config.MaxTravelDistance)
	{
		ReleaseToPoolOrDestroy();
	}
}

AProjectileBase* AProjectileBase::Spawn(UObject* WorldContextObject,
	TSubclassOf<AProjectileBase> ProjectileClass,
	const FTransform& SpawnTransform,
	const FProjectileConfig& InConfig,
	AActor* InOwner,
	APawn* InInstigator)
{
	if (UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
	{
		FActorSpawnParameters Params;
		Params.Owner = InOwner;
		Params.Instigator = InInstigator;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform, Params);
		if (Projectile)
		{
			Projectile->InitializeProjectile(InConfig);
			return Projectile;
		}
	}
	return nullptr;
}

void AProjectileBase::InitializeProjectile(const FProjectileConfig& InConfig)
{
	Config = InConfig;
	CollisionComponent->SetSphereRadius(Config.Radius);
	if (Config.Mesh)
	{
		MeshComponent->SetStaticMesh(Config.Mesh);
		MeshComponent->SetRelativeTransform(Config.MeshRelativeTransform);
	}
}

void AProjectileBase::StartLinear(const FVector& Direction)
{
	LinearDirection = Direction.GetSafeNormal();
	bIsActive = true;
	if (Config.bRotateToVelocity)
	{
		SetActorRotation(LinearDirection.Rotation());
	}
}

void AProjectileBase::StartParabola(const FVector& InitialVelocity)
{
	Velocity = InitialVelocity;
	bIsActive = true;
	if (Config.bRotateToVelocity)
	{
		SetActorRotation(Velocity.Rotation());
	}
}

void AProjectileBase::ReleaseToPoolOrDestroy()
{
	bIsActive = false;
	if (OwningPool.IsValid())
	{
		OwningPool.Get()->ReleaseProjectile(this);
	}
	else
	{
		Destroy();
	}
}

void AProjectileBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	// Attempt to apply damage if target has DamageHandlerComponent
	if (Config.DamageValue > 0)
	{
		if (UDamageHandlerComponent* DamageHandler = OtherActor->FindComponentByClass<UDamageHandlerComponent>())
		{
			FDamageInstance Instance;
			Instance.DamageValue = Config.DamageValue;
			TArray<FDamageInstance> Instances;
			Instances.Add(Instance);
			DamageHandler->TakeDamage(Instances);
		}
	}

	NumPenetrations++;
	if (Config.bDestroyOnHit && NumPenetrations > Config.MaxPenetrations)
	{
		ReleaseToPoolOrDestroy();
	}
}


