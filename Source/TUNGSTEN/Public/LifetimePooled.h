#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LifetimePooled.generated.h"

UINTERFACE(BlueprintType)
class ULifetimePooled : public UInterface
{
	GENERATED_BODY()
};

class ILifetimePooled
{
	GENERATED_BODY()

public:
	// Called by pool when the object is checked out
	UFUNCTION(BlueprintNativeEvent, Category = "Pool")
	void OnPoolAcquire();

	// Called by pool when the object is returned
	UFUNCTION(BlueprintNativeEvent, Category = "Pool")
	void OnPoolRelease();
};


