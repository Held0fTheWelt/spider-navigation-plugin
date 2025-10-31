// @Yves Tanas 2020


#include "Behavior/Spider/Service/SpiderAggroCheck.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

#include "Interfaces/SpiderNavigationInterface.h"
#include "Interfaces/SpiderAIControllerInterface.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

USpiderAggroCheck::USpiderAggroCheck()
{
	CurrentLocation = FVector();
//	TargetToFollow = FBlackboardKeySelector();
	//DesiredObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	AggroRadius = 15000.f;
}

void USpiderAggroCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/*if(AIController == nullptr)
	{
		AIController = Cast<USpiderAIControllerInterface>(OwnerComp.GetAIOwner());
	}

	if (Navigation == nullptr)
	{
		TArray<UActorComponent*> NavigationComponents =GetWorld()->GetGameState()->GetComponentsByInterface(USpiderNavigationInterface::StaticClass());
		if (NavigationComponents.Num() > 0)
		{
			Navigation = Cast<USpiderNavigationInterface>(NavigationComponents[0]);
		}
	}

	if(Target == nullptr)
	{

	}*/

	//PerformAggroCheck();	
}

void USpiderAggroCheck::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	//CheckReferences();
}

void USpiderAggroCheck::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);

	//PlayerPawn = nullptr;
	Navigation = nullptr;
	AIController = nullptr;
}
//
//bool USpiderAggroCheck::CheckReferences()
//{
//	if (PlayerPawn == nullptr)
//	{
//		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
//
//		if (PlayerPawn == nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("PlayerPawn was null"));
//			return false;
//		}
//	}
//
//	if (Navigation == nullptr)
//	{
//		TArray<AActor*> Actors;
//
//		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USpiderNavigationInterface::StaticClass(), Actors);
//
//		if (Actors.Num() == 0)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("No Navigation Actor found"));
//			return false;
//		}
//
//		Navigation = Cast<ISpiderNavigationInterface>(Actors[0]);
//
//		if (Navigation == nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Could not cast Actor to Navigation Actor to interface"));
//			return false;
//		}
//	}
//
//	if (AIController == nullptr)
//	{
//		if (ActorOwner != nullptr)
//		{
//			AIController = Cast<ITrackingSpiderAIInterface>(ActorOwner);
//		}
//		else
//		{
//			return false;
//		}
//
//		if (AIController == nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Could not cast AI Controller Actor to interface"));
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void USpiderAggroCheck::PerformAggroCheck()
//{
//	CurrentLocation = AIController->GetLocation();
//
//	TArray<AActor*> SpiderActors;
//
//	if (AIController->MustCheckTargetVisibility())
//	{
//		//UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTarantulaInterface::StaticClass(), SpiderActors);
//
//		//if (SpiderActors.Num() == 0)
//		//{
//		//	UE_LOG(LogTemp, Warning, TEXT("Could not find any Spiders in scene."));
//		//}
//		//else
//		//{
//		//	FCollisionShape MySphere = FCollisionShape::MakeSphere(AggroRadius);
//		//	FCollisionQueryParams QueryParams;
//		//	QueryParams.AddIgnoredActors(SpiderActors);
//		//	TArray<FHitResult> OutResults;
//		//	GetWorld()->SweepMultiByChannel(OutResults, CurrentLocation, CurrentLocation + FVector(0, 0, 15), FQuat::Identity, ECollisionChannel::ECC_Visibility, MySphere, QueryParams);
//
//		//	for (auto HitResult : OutResults)
//		//	{
//		//		FHitResult Result;
//		//		GetWorld()->LineTraceSingleByChannel(Result, CurrentLocation, HitResult.Location, ECollisionChannel::ECC_Visibility, QueryParams);
//
//		//		if (!(Result.Actor == PlayerPawn && Result.Actor == HitResult.Actor))
//		//		{
//		//			UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollow, nullptr);
//		//			return;
//		//		}
//		//	}
//		//}
//	}
//
//	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollow, PlayerPawn);
//	UBTFunctionLibrary::SetBlackboardValueAsVector(this, TargetLocation, PlayerPawn->GetActorLocation());
//}
