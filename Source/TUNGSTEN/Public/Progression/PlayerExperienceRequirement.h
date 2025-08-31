#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerExperienceRequirement.generated.h"

USTRUCT(BlueprintType)
struct TUNGSTEN_API FPlayerExperienceRequirement : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int32 RequiredEXP = 0; // Renamed from TotalExperienceRequired

public:
	FPlayerExperienceRequirement()
	{
		Level = 1;
		RequiredEXP = 0;
	}
};
