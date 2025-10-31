// Copyright Yves Tanas 2025


#include "Components/NavGridComponent.h"

#include "Subsystems/SpiderNavigationSubsystem.h"

#include "Structs/SpiderNavNode.h"

DEFINE_LOG_CATEGORY(NavGridComponent_LOG);
UNavGridComponent::UNavGridComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bAutoLoadGrid = true;
	DebugLinesThickness = 0.0f;
}


void UNavGridComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoLoadGrid) {
		LoadGrid();
	}
}

void UNavGridComponent::LoadGrid()
{
	if (USpiderNavigationSubsystem* NavSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USpiderNavigationSubsystem>())
	{
		LoadedGrid = NavSubsystem->LoadGrid("SpiderNavGridSave", 0);
	}
	else
	{

	}
}

TArray<FVector> UNavGridComponent::FindPath(FVector Start, FVector End, bool& bFoundCompletePath)
{
	TArray<FVector> Path;

	FSpiderNavNode* StartNode = FindClosestNode(Start);
	FSpiderNavNode* EndNode = FindClosestNode(End);
	TArray<FSpiderNavNode*> NodesPath = FindNodesPath(StartNode, EndNode, bFoundCompletePath);

	for (int32 i = 0; i < NodesPath.Num(); i++) {
		FSpiderNavNode* Node = NodesPath[i];
		Path.Add(Node->Location);
	}

	return Path;
}

TArray<FSpiderNavNode*> UNavGridComponent::FindNodesPath(FSpiderNavNode* StartNode, FSpiderNavNode* EndNode, bool& bFoundCompletePath)
{
	TArray<FSpiderNavNode*> Path;
	FSpiderNavNode* Node = NULL;
	TArray <FSpiderNavNode*> Neighbors;

	if (!StartNode || !EndNode) {
		//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("Not found closest nodes"));
		UE_LOG(NavGridComponent_LOG, Warning, TEXT("Not found closest nodes"));
		return Path;
	}


	ResetGridMetrics();
	//OpenList.Empty();
	std::vector<FSpiderNavNode*> openList;
	TArray<FSpiderNavNode*> ClosedList;

	//OpenList.Add(StartNode);
	openList.push_back(StartNode);
	std::make_heap(openList.begin(), openList.end(), LessThanByNodeF());
	StartNode->Opened = true;


	while (openList.size()) {
		//Node = GetFromOpenList();
		std::pop_heap(openList.begin(), openList.end(), LessThanByNodeF());
		Node = openList.back();
		openList.pop_back();

		Node->Closed = true;
		if (Node != StartNode) {
			ClosedList.Add(Node);
		}

		if (Node->Index == EndNode->Index) {
			bFoundCompletePath = true;
			return BuildNodesPathFromEndNode(Node);
		}

		for (FSpiderNavNode* Neighbor : Node->Neighbors) {

			if (Neighbor->Closed) {
				continue;
			}

			//DrawDebugString(GetWorld(), Neighbor->Location, *FString::Printf(TEXT("[%d]"), Neighbor->Neighbors.Num()), NULL, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f).ToFColor(true), 20.0f, false);

			// get the distance between current node and the neighbor
			// and calculate the next g score
			float NewG = Node->G + (Neighbor->Location - Node->Location).Size();

			// check if the neighbor has not been inspected yet, or
			// can be reached with smaller cost from the current node
			if (!Neighbor->Opened || NewG < Neighbor->G) {
				Neighbor->G = NewG;
				Neighbor->H = (Neighbor->Location - EndNode->Location).Size();
				Neighbor->F = Neighbor->G + Neighbor->H;
				Neighbor->ParentIndex = Node->Index;

				if (!Neighbor->Opened) {
					//OpenList.Add(Neighbor);
					openList.push_back(Neighbor);
					std::push_heap(openList.begin(), openList.end(), LessThanByNodeF());
					Neighbor->Opened = true;
				}
				else {
					// the neighbor can be reached with smaller cost.
					// Since its f value has been updated, we have to
					// update its position in the open list
					std::make_heap(openList.begin(), openList.end(), LessThanByNodeF());
				}
			}
		}
	}

	UE_LOG(NavGridComponent_LOG, Warning, TEXT("Not found complete path"));


	//Finding closest to end
	float IterMin = 99999999999.0f;
	for (FSpiderNavNode* IterNode : ClosedList) {
		if (IterNode->F < IterMin) {
			IterMin = IterNode->F;
			Node = IterNode;
		}
		//DrawDebugString(GetWorld(), closedList[i]->Location, *FString::Printf(TEXT("[%f, %f]"), closedList[i]->F, closedList[i]->H), NULL, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f).ToFColor(true), DebugLinesThickness, false);
	}

	if (Node) {
		UE_LOG(NavGridComponent_LOG, Log, TEXT("Min F = %f"), Node->F);
		bFoundCompletePath = false;
		//DrawDebugString(GetWorld(), Node->Location, *FString::Printf(TEXT("MINH[%f]"), Node->H), NULL, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f).ToFColor(true), DebugLinesThickness, false);

		return BuildNodesPathFromEndNode(Node);
	}


	return Path;
}

