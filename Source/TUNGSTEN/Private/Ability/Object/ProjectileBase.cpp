#include "Ability/Object/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameState/Helpers/ProjectilePoolComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(16.f);
    Collision->SetCollisionProfileName(TEXT("Projectile"));
    RootComponent = Collision;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Collision);

    //Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);

    Direction = FVector::ForwardVector;
    Speed = 1000.f;
    OwningPool = nullptr;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DeltaMove = Direction.GetSafeNormal() * Speed * DeltaTime;
    AddActorWorldOffset(DeltaMove, true);
}

void AProjectileBase::InitProjectile(const FVector& InDirection, float InSpeed, UProjectilePoolComponent* InPool)
{
    Direction = InDirection.GetSafeNormal();
    Speed = InSpeed;
    OwningPool = InPool;

    SetActorHiddenInGame(false);
    SetActorTickEnabled(true);
}

void AProjectileBase::Deactivate()
{
    OnProjectileDeactivate();

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

    if (OwningPool)
    {
        OwningPool->ReleaseProjectile(this);
    }
}