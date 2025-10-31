// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/SpiderNavNode.h"
#include "Structs/SavedSpiderNavGrid.h"
#include "SpiderNavigationSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SpiderNAVSubsystem_LOG, Log, All);
/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_API USpiderNavigationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FSavedSpiderNavGrid LoadGrid(FString GridSaveName, int32 GridIndex);

private:
	void AddGridNode(FSavedSpiderNavGrid& SavedGrid, int32 SavedIndex, FVector Location, FVector Normal);
	void SetGridNodeNeighbors(FSavedSpiderNavGrid& SavedGrid, int32 SavedIndex, TArray<int32> NeighborsSavedIndexes);

};
