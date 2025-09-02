#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerUFO.generated.h"

class UAbilityComponent;
class UStatsRuntimeComponent;
class UStatsLoaderComponent;
class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class EUFOModelType : uint8
{
	TypeA,
	TypeB,
	TypeC,
	TypeD
};

UENUM(BlueprintType)
enum class EUFOModelAction : uint8
{
	None,
	ShowWeapon,
};

UENUM(BlueprintType)
enum class EUFOBeamType : uint8
{
	None,
	Beam,
	BurstBeam
};

UENUM(BlueprintType)
enum class EUFOFloatState : uint8
{
	None,
	Starting,
	Looping,
	Ending
};

UENUM(BlueprintType)
enum class EUFOBeamState : uint8
{
	None,
	Starting,
	Looping,
	Ending
};

UENUM(BlueprintType)
enum class EUFODodgeState : uint8
{
	None,
	Starting,
	Executing,
	Ending
};

USTRUCT(BlueprintType)
struct FUFOFloatActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Float Settings")
	float Height = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Float Settings")
	float RiseTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Float Settings")
	float SinkTime = 1.0f;

	FUFOFloatActionConfig()
	{
		Height = 100.0f;
		RiseTime = 1.0f;
		SinkTime = 1.0f;
	}
};

USTRUCT(BlueprintType)
struct FUFOBeamActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Beam Settings")
	float StartTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Beam Settings")
	float EndTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Beam Settings")
	float OscSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Beam Settings")
	float OscRange = 0.1f;

	FUFOBeamActionConfig()
	{
		StartTime = 1.0f;
		EndTime = 1.0f;
		OscSpeed = 2.0f;
		OscRange = 0.1f;
	}
};

USTRUCT(BlueprintType)
struct FUFOAimConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Control")
	float MouseYawOffset = -90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Control")
	float MinMouseAimDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Control")
	float MouseMoveDeadzone = 5.0f;
};

USTRUCT(BlueprintType)
struct FUFOActionDodgeConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Dodge Settings")
	float Distance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Dodge Settings")
	float Duration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Dodge Settings")
	float Cooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Dodge Settings")
	bool bInvulnerableDuringDodge = true;

	FUFOActionDodgeConfig()
	{
		Distance = 300.0f;
		Duration = 0.3f;
		Cooldown = 0.5f;
		bInvulnerableDuringDodge = true;
	}
};

USTRUCT(BlueprintType)
struct FUFOFloatActionRuntime
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Float Runtime")
	EUFOFloatState State = EUFOFloatState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Float Runtime")
	float Time = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Float Runtime")
	float StartZ = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Float Runtime")
	FTimerHandle TimerHandle;

	FUFOFloatActionRuntime()
	{
		State = EUFOFloatState::None;
		Time = 0.0f;
		StartZ = 0.0f;
	}
};

USTRUCT(BlueprintType)
struct FUFOBeamActionRuntime
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Beam Runtime")
	EUFOBeamState State = EUFOBeamState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Beam Runtime")
	float Time = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Beam Runtime")
	float OscTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Beam Runtime")
	float Alpha = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Beam Runtime")
	FTimerHandle TimerHandle;

	FUFOBeamActionRuntime()
	{
		State = EUFOBeamState::None;
		Time = 0.0f;
		OscTime = 0.0f;
		Alpha = 0.0f;
	}
};

USTRUCT(BlueprintType)
struct FUFODodgeActionRuntime
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Dodge Runtime")
	EUFODodgeState State = EUFODodgeState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Dodge Runtime")
	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Dodge Runtime")
	float Time = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Dodge Runtime")
	float CooldownTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Dodge Runtime")
	FTimerHandle TimerHandle;

	FUFODodgeActionRuntime()
	{
		State = EUFODodgeState::None;
		StartLocation = FVector::ZeroVector;
		Time = 0.0f;
		CooldownTime = 0.0f;
	}
};

UCLASS()
class TUNGSTEN_API APlayerUFO : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerUFO();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "TUNGSTEN")
	AActor* GetCurrentTarget() const;

	UFUNCTION(BlueprintCallable, Category = "UFO Control")
	void UpdateAimWithInputType(const FVector2D& AimInput, bool bIsGamepadInput);

	// New control functions for Blueprint
	UFUNCTION(BlueprintCallable, Category = "UFO Control")
	void StartActionFloat();

	UFUNCTION(BlueprintCallable, Category = "UFO Control")
	void StopActionFloat();

	UFUNCTION(BlueprintCallable, Category = "UFO Control")
	void StartActionBeam();

	UFUNCTION(BlueprintCallable, Category = "UFO Control")
	void StopActionBeam();

	UFUNCTION(BlueprintCallable, Category = "UFO Control")
	void StartActionDodge();

	// Blueprint implementable events (Blueprint-only)
	UFUNCTION(BlueprintImplementableEvent, Category = "UFO Events")
	void OnFloatActionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "UFO Events")
	void OnFloatActionStopped();

	UFUNCTION(BlueprintImplementableEvent, Category = "UFO Events")
	void OnBeamActionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "UFO Events")
	void OnBeamActionStopped();

	UFUNCTION(BlueprintImplementableEvent, Category = "UFO Events")
	void OnDodgeActionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "UFO Events")
	void OnDodgeActionCompleted();

protected:
	// Camera components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	// Main body mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BeamMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatsRuntimeComponent> RuntimeStatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatsLoaderComponent> StatsLoaderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilityComponent> AbilityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	EUFOModelAction ModelActionType = EUFOModelAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	EUFOModelType ModelType = EUFOModelType::TypeA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	EUFOBeamType BeamType = EUFOBeamType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Settings")
	FUFOFloatActionConfig FloatConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Settings")
	FUFOBeamActionConfig BeamConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Settings")
	FUFOActionDodgeConfig DodgeConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Control")
	FUFOAimConfig AimConfig;

	UFUNCTION(BlueprintCallable, Category = "UFO Actions")
	bool CanDrop() const;

protected:
	void FloatingTick();
	void BeamingTick();
	void DodgingTick();
	float EaseInOut(float t);

	// Runtime structs for state management
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Runtime")
	FUFOFloatActionRuntime FloatRuntime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Runtime")
	FUFOBeamActionRuntime BeamRuntime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFO Runtime")
	FUFODodgeActionRuntime DodgeRuntime;

};
