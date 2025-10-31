// Copyright Yves Tanas 2025

#include "Behavior/Spider/Service/SpiderInputValueCheck.h"

#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Engine/EngineTypes.h"
#include "Engine/World.h"

#include "GameFramework/Controller.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/Pawn.h"

#include "Interfaces/SpiderNavigationInterface.h"
#include "Interfaces/SpiderAIControllerInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Subsystems/SpiderNavigationSubsystem.h"

#include "AIController.h"

USpiderInputValueCheck::USpiderInputValueCheck()
{
	Interval = 0.6f;         // alle 0.25 Sekunden pr�fen
	RandomDeviation = 0.05f;  // kleine Abweichung
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = true;
	CurrentLocation = FVector();
	//	TargetToFollow = FBlackboardKeySelector();
	DesiredObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	SenseRadius = 15000.f;
}

void USpiderInputValueCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if(!CheckReferences(OwnerComp))
	{
		return;
	}

	PerformValueCheck(OwnerComp);
}

void USpiderInputValueCheck::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void USpiderInputValueCheck::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);

	//PlayerPawn = nullptr;
	Navigation = nullptr;
	AIController = nullptr;
}

bool USpiderInputValueCheck::CheckReferences(UBehaviorTreeComponent& OwnerComp)
{
	bool bOK = true;
	//UE_LOG(LogTemp, Warning, TEXT("Checking References in SpiderInputValueCheck."));
	if (AIController == nullptr)
	{
		AIController = Cast<AActor>(OwnerComp.GetAIOwner());
	}

	if (Navigation == nullptr)
	{
		if (AGameStateBase* GameState = GetWorld()->GetGameState())
		{
			TArray<UActorComponent*> NavigationComponents = GameState->GetComponentsByInterface(USpiderNavigationInterface::StaticClass());
			if (NavigationComponents.Num() > 0)
			{
				Navigation = NavigationComponents[0];
			}
		}
	}

	/*if (AIController != nullptr)
	{
		Target = ISpiderAIControllerInterface::Execute_GetTargetActor(AIController);
	}*/

	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//UE_LOG(LogTemp, Warning, TEXT("Target Actor: %s"), *Target->GetName());

	return Navigation != nullptr && Target != nullptr;
}

void USpiderInputValueCheck::PerformValueCheck(UBehaviorTreeComponent& OwnerComp)
{
	CurrentLocation = ISpiderAIControllerInterface::Execute_GetLocation(AIController);
	bool bFoundTarget = false;
	if (ISpiderAIControllerInterface::Execute_MustCheckTargetVisibility(AIController))
	{
		TArray<AActor*> SpiderActors;


		if(Navigation != nullptr)
		{
			SpiderActors = ISpiderNavigationInterface::Execute_GetAllSpiderNavGridsActors(Navigation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Navigation is null in SpiderInputValueCheck."));
		}

		if (SpiderActors.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find any Spiders in scene."));
			
		}

		TArray<FHitResult> OutHits;
		FHitResult LineOfSightHit;

		if(UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			CurrentLocation,
			CurrentLocation + FVector(0, 0, 1), // leichter Offset reicht
			SenseRadius,
			DesiredObjectTypes,
			false,
			SpiderActors,
			EDrawDebugTrace::None,
			OutHits,
			true
		))
		{
	
			for (auto& HitResult : OutHits)
			{
				if (HitResult.GetActor() == Target)
				{
					if (UKismetSystemLibrary::LineTraceSingleForObjects(
						GetWorld(),
						CurrentLocation,
						Target->GetActorLocation(),
						DesiredObjectTypes,
						false,
						SpiderActors,
						EDrawDebugTrace::None,
						LineOfSightHit,
						true
					))
					{
						if (LineOfSightHit.GetActor() == Target)
						{
							UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
							if (BB)
							{
								BB->SetValueAsObject(TargetToFollow.SelectedKeyName, Target);
								BB->SetValueAsVector(TargetLocation.SelectedKeyName, Target->GetActorLocation());
							}
							bFoundTarget = true;
							//UE_LOG(LogTemp, Warning, TEXT("Target Found and Visible: %s"), *Target->GetName());
							break;
						}
					}
				}
			}
		}

		if (!bFoundTarget)
		{
			UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
			if (BB)
			{
				BB->SetValueAsObject(TargetToFollow.SelectedKeyName, nullptr);
				BB->SetValueAsVector(TargetLocation.SelectedKeyName, CurrentLocation);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Visibility Check is disabled."));
	}
}
