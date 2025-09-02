#include "Ability/AbilityCooldownHandler.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

UAbilityCooldownHandler::UAbilityCooldownHandler()
{
	CooldownTime = 1.0f;
	bIsInCooldown = false;
}

bool UAbilityCooldownHandler::IsCooldownReady() const
{
	return !bIsInCooldown;
}

float UAbilityCooldownHandler::GetCooldownPercent(AActor* OwnerActor) const
{
	if (!bIsInCooldown || !IsValid(OwnerActor))
	{
		return 1.0f;
	}

	if (OwnerActor->GetWorldTimerManager().IsTimerActive(CooldownTimerHandle))
	{
		float TimerRemaining = OwnerActor->GetWorldTimerManager().GetTimerRemaining(CooldownTimerHandle);
		return 1.0f - (TimerRemaining / CooldownTime);
	}

	return 1.0f;
}

void UAbilityCooldownHandler::EnterCooldown(AActor* OwnerActor)
{
	if (!IsValid(OwnerActor))
	{
		return;
	}

	bIsInCooldown = true;
	OnEnterCooldown();
	OnEnterCooldownEvent.Broadcast();

	OwnerActor->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityCooldownHandler::HandleExitCooldown, CooldownTime, false);
}

void UAbilityCooldownHandler::ResetCooldown(AActor* OwnerActor)
{
	if (!IsValid(OwnerActor))
	{
		return;
	}

	OwnerActor->GetWorldTimerManager().ClearTimer(CooldownTimerHandle);

	HandleExitCooldown();
}

void UAbilityCooldownHandler::HandleExitCooldown()
{
	bIsInCooldown = false;
	OnExitCooldown();
	OnExitCooldownEvent.Broadcast();
}