FSpiderNavNode* UNavGridComponent::GetFromOpenList()
{
	float MinF = 9999999999.0f;
	FSpiderNavNode* MinNode = NULL;
	for (FSpiderNavNode* Node : OpenList) {
		if (Node->F < MinF) {
			MinF = Node->F;
			MinNode = Node;
		}
	}
	if (MinNode) {
		OpenList.Remove(MinNode);
	}

	return MinNode;
}

FSpiderNavNode* UNavGridComponent::FindClosestNode(FVector Location)
{
	FSpiderNavNode* ClosestNode = nullptr;
	float MinDistance = 999999999.f;
	for (int32 i = 0; i != LoadedGrid.NavNodes.Num(); i++) {
		float Distance = (LoadedGrid.NavNodes[i].Location - Location).Size();
		if (Distance < MinDistance) {
			MinDistance = Distance;
			ClosestNode = &(LoadedGrid.NavNodes[i]);
		}
	}

	return ClosestNode;
}

void UNavGridComponent::ResetGridMetrics()
{
	for (int32 i = 0; i != LoadedGrid.NavNodes.Num(); ++i) {
		FSpiderNavNode Node = LoadedGrid.NavNodes[i];
		Node.ResetMetrics();
		LoadedGrid.NavNodes[i] = Node;
	}
}

TArray<FVector> UNavGridComponent::BuildPathFromEndNode(FSpiderNavNode* EndNode)
{
	TArray<FVector> Path;
	TArray<int32> ReversedPathIndexes;

	ReversedPathIndexes.Add(EndNode->Index);

	FSpiderNavNode* IterNode = EndNode;
	while (IterNode->ParentIndex > -1) {
		ReversedPathIndexes.Add(IterNode->ParentIndex);
		IterNode = &LoadedGrid.NavNodes[IterNode->ParentIndex];
	}

	for (int32 i = ReversedPathIndexes.Num() - 1; i > -1; i--) {
		int32 Index = ReversedPathIndexes[i];
		Path.Add(LoadedGrid.NavNodes[Index].Location);
	}

	return Path;
}

TArray<FSpiderNavNode*> UNavGridComponent::BuildNodesPathFromEndNode(FSpiderNavNode* EndNode)
{
	TArray<FSpiderNavNode*> Path;
	TArray<int32> ReversedPathIndexes;

	ReversedPathIndexes.Add(EndNode->Index);

	FSpiderNavNode* IterNode = EndNode;
	while (IterNode->ParentIndex > -1) {
		ReversedPathIndexes.Add(IterNode->ParentIndex);
		IterNode = &LoadedGrid.NavNodes[IterNode->ParentIndex];
	}

	for (int32 i = ReversedPathIndexes.Num() - 1; i > -1; i--) {
		int32 Index = ReversedPathIndexes[i];
		Path.Add(&LoadedGrid.NavNodes[Index]);
	}

	return Path;
}

