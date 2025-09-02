#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectilePoolComponent;

UCLASS()
class TUNGSTEN_API AProjectileBase : public AActor
{
    GENERATED_BODY()

public:
    AProjectileBase();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void InitProjectile(const FVector& InDirection, float InSpeed, UProjectilePoolComponent* InPool);

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void Deactivate();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
    void OnProjectileDeactivate();

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    FVector Direction;
    float Speed;

    UPROPERTY()
    UProjectilePoolComponent* OwningPool;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* Collision;
};
