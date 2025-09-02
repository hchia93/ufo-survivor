#include "Player/PlayerUFO.h"
#include "Ability/AbilityComponent.h"
#include "Stats/StatsRuntimeComponent.h"
#include "Stats/StatsLoaderComponent.h"
#include "TUNGSTEN/TUNGSTEN.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"

APlayerUFO::APlayerUFO()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create UFO body mesh
	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	BodyMeshComponent->SetupAttachment(RootComponent);
	BodyMeshComponent->PrimaryComponentTick.bCanEverTick = false;

	// Create beam mesh
	BeamMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMeshComponent"));
	BeamMeshComponent->SetupAttachment(BodyMeshComponent);
	BeamMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -100.0f));
	BeamMeshComponent->SetScalarParameterValueOnMaterials(FName("Opacity"), 0.0f);
	BeamMeshComponent->SetVisibility(false);
	BeamMeshComponent->PrimaryComponentTick.bCanEverTick = false;

	RuntimeStatsComponent = CreateDefaultSubobject<UStatsRuntimeComponent>(TEXT("RuntimeStatsComponent"));
	RuntimeStatsComponent->PrimaryComponentTick.bCanEverTick = false;

	StatsLoaderComponent = CreateDefaultSubobject<UStatsLoaderComponent>(TEXT("StatsLoaderComponent"));
	StatsLoaderComponent->PrimaryComponentTick.bCanEverTick = false;

	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	AbilityComponent->PrimaryComponentTick.bCanEverTick = true;
}

void APlayerUFO::BeginPlay()
{
	Super::BeginPlay();

	if (BeamMeshComponent && BeamMeshComponent->GetMaterial(0))
	{
		BeamMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	}
}

void APlayerUFO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update cooldown times
	if (DodgeRuntime.CooldownTime < DodgeConfig.Cooldown)
	{
		DodgeRuntime.CooldownTime += DeltaTime;
	}
}

void APlayerUFO::UpdateAimWithInputType(const FVector2D& AimInput, bool bIsGamepadInput)
{
	if (!BodyMeshComponent)
	{
		return;
	}

	if (bIsGamepadInput)
	{
		// Gamepad control: use 2D axis input method
		if (AimInput.Size() < AimConfig.MouseMoveDeadzone)
		{
			return;
		}

		float TargetYaw = FMath::Atan2(AimInput.X, AimInput.Y) * (180.0f / PI);
		TargetYaw = FMath::Fmod(TargetYaw + 360.0f, 360.0f);
		
		FRotator CurrentRot = BodyMeshComponent->GetComponentRotation();
		FRotator NewRot(CurrentRot.Pitch, TargetYaw, CurrentRot.Roll);
		BodyMeshComponent->SetWorldRotation(NewRot);
	}
	else
	{
		// Keyboard/Mouse control: use old GetHitResultUnderCursor method
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC)
		{
			return;
		}

		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		if (Hit.bBlockingHit)
		{
			const FVector SelfLoc = BodyMeshComponent->GetComponentLocation();
			FVector Target = Hit.Location;
			Target.Z = SelfLoc.Z;
			
			// Check if hit distance is too close to prevent "fighting"
			if (FVector::Dist2D(Target, SelfLoc) < AimConfig.MinMouseAimDistance)
			{
				return;
			}
			
			const FRotator LookRot = (Target - SelfLoc).Rotation();
			FRotator NewRot(0.0f, LookRot.Yaw + AimConfig.MouseYawOffset, 0.0f);
			BodyMeshComponent->SetWorldRotation(NewRot);
		}
	}
}


AActor* APlayerUFO::GetCurrentTarget() const
{
	return CurrentTarget;
}

// New control functions for Blueprint
void APlayerUFO::StartActionFloat()
{
	if (FloatRuntime.State != EUFOFloatState::None)
	{
		UE_LOG(LogTungsten, Warning, TEXT("Unable to request start ufo floating due to incorrect prerequisites state: %s"),
			*UEnum::GetValueAsString(FloatRuntime.State));
		return;
	}

	FloatRuntime.Time = 0.0f;
	FloatRuntime.StartZ = BodyMeshComponent->GetRelativeLocation().Z;
	FloatRuntime.State = EUFOFloatState::Starting;

	// Start ticking for the rise immediately
	GetWorldTimerManager().SetTimer(FloatRuntime.TimerHandle, this, &APlayerUFO::FloatingTick, 0.01f, true);

	// Call blueprint event
	OnFloatActionStarted();
}

