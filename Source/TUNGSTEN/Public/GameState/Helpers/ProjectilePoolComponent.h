#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectilePoolComponent.generated.h"

class AProjectileBase;

USTRUCT(BlueprintType)
struct FProjectileClassPool
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AProjectileBase> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InitialSize = 8;
};

// Bucket that holds free/used arrays for a given class
USTRUCT()
struct FProjectilePoolBucket
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<AProjectileBase>> Free;

    UPROPERTY()
    TArray<TObjectPtr<AProjectileBase>> Used;
};

UCLASS(ClassGroup=(GameState), meta=(BlueprintSpawnableComponent))
class TUNGSTEN_API UProjectilePoolComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UProjectilePoolComponent();

    UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
    void ConfigurePools(const TArray<FProjectileClassPool>& InPools);

    UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
    AProjectileBase* AcquireProjectile(TSubclassOf<AProjectileBase> ProjectileClass, const FTransform& SpawnTransform);

    UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
    void ReleaseProjectile(AProjectileBase* Projectile);

protected:
    virtual void BeginPlay() override;

private:
    UWorld* GetWorldChecked() const;

private:
    // Class -> bucket of free/used instances
    UPROPERTY()
    TMap<TSubclassOf<AProjectileBase>, FProjectilePoolBucket> ClassToBuckets;

    UPROPERTY(EditAnywhere, Category = "Projectile Pool")
    TArray<FProjectileClassPool> PoolConfigs;
};
