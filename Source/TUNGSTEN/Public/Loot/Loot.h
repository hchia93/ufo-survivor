#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Loot.generated.h"

class AActor;

/*
 *	FLootParam : 
 *	- Defines loot parameters for dropped items and experience
 *	- Used when actors die to determine what they drop
 */
USTRUCT(BlueprintType)
struct TUNGSTEN_API FLootParam
{
	GENERATED_BODY()

public:
	/// Experience dropped on death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 DroppedExp = 0;

	/// Array of items dropped on death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<TSubclassOf<AActor>> DroppedItems;

public:
	FLootParam()
	{
		DroppedExp = 0;
		DroppedItems.Empty();
	}

	FLootParam(int32 Exp, const TArray<TSubclassOf<AActor>>& Items)
	{
		DroppedExp = Exp;
		DroppedItems = Items;
	}
};