void APlayerUFO::StopActionFloat()
{
	if (FloatRuntime.State != EUFOFloatState::Looping)
	{
		UE_LOG(LogTungsten, Warning, TEXT("Unable to request stop ufo floating due to incorrect prerequisites state: %s"),
			*UEnum::GetValueAsString(FloatRuntime.State));
		return;
	}

	// If beaming, cancel it first
	if (BeamRuntime.State != EUFOBeamState::None)
	{
		StopActionBeam();
	}

	FloatRuntime.Time = 0.0f;
	FloatRuntime.State = EUFOFloatState::Ending;

	// Call blueprint event
	OnFloatActionStopped();
}

void APlayerUFO::StartActionBeam()
{
	if (FloatRuntime.State != EUFOFloatState::Looping || BeamRuntime.State != EUFOBeamState::None)
	{
		UE_LOG(LogTungsten, Warning, TEXT("Unable to request start ufo beaming due to incorrect prerequisites state - Float: %s, Beam: %s"),
			*UEnum::GetValueAsString(FloatRuntime.State), *UEnum::GetValueAsString(BeamRuntime.State));
		return;
	}

	BeamMeshComponent->SetVisibility(true);
	BeamRuntime.State = EUFOBeamState::Starting;
	BeamRuntime.Time = 0.0f;
	BeamRuntime.OscTime = 0.0f;
	GetWorldTimerManager().SetTimer(BeamRuntime.TimerHandle, this, &APlayerUFO::BeamingTick, 0.01f, true);

	// Call blueprint event
	OnBeamActionStarted();
}

void APlayerUFO::StopActionBeam()
{
	if (BeamRuntime.State != EUFOBeamState::Looping && BeamRuntime.State != EUFOBeamState::Starting)
	{
		UE_LOG(LogTungsten, Warning, TEXT("Unable to request stop ufo beaming due to incorrect prerequisites state: %s"),
			*UEnum::GetValueAsString(BeamRuntime.State));
		return;
	}

	BeamRuntime.State = EUFOBeamState::Ending;
	BeamRuntime.Time = 0.0f;

	// Call blueprint event
	OnBeamActionStopped();
}

void APlayerUFO::StartActionDodge()
{
	if (DodgeRuntime.State != EUFODodgeState::None)
	{
		UE_LOG(LogTungsten, Warning, TEXT("Unable to request start ufo dodge due to incorrect prerequisites state: %s"),
			*UEnum::GetValueAsString(DodgeRuntime.State));
		return;
	}

	// Check cooldown
	if (DodgeRuntime.CooldownTime < DodgeConfig.Cooldown)
	{
		UE_LOG(LogTungsten, Warning, TEXT("Unable to request start ufo dodge due to cooldown: %.2f < %.2f"),
			DodgeRuntime.CooldownTime, DodgeConfig.Cooldown);
		return;
	}

	// Store starting location and time
	DodgeRuntime.StartLocation = GetActorLocation();
	DodgeRuntime.Time = 0.0f;
	DodgeRuntime.State = EUFODodgeState::Starting;

	// Start ticking for the dodge immediately
	GetWorldTimerManager().SetTimer(DodgeRuntime.TimerHandle, this, &APlayerUFO::DodgingTick, 0.01f, true);

	// Call blueprint event
	OnDodgeActionStarted();
}

void APlayerUFO::FloatingTick()
{
	FVector CurrentLocation = BodyMeshComponent->GetRelativeLocation();
	float ZOffset = 0.0f;

	switch (FloatRuntime.State)
	{
	case EUFOFloatState::Starting:
	{
		FloatRuntime.Time += 0.01f;
		float Alpha = FMath::Clamp(FloatRuntime.Time / FloatConfig.RiseTime, 0.f, 1.f);
		float EasedAlpha = EaseInOut(Alpha);
		ZOffset = FMath::Lerp(0.f, FloatConfig.Height, EasedAlpha);

		if (Alpha >= 1.0f)
		{
			FloatRuntime.State = EUFOFloatState::Looping; // Remain in loop until StopActionFloat is called
		}
		break;
	}
	case EUFOFloatState::Looping:
	{
		// Stay at max height, do nothing unless StopActionFloat is called
		ZOffset = FloatConfig.Height;
		break;
	}
	case EUFOFloatState::Ending:
	{
		FloatRuntime.Time += 0.01f;
		float CurrentZ = BodyMeshComponent->GetRelativeLocation().Z;
		float Alpha = FMath::Clamp(FloatRuntime.Time / FloatConfig.SinkTime, 0.f, 1.f);
		float EasedAlpha = EaseInOut(Alpha);
		ZOffset = FMath::Lerp(CurrentZ - FloatRuntime.StartZ, 0.f, EasedAlpha);

		if (Alpha >= 1.0f)
		{
			GetWorldTimerManager().ClearTimer(FloatRuntime.TimerHandle);
			FloatRuntime.State = EUFOFloatState::None;
		}
		break;
	}
	default:
		break;
	}

	// Always add ZOffset to the current X,Y,Z (so X,Y can move during float)
	if (FloatRuntime.State == EUFOFloatState::Starting || FloatRuntime.State == EUFOFloatState::Looping || FloatRuntime.State == EUFOFloatState::Ending)
	{
		FVector NewLocation = CurrentLocation;
		NewLocation.Z = FloatRuntime.StartZ + ZOffset;
		BodyMeshComponent->SetRelativeLocation(NewLocation);
	}
}

