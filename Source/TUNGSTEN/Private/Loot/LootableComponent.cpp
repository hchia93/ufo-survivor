#include "Loot/LootableComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"

ULootableComponent::ULootableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULootableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCollectionSphere();
}

void ULootableComponent::SetupCollectionSphere()
{
	// Create collection sphere if it doesn't exist
	if (!CollectionSphere)
	{
		CollectionSphere = NewObject<USphereComponent>(GetOwner(), TEXT("CollectionSphere"));
		CollectionSphere->RegisterComponent();
		CollectionSphere->SetupAttachment(GetOwner()->GetRootComponent());
	}

	// Configure collection sphere
	CollectionSphere->SetSphereRadius(CollectionRadius);
	CollectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ULootableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULootableComponent::TriggerLoot(AActor* Looter)
{
	if (Looter)
	{
		OnLooted.Broadcast(Looter);
	}
}