FVector UNavGridComponent::FindClosestNodeLocation_Implementation(FVector Location)
{
	FVector NodeLocation;
	FSpiderNavNode* Node = FindClosestNode(Location);
	if (Node) {
		NodeLocation = Node->Location;
	}
	return NodeLocation;
}

FVector UNavGridComponent::FindClosestNodeNormal(FVector Location)
{
	FVector NodeNormal;
	FSpiderNavNode* Node = FindClosestNode(Location);
	if (Node) {
		NodeNormal = Node->Normal;
	}
	return NodeNormal;
}

bool UNavGridComponent::FindNextLocationAndNormal(FVector CurrentLocation, FVector TargetLocation, FVector& NextLocation, FVector& Normal)
{
	FSpiderNavNode* StartNode = FindClosestNode(CurrentLocation);
	FSpiderNavNode* EndNode = FindClosestNode(TargetLocation);
	bool bFoundPartialPath;

	FSpiderNavNode* NextNode = NULL;

	TArray<FSpiderNavNode*> NodesPath = FindNodesPath(StartNode, EndNode, bFoundPartialPath);

	if (NodesPath.Num() > 1) {
		NextNode = NodesPath[1];
	}

	if (!NextNode) {
		return false;
	}

	NextLocation = NextNode->Location;
	Normal = NextNode->Normal;

	return true;
}

TArray<FVector> UNavGridComponent::FindPathBetweenPoints_Implementation(FVector StartLocation, FVector EndLocation)
{
	TArray<FVector> Path;

	FSpiderNavNode* StartNode = FindClosestNode(StartLocation);
	FSpiderNavNode* EndNode = FindClosestNode(EndLocation);
	TArray<FSpiderNavNode*> NodesPath = FindNodesPath(StartNode, EndNode);

	for (int32 i = 0; i < NodesPath.Num(); i++) {
		FSpiderNavNode* Node = NodesPath[i];
		Path.Add(Node->Location);
	}

	return Path;
}

