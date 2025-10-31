// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree/BehaviorTreeTypes.h" 
#include "BehaviorTreeHelpers.generated.h"

/**
 * 
 */
UCLASS(abstract)
class SPIDERNAVIGATION_API UBehaviorTreeHelpers : public UObject
{
	GENERATED_BODY()
	
public:
	static FString AbortModeToText(EBTFlowAbortMode::Type Mode)
	{
		switch (Mode)
		{
		case EBTFlowAbortMode::None:          return TEXT("");                     // oder TEXT("(no aborts)")
		case EBTFlowAbortMode::Self:          return TEXT("(aborts self)");
		case EBTFlowAbortMode::LowerPriority: return TEXT("(aborts lower priority)");
		case EBTFlowAbortMode::Both:          return TEXT("(aborts both)");
		default:                               return TEXT("");
		}
	}
};
