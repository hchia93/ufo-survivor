#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatsDefine.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	Alive,
	Dead
};

UENUM(BlueprintType)
enum class EStatModifierType : uint8
{
	Additive,      // Adds to base stat
	Multiplicative, // Multiplies base stat
	Temporary      // Temporary modifier that can be removed
};

USTRUCT(BlueprintType)
struct TUNGSTEN_API FCharacterStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DEF;

	FCharacterStats()
	{
		HP = 0;
		ATK = 0;
		DEF = 0;
	}

	FCharacterStats(int32 InHP, int32 InATK, int32 InDEF)
	{
		HP = InHP;
		ATK = InATK;
		DEF = InDEF;
	}

	// Operator overloads for stat calculations
	FCharacterStats operator+(const FCharacterStats& Other) const
	{
		return FCharacterStats(HP + Other.HP, ATK + Other.ATK, DEF + Other.DEF);
	}

	FCharacterStats operator-(const FCharacterStats& Other) const
	{
		return FCharacterStats(HP - Other.HP, ATK - Other.ATK, DEF - Other.DEF);
	}

	FCharacterStats& operator+=(const FCharacterStats& Other)
	{
		HP += Other.HP;
		ATK += Other.ATK;
		DEF += Other.DEF;
		return *this;
	}

	FCharacterStats& operator-=(const FCharacterStats& Other)
	{
		HP -= Other.HP;
		ATK -= Other.ATK;
		DEF -= Other.DEF;
		return *this;
	}
};

USTRUCT(BlueprintType)
struct TUNGSTEN_API FDamageInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageValue;

	FDamageInstance()
	{
		DamageValue = 0;
	}

	FDamageInstance(int32 InDamageValue)
	{
		DamageValue = InDamageValue;
	}
};

// Stat modifier source for tracking and removal
USTRUCT(BlueprintType)
struct TUNGSTEN_API FStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ModifierID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatModifierType ModifierType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStats StatModification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration; // 0.0f = permanent

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeRemaining;

	FStatModifier()
	{
		ModifierID = TEXT("");
		ModifierType = EStatModifierType::Additive;
		StatModification = FCharacterStats();
		Duration = 0.0f;
		TimeRemaining = 0.0f;
	}

	FStatModifier(const FString& InModifierID, EStatModifierType InType, const FCharacterStats& InModification, float InDuration = 0.0f)
	{
		ModifierID = InModifierID;
		ModifierType = InType;
		StatModification = InModification;
		Duration = InDuration;
		TimeRemaining = InDuration;
	}
};
