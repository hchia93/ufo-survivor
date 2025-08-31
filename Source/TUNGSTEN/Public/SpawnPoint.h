#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.generated.h"

class AActor;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class ESpawnPointType : uint8
{
	Round,
	Square,
	NormalTiledRound,
	NormalTiledSquare,
	NormalTiledRoundEnd,
	NormalTiledSquareEnd,
	SnowTiledRound,
	SnowTiledSquare,
	SnowTiledRoundEnd,
	SnowTiledSquareEnd
};

UCLASS()
class TUNGSTEN_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ASpawnPoint();

	UFUNCTION(BlueprintCallable, Category = "Materials")
	void ApplyMaterial(int32 slot);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TUNGSTEN")
	ESpawnPointType SpawnPointType = ESpawnPointType::Round;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TUNGSTEN")
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TUNGSTEN")
	int32 UseMaterialSlotIndex = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;
};