#include "Loot/ExperienceLoot.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Loot/LootableComponent.h"
#include "Loot/AttractibleComponent.h"

AExperienceLoot::AExperienceLoot()
{
	PrimaryActorTick.bCanEverTick = false; // No more tick needed

	// Create lootable component first (it will handle CollectionSphere)
	LootableComponent = CreateDefaultSubobject<ULootableComponent>(TEXT("LootableComponent"));

	// Create experience widget
	ExperienceWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ExperienceWidget"));
	ExperienceWidget->SetupAttachment(RootComponent);
	ExperienceWidget->SetRelativeLocation(WidgetOffset);
	ExperienceWidget->SetWidgetSpace(EWidgetSpace::World);
	ExperienceWidget->SetDrawSize(FVector2D(100.0f * WidgetScale, 100.0f * WidgetScale));

	// Create attractible component
	AttractibleComponent = CreateDefaultSubobject<UAttractibleComponent>(TEXT("AttractibleComponent"));

	// Bind overlap event to LootableComponent's CollectionSphere
	if (LootableComponent && LootableComponent->CollectionSphere)
	{
		LootableComponent->CollectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AExperienceLoot::OnActorOverlap);
	}
}

void AExperienceLoot::BeginPlay()
{
	Super::BeginPlay();

	SetupExperienceWidget();
	
	// Bind OnLoot event to HandleExperienceCollection
	if (LootableComponent)
	{
		LootableComponent->OnLooted.AddDynamic(this, &AExperienceLoot::HandleExperienceCollection);
		
		// Bind overlap event after LootableComponent has created CollectionSphere
		if (LootableComponent->CollectionSphere)
		{
			LootableComponent->CollectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AExperienceLoot::OnActorOverlap);
		}
	}
}

void AExperienceLoot::SetupExperienceWidget()
{
	if (ExperienceWidgetClass && ExperienceWidget)
	{
		ExperienceWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ExperienceWidgetClass);
		if (ExperienceWidgetInstance)
		{
			ExperienceWidget->SetWidget(ExperienceWidgetInstance);
		}
	}
}

void AExperienceLoot::UpdateWidgetRotation()
{
	if (!ExperienceWidget)
	{
		return;
	}

	// Find player and rotate widget to face them
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->GetPawn())
	{
		FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
		FVector Direction = (PlayerLocation - GetActorLocation()).GetSafeNormal();
		FRotator LookRotation = Direction.Rotation();
		
		// Keep widget upright but face player
		LookRotation.Pitch = 0.0f;
		LookRotation.Roll = 0.0f;
		
		ExperienceWidget->SetWorldRotation(LookRotation);
	}
}

void AExperienceLoot::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCollected || !LootableComponent || !LootableComponent->ShouldAutoCollect())
	{
		return;
	}

	// Check if overlapping actor is a player
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn && Pawn->IsPlayerControlled())
	{
		// Trigger loot event
		LootableComponent->TriggerLoot(Pawn);
	}
}

void AExperienceLoot::HandleExperienceCollection(AActor* Looter)
{
	if (bIsCollected)
	{
		return;
	}

	bIsCollected = true;
	OnExperienceCollected(ExperienceValue);
	
	// Return to pool instead of destroying
	ReturnToPool();
}

void AExperienceLoot::ReturnToPool()
{
	// Reset collection state
	bIsCollected = false;
	
	// Hide and disable collision temporarily
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
	// Move to hidden location
	SetActorLocation(FVector(0, 0, -10000));
	
	// Reset widget if exists
	if (ExperienceWidgetInstance)
	{
		ExperienceWidgetInstance->RemoveFromParent();
		ExperienceWidgetInstance = nullptr;
	}
	
	// This will be handled by the pool system
	// The pool will call ReturnExperienceLoot when needed
}
