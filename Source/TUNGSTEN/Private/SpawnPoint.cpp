#include "SpawnPoint.h"
#include "Components/StaticMeshComponent.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	SpawnPointType = ESpawnPointType::Round;
}

void ASpawnPoint::ApplyMaterial(int32 slot)
{
	if (MeshComponent != nullptr)
	{
		if (Materials.IsValidIndex(slot) && Materials[slot] != nullptr)
		{
			MeshComponent->SetMaterial(0, Materials[slot]);
		}
	}
}