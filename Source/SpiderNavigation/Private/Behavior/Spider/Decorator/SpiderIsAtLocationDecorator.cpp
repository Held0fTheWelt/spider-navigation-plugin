// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior/Spider/Decorator/SpiderIsAtLocationDecorator.h"

#include "Behavior/BehaviorTreeHelpers.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"

#include "AIController.h"

void USpiderIsAtLocationDecorator::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetLocation.ResolveSelectedKey(*BBAsset);
	}
}

bool USpiderIsAtLocationDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool OK = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		if (APawn* Pawn = Cast<APawn>(Controller->GetPawn()))
		{
			const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
			if (BB)
			{
				FVector TargetLoc = BB->GetValueAsVector(TargetLocation.SelectedKeyName);
				float Distance = FVector::Dist(Pawn->GetActorLocation(), TargetLoc);
				return Distance <= AcceptableDistance;
			}
			else
			{
				UE_LOG(LogBehaviorTree, Warning, TEXT("USpiderIsAtLocationDecorator: No BlackboardComponent found for BehaviorTreeComponent %s"), *OwnerComp.GetName());
			}
		}
		else
		{
			UE_LOG(LogBehaviorTree, Warning, TEXT("USpiderIsAtLocationDecorator: No Pawn found for AIController %s"), *Controller->GetName());
		}
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("USpiderIsAtLocationDecorator: No AIController found"));
	}

	return false;
}
FString USpiderIsAtLocationDecorator::GetStaticDescription() const
{
	const FString AbortText = UBehaviorTreeHelpers::AbortModeToText(FlowAbortMode);

	// Zeile nur anhängen, wenn sinnvoll
	const FString AbortLine = AbortText.IsEmpty() ? TEXT("") : (AbortText + TEXT("\n"));

	return FString::Printf(
		TEXT("%sTargetLocation: %s\nAcceptable Distance: %.1f"),
		*AbortLine,
		*TargetLocation.SelectedKeyName.ToString(),
		AcceptableDistance
	);
}