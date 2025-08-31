#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EProjectileMovementMode : uint8
{
	Linear,
	Parabola
};

USTRUCT(BlueprintType)
struct TUNGSTEN_API FProjectileConfig
{
	GENERATED_BODY()

	// Visual
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Visual")
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Visual")
	FTransform MeshRelativeTransform = FTransform::Identity;

	// Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Collision")
	float Radius = 8.0f;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	EProjectileMovementMode MovementMode = EProjectileMovementMode::Linear;

	// Linear: Unit direction will be provided at start; Parabola: Initial velocity will be provided at start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	float Speed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	float GravityZ = -980.0f;

	// Lifetime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Lifetime")
	float MaxTravelDistance = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Lifetime")
	bool bDestroyOnHit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Lifetime")
	int32 MaxPenetrations = 0; // 0 = no penetration, destroy on first hit if bDestroyOnHit is true

	// Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Damage")
	int32 DamageValue = 0;

	// Behavior
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	bool bRotateToVelocity = true;
};

UCLASS()
class TUNGSTEN_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile|Spawn")
	static AProjectileBase* Spawn(UObject* WorldContextObject,
		TSubclassOf<AProjectileBase> ProjectileClass,
		const FTransform& SpawnTransform,
		const FProjectileConfig& InConfig,
		AActor* InOwner,
		APawn* InInstigator);

	UFUNCTION(BlueprintCallable, Category = "Projectile|Init")
	void InitializeProjectile(const FProjectileConfig& InConfig);

	// Start movement
	UFUNCTION(BlueprintCallable, Category = "Projectile|Movement")
	void StartLinear(const FVector& Direction);

	UFUNCTION(BlueprintCallable, Category = "Projectile|Movement")
	void StartParabola(const FVector& InitialVelocity);

	UFUNCTION(BlueprintCallable, Category = "Projectile|Lifetime")
	void ReleaseToPoolOrDestroy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// Config and runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FProjectileConfig Config;

	// Runtime state
	FVector LinearDirection = FVector::ForwardVector; // Unit
	FVector Velocity = FVector::ZeroVector;
	FVector StartLocation = FVector::ZeroVector;
	float TraveledDistance = 0.0f;
	int32 NumPenetrations = 0;
	bool bIsActive = false;

public:
	// Internal hook for pool – if set, Release will use pool instead of Destroy
	TWeakObjectPtr<class UProjectilePoolComponent> OwningPool;
};


