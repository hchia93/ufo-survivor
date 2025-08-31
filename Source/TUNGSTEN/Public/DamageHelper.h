// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DamageHelper.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EUnitLiveState : uint8
{
	Alive,
	Dead
};

USTRUCT(BlueprintType)
struct TUNGSTEN_API FUnitStats : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DEF;
};

USTRUCT(BlueprintType)
struct TUNGSTEN_API FDamageInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageValue;
};

