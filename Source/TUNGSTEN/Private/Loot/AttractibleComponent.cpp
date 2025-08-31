#include "Loot/AttractibleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

UAttractibleComponent::UAttractibleComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // No tick needed, using timer-based updates

	// Create attraction sphere
	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphere"));
	AttractionSphere->SetSphereRadius(AttractionRadius);
	AttractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UAttractibleComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupAttractionSphere();
}

void UAttractibleComponent::SetupAttractionSphere()
{
	if (AttractionSphere)
	{
		// Bind overlap event
		AttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &UAttractibleComponent::OnAttractionSphereOverlap);
		AttractionSphere->OnComponentEndOverlap.AddDynamic(this, &UAttractibleComponent::OnAttractionSphereEndOverlap);
	}
}

void UAttractibleComponent::OnAttractionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bEnableAttraction || !OtherActor)
	{
		return;
	}

	// Check if this is a valid player
	if (IsValidPlayer(OtherActor))
	{
		if (!bIsAttracting)
		{
			StartAttraction(OtherActor);
		}
	}
	else
	{
		// If the overlapping actor is no longer valid, stop attraction
		if (bIsAttracting && TargetPlayer == OtherActor)
		{
			StopAttraction();
		}
	}
}

void UAttractibleComponent::OnAttractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bEnableAttraction || !OtherActor)
	{
		return;
	}

	// If the overlapping actor is no longer valid, stop attraction
	if (bIsAttracting && TargetPlayer == OtherActor)
	{
		StopAttraction();
	}
}

void UAttractibleComponent::StartAttraction(AActor* Player)
{
	if (bIsAttracting || !Player)
	{
		return;
	}

	bIsAttracting = true;
	TargetPlayer = Player;

	// Call blueprint event
	OnAttractionStarted();

	// Start attraction update timer (update every frame equivalent but without tick)
	GetWorld()->GetTimerManager().SetTimer(AttractionTimerHandle, this, &UAttractibleComponent::UpdateAttraction, 1.0f / 60.0f, true);
}

void UAttractibleComponent::StopAttraction()
{
	if (!bIsAttracting)
	{
		return;
	}

	bIsAttracting = false;
	TargetPlayer = nullptr;

	// Stop attraction timer
	GetWorld()->GetTimerManager().ClearTimer(AttractionTimerHandle);

	// Call blueprint event
	OnAttractionCompleted();
}

void UAttractibleComponent::UpdateAttraction()
{
	if (!bIsAttracting || !TargetPlayer || !GetOwner())
	{
		StopAttraction();
		return;
	}

	// Check if target is still valid
	if (!IsValid(TargetPlayer) || !IsValidPlayer(TargetPlayer))
	{
		StopAttraction();
		return;
	}

	AActor* Owner = GetOwner();
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector TargetLocation = TargetPlayer->GetActorLocation();
	FVector Direction = (TargetLocation - OwnerLocation).GetSafeNormal();
	float Distance = FVector::Dist(OwnerLocation, TargetLocation);

	// If we're close enough, stop attraction
	if (Distance <= MinDistanceToPlayer)
	{
		StopAttraction();
		return;
	}

	// Move towards target
	FVector NewLocation = OwnerLocation + (Direction * AttractionSpeed * (1.0f / 60.0f));
	Owner->SetActorLocation(NewLocation, true);
}

bool UAttractibleComponent::IsValidPlayer(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}

	// Check if it's a player-controlled pawn
	APawn* Pawn = Cast<APawn>(Actor);
	if (Pawn && Pawn->IsPlayerControlled())
	{
		return true;
	}

	// Check if it's a player controller
	APlayerController* PC = Cast<APlayerController>(Actor);
	if (PC)
	{
		return true;
	}

	return false;
}
