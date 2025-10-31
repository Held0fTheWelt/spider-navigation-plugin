// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "SpiderNavNodeBuilder.generated.h"

USTRUCT(BlueprintType)
struct FSpiderNavNodeBuilder
{
    GENERATED_BODY()

    FVector Location;
    FVector Normal;
    TArray<int32> Neighbors;
};

USTRUCT(BlueprintType)
struct FSpiderNavEdgeBuilder
{
    GENERATED_BODY()

    FVector Intersection;
    FVector Normal;
    int32 NodeA;
    int32 NodeB;
};