void APlayerUFO::BeamingTick()
{
	float Value = 0.0f;

	switch (BeamRuntime.State)
	{
	case EUFOBeamState::Starting:
	{
		BeamRuntime.Time += 0.01f;
		float Alpha = FMath::Clamp(BeamRuntime.Time / BeamConfig.StartTime, 0.f, 1.f);
		Value = FMath::Lerp(0.f, 1.f, EaseInOut(Alpha));

		if (Alpha >= 1.0f)
		{
			BeamRuntime.State = EUFOBeamState::Looping;
			BeamRuntime.Time = 0.0f;
			BeamRuntime.OscTime = 0.0f;
		}
		break;
	}
	case EUFOBeamState::Looping:
	{
		BeamRuntime.OscTime += 0.01f;
		Value = 0.9f + BeamConfig.OscRange * FMath::Sin(BeamRuntime.OscTime * BeamConfig.OscSpeed * 2.0f * PI);
		break;
	}
	case EUFOBeamState::Ending:
	{
		BeamRuntime.Time += 0.01f;
		// Fade out from current value to 0
		float Alpha = FMath::Clamp(BeamRuntime.Time / BeamConfig.EndTime, 0.f, 1.f);
		Value = FMath::Lerp(BeamRuntime.Alpha, 0.f, EaseInOut(Alpha));

		if (Alpha >= 1.0f)
		{
			GetWorldTimerManager().ClearTimer(BeamRuntime.TimerHandle);
			BeamRuntime.State = EUFOBeamState::None;
			BeamMeshComponent->SetVisibility(false);
		}
		break;
	}
	}

	BeamRuntime.Alpha = Value;
	BeamMeshComponent->SetScalarParameterValueOnMaterials(FName("Opacity"), Value);
}

void APlayerUFO::DodgingTick()
{
	DodgeRuntime.Time += 0.01f;
	FVector CurrentLocation = GetActorLocation();

	switch (DodgeRuntime.State)
	{
	case EUFODodgeState::Starting:
	{
		// Brief start phase for visual effects
		if (DodgeRuntime.Time >= 0.05f) // 50ms start phase
		{
			DodgeRuntime.State = EUFODodgeState::Executing;
			DodgeRuntime.Time = 0.0f;
		}
		break;
	}
	case EUFODodgeState::Executing:
	{
		// Calculate dodge movement
		float Alpha = FMath::Clamp(DodgeRuntime.Time / DodgeConfig.Duration, 0.f, 1.f);
		float EasedAlpha = EaseInOut(Alpha);
		
		// Get forward direction from BodyMeshComponent's world rotation for dodge
		// Use GetRightVector() to get the correct forward direction (X-axis)
		FVector DodgeDirection = BodyMeshComponent->GetRightVector();
		
		// Calculate target location
		FVector TargetLocation = DodgeRuntime.StartLocation + (DodgeDirection * DodgeConfig.Distance);
		
		// Interpolate position
		FVector NewLocation = FMath::Lerp(DodgeRuntime.StartLocation, TargetLocation, EasedAlpha);
		NewLocation.Z = CurrentLocation.Z; // Keep current Z to maintain height
		
		// Set new location
		SetActorLocation(NewLocation);

		if (Alpha >= 1.0f)
		{
			DodgeRuntime.State = EUFODodgeState::Ending;
			DodgeRuntime.Time = 0.0f;
		}
		break;
	}
	case EUFODodgeState::Ending:
	{
		// Brief end phase for visual effects
		if (DodgeRuntime.Time >= 0.05f) // 50ms end phase
		{
			GetWorldTimerManager().ClearTimer(DodgeRuntime.TimerHandle);
			DodgeRuntime.State = EUFODodgeState::None;
			DodgeRuntime.CooldownTime = 0.0f;
			
			// Call blueprint event
			OnDodgeActionCompleted();
		}
		break;
	}
	}
}

float APlayerUFO::EaseInOut(float t)
{
	return t < 0.5f
		? 2.f * t * t
		: -1.f + (4.f - 2.f * t) * t;
}

bool APlayerUFO::CanDrop() const
{
	return BeamRuntime.State == EUFOBeamState::None;
}
