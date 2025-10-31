#pragma once

#include "Structs/SpiderNavNode.h"
#include "SavedSpiderNavGrid.generated.h"

USTRUCT(BlueprintType)
struct FSavedSpiderNavGrid
{
    GENERATED_BODY()

public:
	TArray<FSpiderNavNode> NavNodes;
	// SavedIndex -> LocalIndex
	TMap<int32, int32> NodesSavedIndexes;

	int GetNavNodesCount() const
	{
		return NavNodes.Num();
	}
};