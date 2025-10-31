// Copyright Yves Tanas 2025


#include "Subsystems/SpiderNavigationSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/SpiderNavGridSaveGame.h"

DEFINE_LOG_CATEGORY(SpiderNAVSubsystem_LOG);
void USpiderNavigationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USpiderNavigationSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FSavedSpiderNavGrid USpiderNavigationSubsystem::LoadGrid(FString GridSaveName, int32 GridIndex)
{
	UE_LOG(SpiderNAVSubsystem_LOG, Log, TEXT("Start loading Spider nav data"));

	FVector* NormalRef = NULL;
	FVector Normal;
	FSavedSpiderNavGrid SavedGrid;

	USpiderNavGridSaveGame* LoadGameInstance = Cast<USpiderNavGridSaveGame>(UGameplayStatics::CreateSaveGameObject(USpiderNavGridSaveGame::StaticClass()));
	LoadGameInstance = Cast<USpiderNavGridSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	if (LoadGameInstance) {
		UE_LOG(SpiderNAVSubsystem_LOG, Log, TEXT("After getting load game instance"));
		for (auto It = LoadGameInstance->NavLocations.CreateConstIterator(); It; ++It) {
			NormalRef = LoadGameInstance->NavNormals.Find(It.Key());
			if (NormalRef) {
				Normal = *NormalRef;
			}
			else {
				Normal = FVector(0.0f, 0.0f, 1.0f);
			}
			AddGridNode(SavedGrid, It.Key(), It.Value(), Normal);
		}
		UE_LOG(SpiderNAVSubsystem_LOG, Log, TEXT("After setting locations"));

		for (auto It = LoadGameInstance->NavRelations.CreateConstIterator(); It; ++It) {
			SetGridNodeNeighbors(SavedGrid, It.Key(), It.Value().Neighbors);
		}
		UE_LOG(SpiderNAVSubsystem_LOG, Log, TEXT("After setting relations"));

		UE_LOG(SpiderNAVSubsystem_LOG, Log, TEXT("Nav Nodes Loaded: %d"), SavedGrid.GetNavNodesCount());
	}
	return SavedGrid;
}

void USpiderNavigationSubsystem::AddGridNode(FSavedSpiderNavGrid& SavedGrid, int32 SavedIndex, FVector Location, FVector Normal)
{
	FSpiderNavNode NavNode;
	NavNode.Location = Location;
	NavNode.Normal = Normal;

	int32 Index = SavedGrid.NavNodes.Add(NavNode);
	SavedGrid.NavNodes[Index].Index = Index;

	SavedGrid.NodesSavedIndexes.Add(SavedIndex, Index);
}

void USpiderNavigationSubsystem::SetGridNodeNeighbors(FSavedSpiderNavGrid& SavedGrid, int32 SavedIndex, TArray<int32> NeighborsSavedIndexes)
{
	int32* Index = SavedGrid.NodesSavedIndexes.Find(SavedIndex);
	if (Index) {
		FSpiderNavNode* NavNode = &(SavedGrid.NavNodes[*Index]);
		for (int32 i = 0; i != NeighborsSavedIndexes.Num(); ++i) {
			int32* NeighborIndex = SavedGrid.NodesSavedIndexes.Find(NeighborsSavedIndexes[i]);
			if (NeighborIndex) {
				FSpiderNavNode* NeighborNode = &(SavedGrid.NavNodes[*NeighborIndex]);
				NavNode->Neighbors.Add(NeighborNode);
			}
		}
	}
}

