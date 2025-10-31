// @Yves Tanas 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpiderNavigationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpiderNavigationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPIDERNAVIGATION_API ISpiderNavigationInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Search")
	FVector FindClosestNodeLocation(FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spiders")
	TArray<class AActor*> GetAllSpiderNavGridsActors();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spiders")
	void RegisterSpiderNavGridActor(class AActor* SpiderNavGridActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spiders")
	void UnRegisterSpiderNavGridActor(class AActor* SpiderNavGridActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spiders")
	TArray<FVector> FindPathBetweenPoints(FVector StartLocation, FVector EndLocation);
};
