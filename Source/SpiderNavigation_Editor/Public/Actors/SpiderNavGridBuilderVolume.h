// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpiderNavGridBuilderVolume.generated.h"

UCLASS()
class SPIDERNAVIGATION_EDITOR_API ASpiderNavGridBuilderVolume : public AActor
{
	GENERATED_BODY()

public:
	ASpiderNavGridBuilderVolume();

	/** Volume where to build navigation */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UBoxComponent* VolumeBox;

};