TArray<FSpiderNavNode*> UNavGridComponent::FindNodesPath(FSpiderNavNode* StartNode, FSpiderNavNode* EndNode)
{
	TArray<FSpiderNavNode*> Path;
	FSpiderNavNode* Node = NULL;
	TArray <FSpiderNavNode*> Neighbors;

	if (!StartNode || !EndNode) {
		//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("Not found closest nodes"));
		UE_LOG(LogTemp, Warning, TEXT("Not found closest nodes"));
		return Path;
	}


	ResetGridMetrics();
	//OpenList.Empty();
	std::vector<FSpiderNavNode*> openList;
	TArray<FSpiderNavNode*> ClosedList;

	//OpenList.Add(StartNode);
	openList.push_back(StartNode);
	std::make_heap(openList.begin(), openList.end(), LessThanByNodeF());
	StartNode->Opened = true;


	while (openList.size()) {
		//Node = GetFromOpenList();
		std::pop_heap(openList.begin(), openList.end(), LessThanByNodeF());
		Node = openList.back();
		openList.pop_back();

		Node->Closed = true;
		if (Node != StartNode) {
			ClosedList.Add(Node);
		}

		if (Node->Index == EndNode->Index) {
			return BuildNodesPathFromEndNode(Node);
		}

		for (FSpiderNavNode* Neighbor : Node->Neighbors) {

			if (Neighbor->Closed) {
				continue;
			}

			//DrawDebugString(GetWorld(), Neighbor->Location, *FString::Printf(TEXT("[%d]"), Neighbor->Neighbors.Num()), NULL, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f).ToFColor(true), 20.0f, false);

			// get the distance between current node and the neighbor
			// and calculate the next g score
			float NewG = Node->G + (Neighbor->Location - Node->Location).Size();

			// check if the neighbor has not been inspected yet, or
			// can be reached with smaller cost from the current node
			if (!Neighbor->Opened || NewG < Neighbor->G) {
				Neighbor->G = NewG;
				Neighbor->H = (Neighbor->Location - EndNode->Location).Size();
				Neighbor->F = Neighbor->G + Neighbor->H;
				Neighbor->ParentIndex = Node->Index;

				if (!Neighbor->Opened) {
					//OpenList.Add(Neighbor);
					openList.push_back(Neighbor);
					std::push_heap(openList.begin(), openList.end(), LessThanByNodeF());
					Neighbor->Opened = true;
				}
				else {
					// the neighbor can be reached with smaller cost.
					// Since its f value has been updated, we have to
					// update its position in the open list
					std::make_heap(openList.begin(), openList.end(), LessThanByNodeF());
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Not found complete path"));


	//Finding closest to end
	float IterMin = 99999999999.0f;
	for (FSpiderNavNode* IterNode : ClosedList) {
		if (IterNode->F < IterMin) {
			IterMin = IterNode->F;
			Node = IterNode;
		}
		//DrawDebugString(GetWorld(), closedList[i]->Location, *FString::Printf(TEXT("[%f, %f]"), closedList[i]->F, closedList[i]->H), NULL, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f).ToFColor(true), DebugLinesThickness, false);
	}

	if (Node) {
		UE_LOG(LogTemp, Log, TEXT("Min F = %f"), Node->F);
		//DrawDebugString(GetWorld(), Node->Location, *FString::Printf(TEXT("MINH[%f]"), Node->H), NULL, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f).ToFColor(true), DebugLinesThickness, false);

		return BuildNodesPathFromEndNode(Node);
	}


	return Path;
}

void UNavGridComponent::RegisterSpiderNavGridActor_Implementation(AActor* SpiderNavGridActor)
{
	if (SpiderNavGridActor && !SpiderNavGridsActors.Contains(SpiderNavGridActor))
	{
		SpiderNavGridsActors.Add(SpiderNavGridActor);
	}
}

void UNavGridComponent::UnRegisterSpiderNavGridActor_Implementation(AActor* SpiderNavGridActor)
{
	if (SpiderNavGridActor && SpiderNavGridsActors.Contains(SpiderNavGridActor))
	{
		SpiderNavGridsActors.Remove(SpiderNavGridActor);
	}
}

TArray<AActor*> UNavGridComponent::GetAllSpiderNavGridsActors_Implementation()
{
	return SpiderNavGridsActors;
}

void UNavGridComponent::DrawDebugRelations()
{
	FColor DrawColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f).ToFColor(true);
	FColor DrawColorNormal = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f).ToFColor(true);
	float DrawDuration = 2.0f;
	bool DrawShadow = false;

	for (int32 i = 0; i != LoadedGrid.NavNodes.Num(); ++i) {
		FSpiderNavNode Nav = LoadedGrid.NavNodes[i];


		//DrawDebugString(GetWorld(), Nav.Location, *FString::Printf(TEXT("[%d]"), Nav.Neighbors.Num()), NULL, DrawColor, DrawDuration, DrawShadow);


		for (int32 j = 0; j != Nav.Neighbors.Num(); ++j) {
			FSpiderNavNode* NeighborNav = Nav.Neighbors[j];
			DrawDebugLine(
				GetWorld(),
				Nav.Location,
				NeighborNav->Location,
				DrawColor,
				false,
				DrawDuration,
				0,
				DebugLinesThickness
			);
		}

		DrawDebugLine(
			GetWorld(),
			Nav.Location,
			Nav.Location + Nav.Normal * 100.0f,
			DrawColorNormal,
			false,
			DrawDuration,
			0,
			DebugLinesThickness
		);


	}
}