// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include <algorithm>
#include <vector>         // std::vector
#include "Components/ActorComponent.h"
#include "Structs/SavedSpiderNavGrid.h"
#include "Interfaces/SpiderNavigationInterface.h"
#include "NavGridComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(NavGridComponent_LOG, Log, All);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPIDERNAVIGATION_API UNavGridComponent : public UActorComponent, public ISpiderNavigationInterface
{
	GENERATED_BODY()

public:	
	UNavGridComponent();

	/** Finds path in grid. Returns array of nodes */
	UFUNCTION(BlueprintCallable, Category = "SpiderNavigation")
	TArray<FVector> FindPath(FVector Start, FVector End, bool& bFoundCompletePath);

	/** Draws debug lines between connected nodes */
	UFUNCTION(BlueprintCallable, Category = "SpiderNavigation")
	void DrawDebugRelations();

	/** Finds closest node's location in grid to specified location */
	UFUNCTION(BlueprintCallable, Category = "SpiderNavigation")
	virtual FVector FindClosestNodeLocation_Implementation(FVector Location) override;

	/** Finds closest node's normal in grid to specified location  */
	UFUNCTION(BlueprintCallable, Category = "SpiderNavigation")
	FVector FindClosestNodeNormal(FVector Location);

	/** Finds path between current location and target location and returns location and normal of the next fisrt node in navigation grid */
	UFUNCTION(BlueprintCallable, Category = "SpiderNavigation")
	bool FindNextLocationAndNormal(FVector CurrentLocation, FVector TargetLocation, FVector& NextLocation, FVector& Normal);

protected:
	virtual void BeginPlay() override;

	void LoadGrid();


	void ResetGridMetrics();
	TArray<FVector> BuildPathFromEndNode(FSpiderNavNode* EndNode);

	FSpiderNavNode* FindClosestNode(FVector Location);

	TArray<FSpiderNavNode*> FindNodesPath(FSpiderNavNode* StartNode, FSpiderNavNode* EndNode, bool& bFoundCompletePath);
	TArray<FSpiderNavNode*> BuildNodesPathFromEndNode(FSpiderNavNode* EndNode);

	TArray<FSpiderNavNode*> OpenList;
	FSpiderNavNode* GetFromOpenList();
protected:
	/** Whether to load the navigation grid on BeginPlay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpiderNavigation")
	bool bAutoLoadGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Spider Navigation|Save")
	FString SaveGameName;
	UPROPERTY(EditDefaultsOnly, Category = "Spider Navigation|Save")
	int32 SaveSlotIndex;

	UPROPERTY(VisibleAnywhere, Category = "Spider Navigation|Save")
	FSavedSpiderNavGrid LoadedGrid;

	/** Thickness of debug lines */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpiderNavGridBuilder")
	float DebugLinesThickness;

private:
	TArray<class AActor*> SpiderNavGridsActors;

private:
	virtual TArray<class AActor*> GetAllSpiderNavGridsActors_Implementation() override;
	virtual TArray<FVector> FindPathBetweenPoints_Implementation(FVector StartLocation, FVector EndLocation) override;
	virtual void RegisterSpiderNavGridActor_Implementation(class AActor* SpiderNavGridActor) override;
	virtual void UnRegisterSpiderNavGridActor_Implementation(class AActor* SpiderNavGridActor) override;

	TArray<FSpiderNavNode*> FindNodesPath(FSpiderNavNode* StartNode, FSpiderNavNode* EndNode);
};
